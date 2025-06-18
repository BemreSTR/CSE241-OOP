#include <iostream>

using namespace std;

int main() {
    for (int i = 3; i <= 100; ++i) {
        bool check = true;
        for (int j = 2; j < i; ++j) {
            if (i % j == 0) {
                check = false;
                break;
            }
        }
        if (check) {
            cout << i << " ";
        }
    }
    cout << "\n";
    return 0;
}
