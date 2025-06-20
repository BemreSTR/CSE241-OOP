// One problem with dynamic arrays is that once the array is created using the new operator the size cannot be changed. For example, you might want to add or delete entries from the array similar to the behavior of a vector. This project asks you to create a class called DynamicStringArray that includes member functions that allow it to emulate the behavior of a vector of strings. The class should have:
// - A private member variable called dynamicArray that references a dynamic array of type string.
// - A private member variable called size that holds the number of entries in the array.
// - A default constructor that sets the dynamic array to NULL and sets size to 0.
// - A function that returns size.
// - A function named addEntry that takes a string as input. The function should create a new dynamic array one element larger than dynamicArray, copy all elements from dynamicArray into the new array, add the new string onto the end of the new array, increment size, delete the old dynamicArray, and then set dynamicArray to the new array. 
// - A function named deleteEntry that takes a string as input. The function should search dynamicArray for the string. If not found, return false. If found, create a new dynamic array one element smaller than dynamicArray. Copy all elements except the input string into the new array, delete dynamicArray, decrement size, and return true.
// - A function named getEntry that takes an integer as input and returns the string at that index in dynamicArray. Return NULL if the index is out of dynamicArray's bounds.
// - A copy constructor that makes a copy of the input object's dynamic array.
// - Overload the assignment operator so that the dynamic array is properly copied to the target object.
// - A destructor that frees up the memory allocated to the dynamic array.

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