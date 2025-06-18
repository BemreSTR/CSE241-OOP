#include <iostream>
#include <vector>
using namespace std;
int main(){
    int n;
    cout << "Enter the number of suitors: ";
    cin >> n;
    vector<int> suitors;
    for(int i=1;i<=n;i++){
        suitors.push_back(i);
    }
    int index=0;
    while(suitors.size()>1){
        index=(index+2)%suitors.size();
        suitors.erase(suitors.begin()+index);
    }
    cout << suitors[0];
    return 0;
}