#include "Cell.h"

// Constructor: Initializes a cell with default values
Cell::Cell() : value(""), numericValue(0), isFormula(false) {}

void Cell::setValue(const std::string& val) {
    value = val;
    isFormula = (!val.empty() && val[0] == '='); // Check if the value is a formula (starts with '=')

    // If the value is not a formula, attempt to convert it to a numeric value
    if (!isFormula) {
        try {
            numericValue = std::stod(val); // Convert to a double if possible
        } catch (...) {
            numericValue = 0; // Set to zero if conversion fails
        }
    } else {
        numericValue = 0; // For formulas, the calculation is done in the Spreadsheet class
    }
}

std::string Cell::getDisplayValue() const {
    // If the cell contains a formula, return numericValue as a string for display purposes
    return isFormula ? std::to_string(numericValue) : value;
}

std::string Cell::getType() const {
    if (isFormula) return "F"; // "F" for Formula

    // Check if the value is numeric, including negative numbers
    try {
        std::stod(value); // Attempt to convert the value to a number
        return "V"; // "V" for Value (numeric)
    } catch (...) {
        return "L"; // "L" for Label (non-numeric text)
    }
}
