#include <iostream>
#include <string>
using namespace std;

// ====== Exception Base Class ======
class Exception {
};

// ====== OutOfRange Exception Class ======
class OutOfRange : public Exception {
public:
    OutOfRange(string thisMessage, int thisIndex)
        : message(thisMessage), index(thisIndex) {}

    string getMessage() const {
        return message;
    }

    int getIndex() const {
        return index;
    }

private:
    string message;
    int index;
};

// ====== PFArrayD Class Definition ======
class PFArrayD {
public:
    PFArrayD()
        : capacity(50), used(0) {
        a = new double[capacity];
    }

    PFArrayD(int capacityValue)
        : capacity(capacityValue), used(0) {
        a = new double[capacity];
    }

    PFArrayD(const PFArrayD& pfaObject)
        : capacity(pfaObject.capacity), used(pfaObject.used) {
        a = new double[capacity];
        for (int i = 0; i < used; i++)
            a[i] = pfaObject.a[i];
    }

    void addElement(double element) {
        if (used >= capacity) {
            throw OutOfRange("Array is full. Cannot add element.", used);
        }
        a[used++] = element;
    }

    bool full() const {
        return capacity == used;
    }

    int getCapacity() const {
        return capacity;
    }

    int getNumberUsed() const {
        return used;
    }

    void emptyArray() {
        used = 0;
    }

    double& operator[](int index) {
        if (index < 0 || index >= used) {
            throw OutOfRange("Index out of range.", index);
        }
        return a[index];
    }

    PFArrayD& operator=(const PFArrayD& rightSide) {
        if (this != &rightSide) {
            delete[] a;
            capacity = rightSide.capacity;
            used = rightSide.used;
            a = new double[capacity];
            for (int i = 0; i < used; i++)
                a[i] = rightSide.a[i];
        }
        return *this;
    }

    ~PFArrayD() {
        delete[] a;
    }

private:
    double* a;
    int capacity;
    int used;
};

// ====== MAIN FUNCTION ======
int main() {
    PFArrayD myArray(5); // capacity = 5

    try {
        cout << "Adding elements to array:\n";
        for (int i = 0; i < 5; ++i) {
            myArray.addElement(i * 1.1);
            cout << "Added: " << myArray[i] << endl;
        }

        // Try to exceed capacity
        cout << "Attempting to add one more element...\n";
        myArray.addElement(99.9); // Should throw exception

    } catch (OutOfRange& e) {
        cerr << "Exception Caught: " << e.getMessage()
             << " [Index: " << e.getIndex() << "]" << endl;
    }

    try {
        cout << "Accessing element at index 10...\n";
        double x = myArray[10]; // Should throw exception

    } catch (OutOfRange& e) {
        cerr << "Exception Caught: " << e.getMessage()
             << " [Index: " << e.getIndex() << "]" << endl;
    }

    return 0;
}
