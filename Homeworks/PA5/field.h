#pragma once // Include guard to prevent multiple inclusions
#include <string> // For string manipulation
#include <vector> // For dynamic arrays
#include <sstream> // For string stream operations
using namespace std; // Use the standard namespace

// Enum to represent the field type (single or multi)
enum class fieldT { single, multi };
// Enum to represent the data type (string, int, double, bool)
enum class dataT { S, I, D, B }; // string, int, double, bool

// Struct to represent the format of a field
struct fieldformat {
    string name; // Name of the field
    dataT type; // Data type of the field
    fieldT sm; // Single or multi
};

// Base class for all field types
class FieldBase {
public:
    // Virtual method to get the field type
    virtual fieldT gettype() const = 0;
    // Virtual method to convert the field to a string
    virtual string tostring() const = 0;
    // Virtual destructor
    virtual ~FieldBase() {}
};

// Template class for single fields
template<class T>
class FieldSingle : public FieldBase {
public:
    // Constructor
    FieldSingle(T data) : data(data) {}
    // Method to get the data
    T getdata() const { return data; }
    // Method to get the field type
    fieldT gettype() const override { return fieldT::single; }
    // Method to convert the field to a string
    string tostring() const override {
        ostringstream oss; // Create a string stream
        oss << data; // Add the data to the string stream
        return oss.str(); // Return the string
    }
private:
    T data; // Data
};

// Template class for multi fields
template<class T>
class FieldMulti : public FieldBase {
public:
    // Constructor
    FieldMulti(vector<T> data) : data(data) {}
    // Method to get the data at a specific index
    T getdata(int index) const { return data[index]; }
    // Method to get the size of the data
    int getsize() const { return data.size(); }
    // Method to get the field type
    fieldT gettype() const override { return fieldT::multi; }
    // Method to convert the field to a string
    string tostring() const override {
        ostringstream oss; // Create a string stream
        for (size_t i = 0; i < data.size(); ++i) { // Iterate over each data element
            if (i) oss << ":"; // Add a ':' separator if it's not the first element
            oss << data[i]; // Add the data element to the string stream
        }
        return oss.str(); // Return the string
    }
private:
    vector<T> data; // Data
};