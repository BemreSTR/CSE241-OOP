#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Cell.h"
#include "FormulaParser.h"
#include "FileManager.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <stack>

class Spreadsheet {
private:
    std::vector<std::vector<Cell>> grid; // The grid representing cells of the spreadsheet
    std::unordered_map<std::string, std::set<std::string>> dependencyGraph; // Tracks dependencies between cells
    std::unordered_map<std::string, std::set<std::string>> reverseDependencyGraph; // Tracks reverse dependencies

    int rows, cols; // Number of rows and columns in the spreadsheet
    FormulaParser parser; // Utility to parse and evaluate formulas

    // Private helper methods
    void updateDependencies(const std::string& cellName, const std::string& formula); // Updates dependencies for a formula
    void recalculateDependents(const std::string& cellName); // Recalculates values of dependent cells
    bool detectCycle(const std::string& startCell, const std::string& currentCell, std::set<std::string>& visited); // Checks for circular dependencies
    std::string getCellName(int row, int col) const; // Converts a row and column index to a cell name (e.g., A1)
    void getCellLocation(const std::string& cellName, int& row, int& col) const; // Converts a cell name (e.g., A1) to row and column indices

public:
    int horizontalOffset; // Horizontal scrolling offset for visible cells
    int verticalOffset; // Vertical scrolling offset for visible cells

    Spreadsheet(int rows, int cols); // Constructor to initialize a spreadsheet with given rows and columns
    void display(int visibleRows, int visibleCols, int selectedRow, int selectedCol); // Displays the spreadsheet within the visible range
    void setCell(int row, int col, const std::string& value); // Sets the value of a cell and recalculates its dependencies
    double evaluateFormula(const std::string& formula); // Evaluates a formula string and returns the result
    void updateCellContent(int row, int col, const std::string& content); // Updates a cell's content and refreshes the display
    void evaluateAllFormulas(); // Recalculates all formulas in the spreadsheet

    // File operations
    std::vector<std::vector<std::string>> exportToData() const; // Exports the spreadsheet data to a 2D vector
    void saveSpreadsheet(const std::string& filename); // Saves the spreadsheet to a file
    void loadSpreadsheet(const std::string& filename); // Loads spreadsheet data from a file
    void importFromData(const std::vector<std::vector<std::string>>& data); // Imports data into the spreadsheet
    void clear(); // Clears all cells in the spreadsheet
    void resizeGrid(int newRows, int newCols); // Dynamically resizes the grid to accommodate new dimensions
    int getRows() const { return rows; } // Returns the total number of rows in the spreadsheet
    int getCols() const { return cols; } // Returns the total number of columns in the spreadsheet
    void autoExpandGrid(int currentRow, int currentCol); // Automatically expands the grid when limits are reached
};

// Converts a column index to its corresponding column name (e.g., 0 -> A, 1 -> B)
std::string columnName(int colIndex);

#endif
