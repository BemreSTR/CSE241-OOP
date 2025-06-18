#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <iostream>
#include <cstdlib> // for rand()

// Forward declaration
class World;

/**
 * Base Robot class for the robot battle simulation
 * This class will be the parent for all robot types
 */
class Robot {
protected:
    int strength;      // Attack strength
    int hitpoints;     // Health points
    std::string name;  // Robot name (format: type_sequence)
    int x, y;          // Position in the world grid
    bool moved;        // Flag to track if robot has moved in current simulation step
    World* world;      // Reference to the world the robot exists in

public:
    // Constructors and destructor
    Robot();
    Robot(int newStrength, int newHitpoints, const std::string& newName, World* newWorld, int newX, int newY);
    virtual ~Robot();

    // Core virtual functions (to be overridden by derived classes)
    virtual std::string getType() const = 0;  // Returns the type of the robot as a string
    virtual int getDamage();                  // Calculate damage for a single attack
    virtual void move() = 0;                  // Handle movement behavior
    
    // Additional virtual functions
    virtual bool isAlive() const;             // Check if robot is still alive
    virtual void heal();                      // Heal function (only affects humanic robots)

    // Accessor methods
    int getStrength() const;
    int getHitpoints() const;
    std::string getName() const;
    int getX() const;
    int getY() const;
    bool hasMoved() const;
    
    // Mutator methods
    void setStrength(int newStrength);
    void setHitpoints(int newHitpoints);
    void setName(const std::string& newName);
    void setPosition(int newX, int newY);
    void setMoved(bool hasMoved);
    
    // Utility methods
    void takeDamage(int damage);
    void displayInfo() const;
};

/**
 * Humanic class - Base class for humanoid robots
 * Has 15% chance for tactical nuke attack
 */
class Humanic : public Robot {
public:
    Humanic();
    Humanic(int newStrength, int newHitpoints, const std::string& newName, World* newWorld, int newX, int newY);
    virtual ~Humanic();
    
    virtual int getDamage() override;  // Add 15% chance for tactical nuke
    virtual void heal() override;      // Humanic robots can heal
};

/**
 * OptimusPrime class - Derived from Humanic
 * Has 20% chance to double damage
 */
class OptimusPrime : public Humanic {
public:
    OptimusPrime();
    OptimusPrime(World* newWorld, int newX, int newY, int sequence);
    virtual ~OptimusPrime();
    
    virtual std::string getType() const override;
    virtual int getDamage() override;  // 20% chance to double damage
    virtual void move() override;
};

/**
 * Robocop class - Derived from Humanic
 * Standard humanic robot
 */
class Robocop : public Humanic {
public:
    Robocop();
    Robocop(World* newWorld, int newX, int newY, int sequence);
    virtual ~Robocop();
    
    virtual std::string getType() const override;
    virtual void move() override;
};

/**
 * Roomba class - Derived from Robot
 * Attacks twice per turn
 */
class Roomba : public Robot {
public:
    Roomba();
    Roomba(World* newWorld, int newX, int newY, int sequence);
    virtual ~Roomba();
    
    virtual std::string getType() const override;
    virtual int getDamage() override;  // Roomba attacks twice
    virtual void move() override;
};

/**
 * Bulldozer class - Derived from Robot
 * Standard robot with high hitpoints
 */
class Bulldozer : public Robot {
public:
    Bulldozer();
    Bulldozer(World* newWorld, int newX, int newY, int sequence);
    virtual ~Bulldozer();
    
    virtual std::string getType() const override;
    virtual void move() override;
};

/**
 * Kamikaze class - Derived from Robot
 * Deals damage equal to its hitpoints, then dies
 */
class Kamikaze : public Robot {
public:
    Kamikaze();
    Kamikaze(World* newWorld, int newX, int newY, int sequence);
    virtual ~Kamikaze();
    
    virtual std::string getType() const override;
    virtual int getDamage() override;  // Damage equals hitpoints, then robot dies
    virtual void move() override;
};

#endif // ROBOT_H