#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int humanTurn(int &score);
int computerTurn(int &score);

int main() {
    srand(time(0));
    int humanScore = 0, computerScore = 0;
    bool humanTurnFlag = true;

    cout << "Welcome to the Game of Pig!\n";
    while (humanScore < 100 && computerScore < 100) {
        if (humanTurnFlag) {
            cout << "\nYour Score: " << humanScore << " | Computer Score: " << computerScore << endl;
            humanScore += humanTurn(humanScore);
            humanTurnFlag = false;
        } else {
            computerScore += computerTurn(computerScore);
            humanTurnFlag = true;
        }
    }
    cout << "\nYour Score: " << humanScore << " | Computer Score: " << computerScore;
    cout << (humanScore >= 100 ? "\nCongratulations! You win!" : "\nThe computer wins! Better luck next time.") << endl;
    return 0;

}

int humanTurn(int &score) {
    int turnTotal = 0, roll;
    char choice;
    do {
        roll = (rand() % 6) + 1;
        cout << "You rolled: " << roll << endl;
        if (roll == 1){
            cout << "You failed !!!!";
            return 0;
        }
        turnTotal += roll;
        cout << "Turn total: " << turnTotal << " | Score: " << score << "\nRoll again (R) or Hold (H)? ";
        cin >> choice;
    } while (toupper(choice) == 'R');
    return turnTotal;
}

int computerTurn(int &score){
    int turnTotal = 0, roll;
    cout << "\nComputer's Turn..." << endl;
    while (turnTotal < 20){
        roll = (rand() % 6) + 1;
        cout << "Computer rolled: " << roll;
        if (roll == 1){
            cout << "\nComputer failed !!!";
            return 0;
        }
        turnTotal += roll;
        cout << " | Turn Total:" << turnTotal << endl;
    }
    return turnTotal;
}