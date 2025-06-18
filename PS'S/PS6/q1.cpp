#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

class DynamicStringArray
{
public:
    DynamicStringArray();
    DynamicStringArray(const DynamicStringArray& other);
    ~DynamicStringArray();
    DynamicStringArray& operator=(const DynamicStringArray& other);
    void addEntry(string newEntry);
    bool deleteEntry(string newEntry);
    string getEntry(int index);
    int getSize();

private:
    string *dynamicArray;
    int size;
};

// Default constructor
DynamicStringArray::DynamicStringArray() {
    dynamicArray = NULL;
    size = 0;
}

// Copy constructor
DynamicStringArray::DynamicStringArray(const DynamicStringArray& other) {
    // Set size to match the other object
    size = other.size;
    
    // Handle empty array case
    if (size == 0) {
        dynamicArray = NULL;
        return;
    }
    
    // Allocate memory for the copy
    dynamicArray = new string[size];
    
    // Copy each string from the other object
    for (int i = 0; i < size; i++) {
        dynamicArray[i] = other.dynamicArray[i];
    }
}

// Destructor
DynamicStringArray::~DynamicStringArray() {
    // Free the dynamically allocated memory
    if (dynamicArray != NULL) {
        delete[] dynamicArray;
    }
}

// Assignment operator overload
DynamicStringArray& DynamicStringArray::operator=(const DynamicStringArray& other) {
    // Check for self-assignment
    if (this == &other) {
        return *this;
    }
    
    // Delete the current dynamic array
    if (dynamicArray != NULL) {
        delete[] dynamicArray;
    }
    
    // Set size to match the other object
    size = other.size;
    
    // Handle empty array case
    if (size == 0) {
        dynamicArray = NULL;
        return *this;
    }
    
    // Allocate memory for the copy
    dynamicArray = new string[size];
    
    // Copy each string from the other object
    for (int i = 0; i < size; i++) {
        dynamicArray[i] = other.dynamicArray[i];
    }
    
    return *this;
}

// Function to get the size of the array
int DynamicStringArray::getSize() {
    return size;
}

// Function to add a new entry to the array
void DynamicStringArray::addEntry(string newEntry) {
    // Create a new dynamic array one element larger than the current one
    string* newArray = new string[size + 1];
    
    // Copy all elements from the current array to the new array
    for (int i = 0; i < size; i++) {
        newArray[i] = dynamicArray[i];
    }
    
    // Add the new string to the end of the new array
    newArray[size] = newEntry;
    
    // Delete the old array
    if (dynamicArray != NULL) {
        delete[] dynamicArray;
    }
    
    // Point to the new array
    dynamicArray = newArray;
    
    // Increment the size
    size++;
}

// Function to delete an entry from the array
bool DynamicStringArray::deleteEntry(string entry) {
    // Search for the string in the array
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (dynamicArray[i] == entry) {
            index = i;
            break;
        }
    }
    
    // If not found, return false
    if (index == -1) {
        return false;
    }
    
    // Create a new dynamic array one element smaller than the current one
    string* newArray = NULL;
    if (size > 1) {
        newArray = new string[size - 1];
    }
    
    // Copy all elements except the one to be deleted
    for (int i = 0, j = 0; i < size; i++) {
        if (i != index) {
            newArray[j] = dynamicArray[i];
            j++;
        }
    }
    
    // Delete the old array
    delete[] dynamicArray;
    
    // Point to the new array
    dynamicArray = newArray;
    
    // Decrement the size
    size--;
    
    return true;
}

// Function to get an entry at a specific index
string DynamicStringArray::getEntry(int index) {
    // Check if index is within bounds
    if (index >= 0 && index < size) {
        return dynamicArray[index];
    }
    
    // Return NULL for out of bounds index
    return "";
}
// Main function for testing
int main() {
    // Create a DynamicStringArray
    DynamicStringArray names;
    
    // Add some names to the array
    names.addEntry("John");
    names.addEntry("Mary");
    names.addEntry("Bob");
    
    // Display the array
    cout << "Array size: " << names.getSize() << endl;
    cout << "Names in the array:" << endl;
    for (int i = 0; i < names.getSize(); i++) {
        cout << names.getEntry(i) << endl;
    }
    
    // Delete an entry
    cout << "\nDeleting 'Mary' from the array..." << endl;
    if (names.deleteEntry("Mary")) {
        cout << "Deleted successfully!" << endl;
    } else {
        cout << "Name not found!" << endl;
    }
    
    // Display the array after deletion
    cout << "\nArray size after deletion: " << names.getSize() << endl;
    cout << "Names in the array after deletion:" << endl;
    for (int i = 0; i < names.getSize(); i++) {
        cout << names.getEntry(i) << endl;
    }
    
    // Test copy constructor
    DynamicStringArray namesCopy(names);
    cout << "\nCopy of the array:" << endl;
    for (int i = 0; i < namesCopy.getSize(); i++) {
        cout << namesCopy.getEntry(i) << endl;
    }
    
    // Test assignment operator
    DynamicStringArray namesAssigned;
    namesAssigned = names;
    cout << "\nAssigned copy of the array:" << endl;
    for (int i = 0; i < namesAssigned.getSize(); i++) {
        cout << namesAssigned.getEntry(i) << endl;
    }
    
    return 0;
}