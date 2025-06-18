#include "Robot.h"
#include "World.h"
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

using namespace std;

// =====================
// Robot Base Class Implementation
// =====================

Robot::Robot() : strength(0), hitpoints(0), name(""), x(0), y(0), moved(false), world(nullptr) {
}

Robot::Robot(int newStrength, int newHitpoints, const string& newName, World* newWorld, int newX, int newY) 
    : strength(newStrength), hitpoints(newHitpoints), name(newName), 
      x(newX), y(newY), moved(false), world(newWorld) {
    // World grid setting is handled by World class
}

Robot::~Robot() {
    // Base destructor - nothing to deallocate here
}

int Robot::getDamage() {
    // Base damage calculation for all robots (random number between 1 and strength)
    int damage = (rand() % strength) + 1;
    cout << getType() << " attacks for " << damage << " points!" << endl;
    return damage;
}

bool Robot::isAlive() const {
    return hitpoints > 0;
}

void Robot::heal() {
    // Base implementation - most robots don't heal
    // This will be overridden by Humanic robots
}

// Accessor methods
int Robot::getStrength() const {
    return strength;
}

int Robot::getHitpoints() const {
    return hitpoints;
}

string Robot::getName() const {
    return name;
}

int Robot::getX() const {
    return x;
}

int Robot::getY() const {
    return y;
}

bool Robot::hasMoved() const {
    return moved;
}

// Mutator methods
void Robot::setStrength(int newStrength) {
    strength = newStrength;
}

void Robot::setHitpoints(int newHitpoints) {
    hitpoints = newHitpoints;
}

void Robot::setName(const string& newName) {
    name = newName;
}

void Robot::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Robot::setMoved(bool hasMoved) {
    moved = hasMoved;
}

void Robot::takeDamage(int damage) {
    hitpoints -= damage;
}

void Robot::displayInfo() const {
    cout << "Robot: " << name << " (" << getType() << ")" << endl;
    cout << "HP: " << hitpoints << ", Strength: " << strength << endl;
    cout << "Position: (" << x << ", " << y << ")" << endl;
}

// =====================
// Humanic Class Implementation
// =====================

Humanic::Humanic() : Robot() {
}

Humanic::Humanic(int newStrength, int newHitpoints, const string& newName, 
                World* newWorld, int newX, int newY)
    : Robot(newStrength, newHitpoints, newName, newWorld, newX, newY) {
}

Humanic::~Humanic() {
    // Nothing specific to deallocate
}

int Humanic::getDamage() {
    // First get base damage from Robot
    int damage = Robot::getDamage();
    
    // 15% chance of inflicting tactical nuke (40 additional damage)
    if ((rand() % 100) < 15) {
        cout << "Humanic robot " << getName() << " inflicts a TACTICAL NUKE attack!" << endl;
        damage += 40;
    }
    
    return damage;
}

void Humanic::heal() {
    // Humanic robots heal 1 hit point per step
    hitpoints++;
    // Can't heal beyond initial values, assuming:
    // OptimusPrime max = 100, Robocop max = 40
    int maxHitpoints = (getType() == "optimusprime") ? 100 : 40;
    if (hitpoints > maxHitpoints) {
        hitpoints = maxHitpoints;
    }
}

// =====================
// OptimusPrime Class Implementation
// =====================

OptimusPrime::OptimusPrime() : Humanic() {
}

OptimusPrime::OptimusPrime(World* newWorld, int newX, int newY, int sequence)
    : Humanic(100, 100, "optimusprime_" + to_string(sequence), newWorld, newX, newY) {
}

OptimusPrime::~OptimusPrime() {
    // Nothing specific to deallocate
}

string OptimusPrime::getType() const {
    return "optimusprime";
}

int OptimusPrime::getDamage() {
    // Get the Humanic damage calculation first
    int damage = Humanic::getDamage();
    
    // 20% chance of doubling damage
    if ((rand() % 100) < 20) {
        cout << "OptimusPrime " << getName() << " inflicts a STRONG attack, doubling damage!" << endl;
        damage *= 2;
    }
    
    return damage;
}

void OptimusPrime::move() {
    // Choose a random direction
    int direction = rand() % 4; // 0: up, 1: right, 2: down, 3: left
    bool keepMoving = true;
    int currentX = x;
    int currentY = y;
    
    while (keepMoving) {
        int newX = currentX;
        int newY = currentY;
        
        // Take a step in the chosen direction
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }
        
        // Check if it's a valid move
        if (world) {
            // If tryMove returns true (moved to empty cell), 
            // continue in the same direction
            // If false (found another robot or hit wall), 
            // stop moving
            if (!world->tryMove(this, newX, newY)) {
                keepMoving = false;
            } else {
                // Position updated, get new position
                currentX = x;
                currentY = y;
            }
        } else {
            keepMoving = false;
        }
    }
}

// =====================
// Robocop Class Implementation
// =====================

Robocop::Robocop() : Humanic() {
}

Robocop::Robocop(World* newWorld, int newX, int newY, int sequence)
    : Humanic(30, 40, "robocop_" + to_string(sequence), newWorld, newX, newY) {
}

Robocop::~Robocop() {
    // Nothing specific to deallocate
}

string Robocop::getType() const {
    return "robocop";
}

void Robocop::move() {
    // Choose a random direction
    int direction = rand() % 4; // 0: up, 1: right, 2: down, 3: left
    bool keepMoving = true;
    int currentX = x;
    int currentY = y;
    
    while (keepMoving) {
        int newX = currentX;
        int newY = currentY;
        
        // Take a step in the chosen direction
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }
        
        // Check if it's a valid move
        if (world) {
            // If tryMove returns true (moved to empty cell), 
            // continue in the same direction
            // If false (found another robot or hit wall), 
            // stop moving
            if (!world->tryMove(this, newX, newY)) {
                keepMoving = false;
            } else {
                // Position updated, get new position
                currentX = x;
                currentY = y;
            }
        } else {
            keepMoving = false;
        }
    }
}

// =====================
// Roomba Class Implementation
// =====================

Roomba::Roomba() : Robot() {
}

Roomba::Roomba(World* newWorld, int newX, int newY, int sequence)
    : Robot(3, 10, "roomba_" + to_string(sequence), newWorld, newX, newY) {
}

Roomba::~Roomba() {
    // Nothing specific to deallocate
}

string Roomba::getType() const {
    return "roomba";
}

int Roomba::getDamage() {
    // Roomba attacks twice
    int damage1 = Robot::getDamage();
    cout << getName() << " attacks again as it's very fast!" << endl;
    int damage2 = (rand() % strength) + 1; // Second attack calculation
    cout << getType() << " attacks for " << damage2 << " more points!" << endl;
    
    return damage1 + damage2;
}

void Roomba::move() {
    // Choose a random direction
    int direction = rand() % 4; // 0: up, 1: right, 2: down, 3: left
    bool keepMoving = true;
    int currentX = x;
    int currentY = y;
    
    while (keepMoving) {
        int newX = currentX;
        int newY = currentY;
        
        // Take a step in the chosen direction
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }
        
        // Check if it's a valid move
        if (world) {
            // If tryMove returns true (moved to empty cell), 
            // continue in the same direction
            // If false (found another robot or hit wall), 
            // stop moving
            if (!world->tryMove(this, newX, newY)) {
                keepMoving = false;
            } else {
                // Position updated, get new position
                currentX = x;
                currentY = y;
            }
        } else {
            keepMoving = false;
        }
    }
}

// =====================
// Bulldozer Class Implementation
// =====================

Bulldozer::Bulldozer() : Robot() {
}

Bulldozer::Bulldozer(World* newWorld, int newX, int newY, int sequence)
    : Robot(50, 200, "bulldozer_" + to_string(sequence), newWorld, newX, newY) {
}

Bulldozer::~Bulldozer() {
    // Nothing specific to deallocate
}

string Bulldozer::getType() const {
    return "bulldozer";
}

void Bulldozer::move() {
    // Choose a random direction
    int direction = rand() % 4; // 0: up, 1: right, 2: down, 3: left
    bool keepMoving = true;
    int currentX = x;
    int currentY = y;
    
    while (keepMoving) {
        int newX = currentX;
        int newY = currentY;
        
        // Take a step in the chosen direction
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }
        
        // Check if it's a valid move
        if (world) {
            // If tryMove returns true (moved to empty cell), 
            // continue in the same direction
            // If false (found another robot or hit wall), 
            // stop moving
            if (!world->tryMove(this, newX, newY)) {
                keepMoving = false;
            } else {
                // Position updated, get new position
                currentX = x;
                currentY = y;
            }
        } else {
            keepMoving = false;
        }
    }
}

// =====================
// Kamikaze Class Implementation
// =====================

Kamikaze::Kamikaze() : Robot() {
}

Kamikaze::Kamikaze(World* newWorld, int newX, int newY, int sequence)
    : Robot(10, 10, "kamikaze_" + to_string(sequence), newWorld, newX, newY) {
}

Kamikaze::~Kamikaze() {
    // Nothing specific to deallocate
}

string Kamikaze::getType() const {
    return "kamikaze";
}

int Kamikaze::getDamage() {
    // Kamikaze inflicts damage equal to its hitpoints
    int damage = hitpoints;
    cout << getName() << " performs a KAMIKAZE attack for " << damage << " points!" << endl;
    
    // Kamikaze dies after attack
    hitpoints = 0;
    
    return damage;
}

void Kamikaze::move() {
    // Choose a random direction
    int direction = rand() % 4; // 0: up, 1: right, 2: down, 3: left
    bool keepMoving = true;
    int currentX = x;
    int currentY = y;
    
    while (keepMoving) {
        int newX = currentX;
        int newY = currentY;
        
        // Take a step in the chosen direction
        switch (direction) {
            case 0: newY--; break; // Up
            case 1: newX++; break; // Right
            case 2: newY++; break; // Down
            case 3: newX--; break; // Left
        }
        
        // Check if it's a valid move
        if (world) {
            // If tryMove returns true (moved to empty cell), 
            // continue in the same direction
            // If false (found another robot or hit wall), 
            // stop moving
            if (!world->tryMove(this, newX, newY)) {
                keepMoving = false;
            } else {
                // Position updated, get new position
                currentX = x;
                currentY = y;
            }
        } else {
            keepMoving = false;
        }
    }
}