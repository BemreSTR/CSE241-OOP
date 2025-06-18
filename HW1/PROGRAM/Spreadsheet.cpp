#include "Spreadsheet.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <unordered_map>

// Convert a column index (e.g., 0, 1, 2) into an Excel-style column name (e.g., A, B, C)
std::string columnName(int colIndex) {
    std::string name;
    while (colIndex >= 0) {
        name = static_cast<char>('A' + colIndex % 26) + name;
        colIndex = (colIndex / 26) - 1;
    }
    return name;
}

// Update the content of a specific cell in the grid
void Spreadsheet::updateCellContent(int row, int col, const std::string& content) {
    if (row >= rows || col >= cols) { // Check if the cell position is valid
        std::cerr << "Error: Invalid cell location (" << row << ", " << col << ")." << std::endl;
        return;
    }

    grid[row][col].setValue(content); // Write the content into the cell
    display(10, 10, row, col); // Refresh the display with updated content
}

// Constructor to initialize the spreadsheet with a specific number of rows and columns
Spreadsheet::Spreadsheet(int rows, int cols) : rows(rows), cols(cols), horizontalOffset(0), verticalOffset(0) {
    grid.resize(rows, std::vector<Cell>(cols)); // Create the grid with specified dimensions
}

// Display the spreadsheet on the terminal, showing a specific visible portion
void Spreadsheet::display(int visibleRows, int visibleCols, int selectedRow, int selectedCol) {
    // Function to clear a terminal line
    auto clearLine = []() { std::cout << "\033[2K"; };

    // Highlight information about the selected cell
    std::string selectedCellName = columnName(selectedCol) + std::to_string(selectedRow + 1);
    const Cell& selectedCell = grid[selectedRow][selectedCol]; // Get the selected cell
    std::string cellType = selectedCell.getType(); // Get the type of the cell ("V", "L", or "F")
    std::string cellDisplayValue = selectedCell.getDisplayValue(); // Get the value to display in the cell
    std::string cellRawValue = selectedCell.value; // Get the raw content of the cell

    // Display the header row with selected cell information
    std::cout << "\033[1;1H"; // Move the cursor to the first line
    clearLine();
    std::cout << "\033[1;42m" << std::setw(20) << std::left << " Selected Cell Info " << "\033[0m" << std::endl;

    // Display selected cell details
    std::cout << "\033[2;1H"; // Move the cursor to the second line
    clearLine();
    std::cout << "\033[1;42m" << " " << selectedCellName << " (" << cellType << "): ";
    if (cellType == "F") { // If the cell contains a formula, show both the formula and the result
        std::cout << cellRawValue << " = " << cellDisplayValue;
    } else { // Otherwise, just show the content
        std::cout << cellDisplayValue;
    }
    std::cout << "\033[0m" << std::endl;

    // Display column headers for the visible range
    std::cout << "     ";
    for (int col = horizontalOffset; col < std::min(horizontalOffset + visibleCols, cols); ++col) {
        std::cout << "\033[44m\033[97m" << std::setw(10) << std::left << columnName(col) << "\033[0m";
    }
    std::cout << std::endl;

    // Display rows and their corresponding cells in the visible range
    for (int row = verticalOffset; row < std::min(verticalOffset + visibleRows, rows); ++row) {
        std::cout << "\033[44m\033[97m" << std::setw(5) << std::left << (row + 1) << "\033[0m";

        for (int col = horizontalOffset; col < std::min(horizontalOffset + visibleCols, cols); ++col) {
            const Cell& cell = grid[row][col];
            std::string cellDisplay = cell.getDisplayValue();
            if (cellDisplay.length() > 10) {
                cellDisplay = cellDisplay.substr(0, 10); // Truncate long cell content
            }

            if (row == selectedRow && col == selectedCol) { // Highlight the selected cell
                std::cout << "\033[42m\033[30m" << std::setw(10) << std::left << cellDisplay << "\033[0m";
            } else {
                std::cout << std::setw(10) << std::left << cellDisplay;
            }
        }
        std::cout << std::endl;
    }
}

// Evaluate a formula by parsing it and calculating its result
double Spreadsheet::evaluateFormula(const std::string& formula) {
    std::unordered_map<std::string, double> cellValues;

    // Collect all numeric values from the spreadsheet cells
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::string cellName = columnName(col) + std::to_string(row + 1);
            cellValues[cellName] = grid[row][col].numericValue; // Store numeric values
        }
    }

    // Parse the formula and return the calculated result
    return parser.parse(formula, cellValues);
}

// Update the dependencies of a cell when its formula changes
void Spreadsheet::updateDependencies(const std::string& cellName, const std::string& formula) {
    // Clear old dependencies
    if (dependencyGraph.find(cellName) != dependencyGraph.end()) {
        for (const auto& dep : dependencyGraph[cellName]) {
            reverseDependencyGraph[dep].erase(cellName);
        }
        dependencyGraph[cellName].clear();
    }

    // If the formula is empty or not valid, skip updating dependencies
    if (formula.empty() || formula[0] != '=') return;

    // Extract new dependencies from the formula
    std::set<std::string> dependencies = parser.extractCellReferences(formula);
    for (const auto& dep : dependencies) {
        dependencyGraph[cellName].insert(dep); // Add to direct dependencies
        reverseDependencyGraph[dep].insert(cellName); // Add to reverse dependencies
    }
}

// Detect if there is a circular dependency in the spreadsheet
bool Spreadsheet::detectCycle(const std::string& startCell, const std::string& currentCell, 
                               std::set<std::string>& visited) {
    if (visited.find(currentCell) != visited.end()) {
        return currentCell == startCell; // Circular reference detected
    }

    visited.insert(currentCell);
    
    // Check all dependencies of the current cell for cycles
    for (const auto& dep : dependencyGraph[currentCell]) {
        if (detectCycle(startCell, dep, visited)) {
            return true;
        }
    }

    visited.erase(currentCell);
    return false;
}


void Spreadsheet::recalculateDependents(const std::string& cellName) {
    std::stack<std::string> toCalculate;
    std::set<std::string> processed;

    // Add the initial cell's dependents to the stack
    if (reverseDependencyGraph.find(cellName) != reverseDependencyGraph.end()) {
        for (const auto& dep : reverseDependencyGraph[cellName]) {
            toCalculate.push(dep);
        }
    }

    // Recalculate all dependent cells
    while (!toCalculate.empty()) {
        std::string current = toCalculate.top();
        toCalculate.pop();

        // Skip already processed cells
        if (processed.find(current) != processed.end()) continue;
        processed.insert(current);

        // Find the row and column for the current cell
        int row, col;
        getCellLocation(current, row, col);

        // Recalculate the formula in the current cell if it is a formula
        if (grid[row][col].isFormula) {
            try {
                grid[row][col].numericValue = evaluateFormula(grid[row][col].value);
            } catch (const std::exception& e) {
                std::cerr << "Error recalculating " << current << ": " << e.what() << std::endl;
            }
        }

        // Add further dependents of the current cell to the stack
        if (reverseDependencyGraph.find(current) != reverseDependencyGraph.end()) {
            for (const auto& dep : reverseDependencyGraph[current]) {
                toCalculate.push(dep);
            }
        }
    }
}

void Spreadsheet::setCell(int row, int col, const std::string& value) {
    // Check if the cell coordinates are within bounds
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Invalid cell location.");
    }

    std::string cellName = getCellName(row, col);

    // Check for circular references if the value is a formula
    if (value[0] == '=') {
        std::set<std::string> visited;
        updateDependencies(cellName, value);
        if (detectCycle(cellName, cellName, visited)) {
            throw std::runtime_error("Circular reference detected!");
            return;
        }
    }

    // Update the cell value
    grid[row][col].setValue(value);

    // If it's a formula, evaluate it and store the result
    if (grid[row][col].isFormula) {
        try {
            grid[row][col].numericValue = evaluateFormula(value);
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating formula: " << e.what() << std::endl;
            grid[row][col].numericValue = 0; // Set default value on error
        }
    }

    // Recalculate all dependent cells
    recalculateDependents(cellName);

    // Refresh the display
    display(10, 10, row, col);
}

std::string Spreadsheet::getCellName(int row, int col) const {
    // Convert row and column indices to a cell name (e.g., A1, B2)
    return columnName(col) + std::to_string(row + 1);
}

void Spreadsheet::getCellLocation(const std::string& cellName, int& row, int& col) const {
    // Validate the input cell name format
    if (cellName.empty() || !std::isalpha(cellName[0])) {
        throw std::invalid_argument("Invalid cell name format: " + cellName);
    }

    col = 0;
    size_t i = 0;

    // Calculate the column index from the cell name
    while (i < cellName.size() && std::isalpha(cellName[i])) {
        col *= 26;
        col += (std::toupper(cellName[i]) - 'A' + 1);
        ++i;
    }
    col -= 1;

    // Calculate the row index from the cell name
    if (i >= cellName.size() || !std::isdigit(cellName[i])) {
        throw std::invalid_argument("Invalid cell name format: " + cellName);
    }
    row = std::stoi(cellName.substr(i)) - 1;
}

void Spreadsheet::saveSpreadsheet(const std::string& filename) {
    FileManager fileManager;
    std::vector<std::vector<std::string>> data(rows, std::vector<std::string>(cols));

    // Prepare the data for saving
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            data[r][c] = grid[r][c].value; // Store the raw cell value
        }
    }

    // Save the data to the specified file
    if (!fileManager.saveToFile(filename, data)) {
        std::cerr << "Error: Failed to save spreadsheet to " << filename << std::endl;
    }
}


void Spreadsheet::loadSpreadsheet(const std::string& filename) {
    FileManager fileManager;
    std::vector<std::vector<std::string>> data;

    // Load data from the file
    if (!fileManager.loadFromFile(filename, data)) {
        std::cerr << "Error: Failed to load spreadsheet from " << filename << std::endl;
        return;
    }

    // Adjust grid size dynamically based on the loaded data
    int newRows = data.size();
    int newCols = newRows > 0 ? data[0].size() : 0;

    resizeGrid(newRows, newCols);

    // Populate the grid with loaded data
    for (int r = 0; r < newRows; ++r) {
        for (int c = 0; c < newCols; ++c) {
            if (c < static_cast<int>(data[r].size())) {
                grid[r][c].setValue(data[r][c]);
            }
        }
    }

    // Rebuild dependencies for all formulas
    dependencyGraph.clear(); // Clear the old dependencies
    reverseDependencyGraph.clear();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].isFormula) {
                std::string cellName = getCellName(r, c);
                updateDependencies(cellName, grid[r][c].value);
            }
        }
    }

    // Recalculate all formulas
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].isFormula) {
                try {
                    grid[r][c].numericValue = evaluateFormula(grid[r][c].value);
                } catch (const std::exception& e) {
                    std::cerr << "Error evaluating formula in " << getCellName(r, c) << ": " << e.what() << std::endl;
                }
            }
        }
    }

    // Reset the display offsets to the top-left corner
    horizontalOffset = 0;
    verticalOffset = 0;

    std::cout << "Spreadsheet loaded" << std::endl;
}

void Spreadsheet::resizeGrid(int newRows, int newCols) {
    // Increase the number of rows if needed
    if (newRows > rows) {
        grid.resize(newRows, std::vector<Cell>(cols)); // Allocate space for new rows
    }

    // Increase the number of columns if needed
    if (newCols > cols) {
        for (auto& row : grid) {
            row.resize(newCols); // Expand existing rows to accommodate new columns
        }
    }

    // Update the dimensions of the grid
    rows = newRows;
    cols = newCols;
}

void Spreadsheet::importFromData(const std::vector<std::vector<std::string>>& data) {
    int newRows = data.size();
    int newCols = data.empty() ? 0 : data[0].size();
    resizeGrid(newRows, newCols);

    // Populate the grid with data from the input
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            grid[i][j].setValue(data[i][j]);
        }
    }
}

void Spreadsheet::clear() {
    // Reset all cells in the grid to an empty state
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.setValue(""); // Clear the cell value
        }
    }
}

std::vector<std::vector<std::string>> Spreadsheet::exportToData() const {
    std::vector<std::vector<std::string>> data;

    // Extract raw cell values into a 2D vector for export
    for (const auto& row : grid) {
        std::vector<std::string> rowData;
        for (const auto& cell : row) {
            rowData.push_back(cell.value); // Use the raw cell value
        }
        data.push_back(rowData);
    }

    return data;
}

void Spreadsheet::autoExpandGrid(int currentRow, int currentCol) {
    bool expanded = false;

    // Automatically expand rows if the last row is reached
    if (currentRow >= rows - 1) {
        int newRows = rows + 10; // Add 10 more rows
        resizeGrid(newRows, cols);
        expanded = true;
    }

    // Automatically expand columns if the last column is reached
    if (currentCol >= cols - 1) {
        int newCols = cols + 10; // Add 10 more columns
        resizeGrid(rows, newCols);
        expanded = true;
    }

    if (expanded) {
        std::cout << "Grid expanded to " << rows << " rows and " << cols << " columns." << std::endl;
    }
}

void Spreadsheet::evaluateAllFormulas() {
    // Iterate through all cells in the grid
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].isFormula) { // Check if the cell contains a formula
                try {
                    // Evaluate the formula and store the result
                    grid[r][c].numericValue = evaluateFormula(grid[r][c].value);
                } catch (const std::exception& e) {
                    std::cerr << "Error evaluating formula in cell " << getCellName(r, c) << ": " << e.what() << std::endl;
                    grid[r][c].numericValue = 0; // Default value in case of an error
                }
            }
        }
    }
}
