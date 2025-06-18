#pragma once // Include guard to prevent multiple inclusions
#include <vector> // For dynamic arrays
#include <string> // For string manipulation
#include <sstream> // For string stream operations
#include "field.h" // Include the field header
using namespace std; // Use the standard namespace

// Class definition for an entry
class entry {
public:
    vector<shared_ptr<FieldBase>> fields; // Vector to store the fields of the entry
    
    // Method to get the string representation of the first field
    string first_field_str() const {
        return fields[0]->tostring(); // Return the string representation of the first field
    }
    
    // Method to convert the entry to a string
    string tostring(const vector<fieldformat>& formats) const {
        ostringstream oss; // Create a string stream
        for (size_t i = 0; i < fields.size(); ++i) { // Iterate over each field
            if (i) oss << "|"; // Add a '|' separator if it's not the first field
            
            // Check if the current field is a boolean type
            if (formats[i].type == dataT::B) {
                // Convert boolean value to "true" or "false" string representation
                string bool_value = fields[i]->tostring();
                oss << (bool_value == "1" || bool_value == "true" ? "true" : "false");
            } else {
                // For non-boolean types, use the default string representation
                oss << fields[i]->tostring(); // Add the string representation of the field
            }
        }
        return oss.str(); // Return the string
    }
};