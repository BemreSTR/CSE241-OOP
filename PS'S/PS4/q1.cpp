// Write a definition for a class named Vector2D that stores information about two_dimensional vectors.
// The class should methods to get and set the x and y components of the vector,where x and y integers.
// Next, overload the * operator so that it returns the dot product of two vectors.
// The dot product of two vectors (x1, y1) and (x2, y2) is defined as x1 * x2 + y1 * y2.

#include <iostream>
using namespace std;
class Vector2D {
private:
    int x, y;
public:
    // Constructor
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    // Getter for x
    int getX() const {
        return x;
    }

    // Getter for y
    int getY() const {
        return y;
    }

    // Setter for x
    void setX(int x) {
        this->x = x;
    }

    // Setter for y
    void setY(int y) {
        this->y = y;
    }

    // Overload * operator for dot product
    int operator*(const Vector2D& other) const {
        return (x * other.x + y * other.y);
    }
};

int main(){
    Vector2D v1(10,0), v2(0,10), v3(10,10), v4(5,4);

    cout << "(" << v1.getX() << "," << v1.getY() << ") * (" << v2.getX() << "," << v2.getY() << ") = " << (v1*v2) << endl;
    cout << "(" << v2.getX() << "," << v2.getY() << ") * (" << v3.getX() << "," << v3.getY() << ") = " << (v1*v3) << endl;
    cout << "(" << v3.getX() << "," << v3.getY() << ") * (" << v4.getX() << "," << v4.getY() << ") = " << (v1*v4) << endl;
    cout << "(" << v2.getX() << "," << v2.getY() << ") * (" << v3.getX() << "," << v3.getY() << ") = " << (v2*v3) << endl;
    cout << "(" << v2.getX() << "," << v2.getY() << ") * (" << v4.getX() << "," << v4.getY() << ") = " << (v2*v4) << endl;
    return 0;

}





























