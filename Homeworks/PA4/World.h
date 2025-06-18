#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <vector>
#include <string>

// Forward declaration for Robot class
class Robot;

/**
 * World class represents the simulation grid where robots interact
 * Manages a 10x10 grid of Robot pointers and simulation logic
 */
class World {
private:
    static const int GRID_SIZE = 10;          // Size of the grid (10x10)
    static const int INITIAL_ROBOTS = 5;      // Initial count of each robot type
    
    Robot* grid[GRID_SIZE][GRID_SIZE];        // 2D grid of Robot pointers
    int robotCount;                           // Current number of robots alive
    int stepCount;                            // Current simulation step

public:
    // Constructor and destructor
    World();
    ~World();
    
    // Core simulation functions
    void initialize();                       // Set up initial robots
    bool simulateOneStep();                  // Simulate one time step, return false if simulation ends
    void resetMoveFlags();                   // Reset moved flags before each step
    void displayGrid() const;                // Display the current state of the grid
    
    // Combat functions
    void fight(Robot* attacker, Robot* defender);  // Handle fight between two robots
    void printHitMessage(Robot* attacker, Robot* defender, int damage, int defenderHpBefore);
    
    // Grid management functions
    Robot* getAt(int x, int y) const;        // Get robot at specific location
    void setAt(int x, int y, Robot* robot);  // Set robot at specific location
    void removeAt(int x, int y);             // Remove robot at specific location
    
    // Robot creation functions
    void createOptimusPrime(int x, int y, int sequence);
    void createRobocop(int x, int y, int sequence);
    void createRoomba(int x, int y, int sequence);
    void createBulldozer(int x, int y, int sequence);
    void createKamikaze(int x, int y, int sequence);
    
    // Utility functions
    bool isValidPosition(int x, int y) const;
    bool isEmpty(int x, int y) const;
    std::pair<int, int> getRandomEmptyCell();
    int getRobotCount() const;
    int getStepCount() const;
    
    // Random position generators (for movement)
    std::pair<int, int> getRandomAdjacentPosition(int x, int y);
    bool tryMove(Robot* robot, int newX, int newY);
};

#endif // WORLD_H