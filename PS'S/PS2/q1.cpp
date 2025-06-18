#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
int main(){
    srand(time(0));
    const int simulationCount = 10000;
    int winAaron = 0;
    int winBob = 0;
    int winCharlie = 0;
    // Simulate 10.000 duels
    for(int i = 0; i < simulationCount; i++){
        bool aliveAaron = true;
        bool aliveBob = true;
        bool aliveCharlie = true;
        // Duel continues until only one person is alive
        while((aliveAaron + aliveBob + aliveCharlie) > 1){
            // Aaron's shot: If Aaron is still alive
            if(aliveAaron){
                if(aliveCharlie){
                    if((rand() / (double)RAND_MAX) <= (1.0/3.0)) aliveCharlie = false;
                } else if(aliveBob){
                    if((rand() / (double)RAND_MAX) <= (1.0/3.0)) aliveBob = false;
                }
            }
            // If only one person is left in the duel, break the loop
            if((aliveAaron + aliveBob + aliveCharlie) == 1) break;
            // Bob's shot: If Bob is still alive
            if(aliveBob){
                if(aliveCharlie){
                    if((rand() / (double)RAND_MAX) <= 0.5) aliveCharlie = false;
                } else if(aliveAaron){
                    if((rand() / (double)RAND_MAX) <= 0.5) aliveAaron = false;
                }
            }
            // Check if only one person is left
            if((aliveAaron + aliveBob + aliveCharlie) == 1) break;
            // Charlie's shot: If Charlie is still alive
            if(aliveCharlie){
                if(aliveBob){
                    aliveBob = false;
                } else if(aliveAaron){
                    aliveAaron = false;
                }
            }
        }
        if(aliveAaron) winAaron++;
        if(aliveBob) winBob++;
        if(aliveCharlie) winCharlie++;
    }
    // Print the results
    cout << "Aaron won: " << winAaron << "/" << simulationCount << " (" << (winAaron * 100.0 / simulationCount) << "%)" << endl;
    cout << "Bob won: " << winBob << "/" << simulationCount << " (" << (winBob * 100.0 / simulationCount) << "%)" << endl;
    cout << "Charlie won: " << winCharlie << "/" << simulationCount << " (" << (winCharlie * 100.0 / simulationCount) << "%)" << endl;
    return 0;
}