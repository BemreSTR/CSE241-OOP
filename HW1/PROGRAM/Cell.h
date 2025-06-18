#ifndef CELL_H
#define CELL_H

#include <string>

// Represents a single cell in the spreadsheet
class Cell {
public:
    std::string value; // Raw cell value (text or formula)
    double numericValue; // Numeric interpretation of the cell's value (if applicable)
    bool isFormula; // Indicates if the cell contains a formula

    // Default constructor: Initializes cell with empty values
    Cell();

    // Sets the value of the cell and determines its type
    void setValue(const std::string& val);

    // Returns the value to display in the spreadsheet
    std::string getDisplayValue() const;

    // Determines and returns the type of the cell ("V" for Value, "L" for Label, "F" for Formula)
    std::string getType() const;
};

#endif // CELL_H
