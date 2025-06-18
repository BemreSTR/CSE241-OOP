#include "World.h"
#include "Robot.h"
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <string>

using namespace std;

int main() {
    cout << "=======================================" << endl;
    cout << "       ROBOT BATTLE SIMULATION         " << endl;
    cout << "=======================================" << endl;
    cout << "5 of each robot type will be created:" << endl;
    cout << "- OptimusPrime (HP:100, STR:100)" << endl;
    cout << "- Robocop (HP:40, STR:30)" << endl;
    cout << "- Roomba (HP:10, STR:3)" << endl;
    cout << "- Bulldozer (HP:200, STR:50)" << endl;
    cout << "- Kamikaze (HP:10, STR:10)" << endl;
    cout << "=======================================" << endl;
    
    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Create and initialize world
    World world;
    world.initialize();
    
    // Display initial grid state
    cout << "\nInitial grid state:" << endl;
    world.displayGrid();
    
    string input;
    bool continueSimulation = true;
    
    // Simulation loop
    while (continueSimulation) {
        cout << "\nPress Enter to simulate one step (or type 'q' to quit): ";
        getline(cin, input);
        
        // Check if user wants to quit
        if (input == "q" || input == "Q") {
            cout << "Simulation terminated by user." << endl;
            break;
        }
        
        // Simulate one step
        continueSimulation = world.simulateOneStep();
        
        // Display the grid after the step
        if (continueSimulation) {
            world.displayGrid();
        }
    }
    
    cout << "\nSimulation ended after " << world.getStepCount() << " steps." << endl;
    
    return 0;
}