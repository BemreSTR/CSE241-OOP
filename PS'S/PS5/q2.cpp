#include <iostream>
#include <string>
using namespace std;

class Student {
    string name;
    int numClasses;
    string* classList;
public:
    Student() : name(""), numClasses(0), classList(nullptr) {}
    Student(const Student& o)
      : name(o.name), numClasses(o.numClasses),
        classList(o.numClasses ? new string[o.numClasses] : nullptr)
    {
        for(int i = 0; i < numClasses; ++i)
            classList[i] = o.classList[i];
    }
    Student& operator=(const Student& o) {
        if(this != &o) {
            delete[] classList;
            name = o.name;
            numClasses = o.numClasses;
            classList = numClasses ? new string[numClasses] : nullptr;
            for(int i = 0; i < numClasses; ++i)
                classList[i] = o.classList[i];
        }
        return *this;
    }
    ~Student() { delete[] classList; }

    void InputData() {
        cout << "Name: ";
        getline(cin, name);
        cout << "Number of classes: ";
        cin >> numClasses;
        classList = numClasses ? new string[numClasses] : nullptr;
        cin.ignore();
        for(int i = 0; i < numClasses; ++i) {
            cout << "Class " << (i+1) << ": ";
            getline(cin, classList[i]);
        }
    }

    void OutputData() const {
        cout << "Name: " << name << "\n";
        cout << "Classes (" << numClasses << "):\n";
        for(int i = 0; i < numClasses; ++i)
            cout << "  " << classList[i] << "\n";
    }

    void ResetClasses() {
        delete[] classList;
        classList = nullptr;
        numClasses = 0;
    }
};

int main() {
    Student s1, s2;
    s1.InputData();
    cout << "\nStudent 1's data:\n";
    s1.OutputData();

    cout << "\nAssigning s1 to s2...\n";
    s2 = s1;
    cout << "Student 2's data after assignment:\n";
    s2.OutputData();

    cout << "\nResetting s1...\n";
    s1.ResetClasses();
    cout << "Student 1's data after reset:\n";
    s1.OutputData();

    cout << "Student 2's data (unchanged):\n";
    s2.OutputData();
    return 0;
}