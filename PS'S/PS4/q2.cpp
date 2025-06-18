//Define a class named MyInteger that stores an integer and has functions to get and set the integer’s value. 
//Then, overload the [] operator so that the index returns the digit in position i, where i = 0 is the least-significant digit. 
//If no such digit exists then −1 should be returned.
//For example, if x is of type MyInteger and is set to 418, then x[0] should return 8, x[1] should return 1, x[2] should return 4, and x[3] should return −1.

#include <iostream>
#include <cmath>
using namespace std;
class MyInteger {
private:
    int value;
public:
    // Constructor
    MyInteger(int value = 0) : value(value) {}

    // Getter for value
    int getValue() const {
        return value;
    }

    // Setter for value
    void setValue(int value) {
        this->value = value;
    }

    // Overload [] operator to return the digit at position i
    int operator[](int i) const {
        if (i < 0) return -1; // Invalid index
        int temp = abs(value);
        for (int j = 0; j < i; j++) {
            temp /= 10;
            if (temp == 0) return -1; // No such digit exists
        }
        return temp % 10;
    }
};
int main() {
    MyInteger x(418);
    cout << "x[0] = " << x[0] << endl; // Should return 8
    cout << "x[1] = " << x[1] << endl; // Should return 1
    cout << "x[2] = " << x[2] << endl; // Should return 4
    cout << "x[3] = " << x[3] << endl; // Should return -1
    return 0;
}