#include <iostream>
#include <string>

// Template class to create pairs of items with different data types
template <class T, class V>
class Pair {
public:
    // Default constructor
    Pair() {}

    // Parameterized constructor
    Pair(T firstValue, V secondValue) : first(firstValue), second(secondValue) {}

    // Function to set the first value
    void setFirst(T newValue) {
        first = newValue;
    }

    // Function to set the second value
    void setSecond(V newValue) {
        second = newValue;
    }

    // Function to get the first value (constant method)
    T getFirst() const {
        return first;
    }

    // Function to get the second value (constant method)
    V getSecond() const {
        return second;
    }

private:
    T first;  // The first element of the pair
    V second; // The second element of the pair
};

int main() {
    // Creating and testing pairs with different data types

    // Pair of integer and double
    Pair<int, double> pair1(5, 3.14);
    std::cout << "Pair 1: (" << pair1.getFirst() << ", " << pair1.getSecond() << ")" << std::endl;
    pair1.setFirst(10);
    pair1.setSecond(6.28);
    std::cout << "Pair 1 (updated): (" << pair1.getFirst() << ", " << pair1.getSecond() << ")" << std::endl;
    std::cout << std::endl;

    // Pair of string and integer
    Pair<std::string, int> pair2("Hello", 123);
    std::cout << "Pair 2: (" << pair2.getFirst() << ", " << pair2.getSecond() << ")" << std::endl;
    pair2.setFirst("World");
    pair2.setSecond(456);
    std::cout << "Pair 2 (updated): (" << pair2.getFirst() << ", " << pair2.getSecond() << ")" << std::endl;
    std::cout << std::endl;

    // Pair of character and boolean (created using the default constructor)
    Pair<char, bool> pair3;
    pair3.setFirst('A');
    pair3.setSecond(true);
    std::cout << "Pair 3: (" << pair3.getFirst() << ", " << pair3.getSecond() << ")" << std::endl;
    pair3.setFirst('B');
    pair3.setSecond(false);
    std::cout << "Pair 3 (updated): (" << pair3.getFirst() << ", " << pair3.getSecond() << ")" << std::endl;
    std::cout << std::endl;

    return 0;
}