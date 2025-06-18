#include "World.h"
#include "Robot.h"
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <utility> // for pair

using namespace std;

// =====================
// World Class Implementation
// =====================

World::World() : robotCount(0), stepCount(0) {
    // Initialize the grid to nullptr
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = nullptr;
        }
    }
}

World::~World() {
    // Clean up all robots in the grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != nullptr) {
                delete grid[i][j];
                grid[i][j] = nullptr;
            }
        }
    }
}

void World::initialize() {
    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Create 5 of each robot type and place them randomly
    for (int i = 0; i < INITIAL_ROBOTS; i++) {
        // Create OptimusPrime
        auto pos = getRandomEmptyCell();
        createOptimusPrime(pos.first, pos.second, i);
        
        // Create Robocop
        pos = getRandomEmptyCell();
        createRobocop(pos.first, pos.second, i);
        
        // Create Roomba
        pos = getRandomEmptyCell();
        createRoomba(pos.first, pos.second, i);
        
        // Create Bulldozer
        pos = getRandomEmptyCell();
        createBulldozer(pos.first, pos.second, i);
        
        // Create Kamikaze
        pos = getRandomEmptyCell();
        createKamikaze(pos.first, pos.second, i);
    }
    
    // Set the initial robot count
    robotCount = INITIAL_ROBOTS * 5;
}

bool World::simulateOneStep() {
    stepCount++;
    cout << "===== Step " << stepCount << " =====" << endl;
    
    // Reset all move flags before processing
    resetMoveFlags();
    
    // Process each cell in the grid for movement and combat
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // It's crucial to use the current state of grid[i][j]
            // as it might change during the loop if robots move into this cell
            // after the original occupant moved out or died.
            Robot* currentRobot = grid[i][j]; 
            
            // Skip empty cells and robots that have already moved
            if (currentRobot == nullptr || currentRobot->hasMoved()) {
                continue;
            }
            
            // Mark this robot as moved
            currentRobot->setMoved(true);
            
            // Let the robot move (handled by the Robot's move() method)
            // This call might result in currentRobot being deleted or moved from grid[i][j]
            currentRobot->move(); 
            
            // DO NOT call heal() here on 'currentRobot' if it might have been deleted.
        }
    }

    // After all robots have attempted to move and fight, apply healing
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != nullptr && grid[i][j]->isAlive()) {
                // heal() method itself should check if the robot is Humanic
                grid[i][j]->heal(); 
            }
        }
    }
    
    // Recalculate robot count at the end of each step
    int actualRobotCount = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != nullptr && grid[i][j]->isAlive()) {
                actualRobotCount++;
            }
        }
    }
    robotCount = actualRobotCount;
        
    // Check if only one robot remains
    if (robotCount <= 1) {
        // Find the last robot standing and announce winner
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (grid[i][j] != nullptr) {
                    cout << "===== SIMULATION ENDED =====" << endl;
                    cout << "Winner: " << grid[i][j]->getName() << " (" << grid[i][j]->getType() << ")" << endl;
                    cout << "HP remaining: " << grid[i][j]->getHitpoints() << endl;
                    cout << "============================" << endl;
                    return false; // Simulation should end
                }
            }
        }
        
        // No robots found at all
        cout << "===== SIMULATION ENDED =====" << endl;
        cout << "No robots remain! It's a draw." << endl;
        cout << "============================" << endl;
        return false;
    }
    
    return true; // Simulation should continue
}

// Reset the moved flags for all robots in the grid
void World::resetMoveFlags() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != nullptr) {
                grid[i][j]->setMoved(false);
            }
        }
    }
}

// Display the current state of the grid
void World::displayGrid() const {
    cout << "   ";
    for (int i = 0; i < GRID_SIZE; i++) {
        cout << i << " ";
    }
    cout << endl;
    
    for (int j = 0; j < GRID_SIZE; j++) {
        cout << j << "  ";
        for (int i = 0; i < GRID_SIZE; i++) {
            if (grid[i][j] == nullptr) {
                cout << ". ";
            } else {
                // Display first letter of robot type
                cout << grid[i][j]->getType()[0] << " ";
            }
        }
        cout << endl;
    }
    
    cout << "\nRemaining robots: " << robotCount << endl;
}

// Handle the fight between two robots
// This function is called when two robots encounter each other
void World::fight(Robot* attacker, Robot* defender) {
    cout << "\n=== FIGHT BEGINS ===" << endl;
    cout << attacker->getName() << " (" << attacker->getType() << ") vs. " 
         << defender->getName() << " (" << defender->getType() << ")" << endl;
    
    while (attacker->isAlive() && defender->isAlive()) {
        // Attacker attacks first
        int attackerDamage = attacker->getDamage();
        int defenderHpBefore = defender->getHitpoints();

        defender->takeDamage(attackerDamage);
        printHitMessage(attacker, defender, attackerDamage, defenderHpBefore);
        
        // Check if defender died
        if (!defender->isAlive()) {
            cout << defender->getName() << " has been destroyed!" << endl;
            robotCount--;
            break;
        }
        
        // Defender counter-attacks
        int defenderDamage = defender->getDamage();
        int attackerHpBefore = attacker->getHitpoints();
        
        // Defender attacks
        // Note: The defender's damage is calculated after the attacker's damage
        attacker->takeDamage(defenderDamage);
        printHitMessage(defender, attacker, defenderDamage, attackerHpBefore);
        
        // Check if attacker died
        if (!attacker->isAlive()) {
            cout << attacker->getName() << " has been destroyed!" << endl;
            robotCount--;
            break;
        }
    }
    
    cout << "=== FIGHT ENDS ===" << endl;
}

void World::printHitMessage(Robot* attacker, Robot* defender, int damage, int defenderHpBefore) {
    cout << attacker->getName() << "(" << attacker->getHitpoints() 
         << ") hits " << defender->getName() << "(" << defenderHpBefore 
         << ") with " << damage << endl;
    cout << "The new hitpoints of " << defender->getName() << " is " 
         << defender->getHitpoints() << endl;
}
// Get the robot at the specified position
Robot* World::getAt(int x, int y) const {
    if (isValidPosition(x, y)) {
        return grid[x][y];
    }
    return nullptr;
}
// Set the robot at the specified position
void World::setAt(int x, int y, Robot* robot) {
    if (isValidPosition(x, y)) {
        grid[x][y] = robot;
    }
}
// Remove the robot at the specified position
void World::removeAt(int x, int y) {
    if (isValidPosition(x, y) && grid[x][y] != nullptr) {
        delete grid[x][y];
        grid[x][y] = nullptr;
    }
}

void World::createOptimusPrime(int x, int y, int sequence) {
    if (isValidPosition(x, y) && grid[x][y] == nullptr) {
        grid[x][y] = new OptimusPrime(this, x, y, sequence);
    }
}

void World::createRobocop(int x, int y, int sequence) {
    if (isValidPosition(x, y) && grid[x][y] == nullptr) {
        grid[x][y] = new Robocop(this, x, y, sequence);
    }
}

void World::createRoomba(int x, int y, int sequence) {
    if (isValidPosition(x, y) && grid[x][y] == nullptr) {
        grid[x][y] = new Roomba(this, x, y, sequence);
    }
}

void World::createBulldozer(int x, int y, int sequence) {
    if (isValidPosition(x, y) && grid[x][y] == nullptr) {
        grid[x][y] = new Bulldozer(this, x, y, sequence);
    }
}

void World::createKamikaze(int x, int y, int sequence) {
    if (isValidPosition(x, y) && grid[x][y] == nullptr) {
        grid[x][y] = new Kamikaze(this, x, y, sequence);
    }
}

bool World::isValidPosition(int x, int y) const {
    return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE);
}

bool World::isEmpty(int x, int y) const {
    return isValidPosition(x, y) && (grid[x][y] == nullptr);
}
// Get a random empty cell in the grid
// This function is used for placing new robots
pair<int, int> World::getRandomEmptyCell() {
    int x, y;
    do {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
    } while (!isEmpty(x, y));
    
    return make_pair(x, y);
}

int World::getRobotCount() const {
    return robotCount;
}

int World::getStepCount() const {
    return stepCount;
}
// Get a random adjacent position (up, down, left, right) from the given position
// This function is used for robot movement
// It returns a pair of integers representing the new position
pair<int, int> World::getRandomAdjacentPosition(int x, int y) {
    int direction = rand() % 4;  // 0: up, 1: right, 2: down, 3: left
    int newX = x, newY = y;
    
    switch (direction) {
        case 0: newY--; break; // Up
        case 1: newX++; break; // Right
        case 2: newY++; break; // Down
        case 3: newX--; break; // Left
    }
    
    return make_pair(newX, newY);
}
// Attempt to move the robot to a new position
// If the move is valid, update the grid and robot's position
bool World::tryMove(Robot* robot, int newX, int newY) {
    if (!isValidPosition(newX, newY)) {
        return false; // Can't move outside the grid
    }
    
    int oldX = robot->getX();
    int oldY = robot->getY();
    
    // If destination is empty, move there
    if (isEmpty(newX, newY)) {
        // Update grid
        grid[newX][newY] = robot;
        grid[oldX][oldY] = nullptr;
        
        // Update robot's position
        robot->setPosition(newX, newY);
        return true;
    }
    // If destination has another robot, initiate fight
    else {
        Robot* defender = grid[newX][newY];
        fight(robot, defender);
        
        // If attacker survived and defender died, move to new position
        if (robot->isAlive() && !defender->isAlive()) {
            delete defender;  // Remove the dead defender
            grid[newX][newY] = robot;
            grid[oldX][oldY] = nullptr;
            robot->setPosition(newX, newY);
            return true;
        }
        // If attacker died, remove from old position
        else if (!robot->isAlive()) {
            grid[oldX][oldY] = nullptr;
            delete robot;
            return false;
        }
        // Both survived, no movement
        return false;
    }
}