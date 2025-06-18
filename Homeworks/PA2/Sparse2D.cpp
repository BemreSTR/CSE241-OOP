#include "Sparse2D.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <set>
#include <map>

/**
 * Default constructor.
 * Initializes an empty sparse matrix with maximum row and column indices set to 0.
 */
Sparse2D::Sparse2D() : maxRow(0), maxCol(0) {}

/**
 * Constructor that reads matrix data from a file.
 * Expected file format per line: "<row_index> <index>|<data> <index>|<data> ..."
 * For example: "3 3|24.6 4|5.5"
 * 
 * @param filename Path to the file containing sparse matrix data
 * @throws std::runtime_error if the file cannot be opened
 */
Sparse2D::Sparse2D(const std::string &filename) : maxRow(0), maxCol(0) {
    loadFromFile(filename);
}

/**
 * Copy constructor.
 * Creates a deep copy of another Sparse2D object.
 * 
 * @param other The Sparse2D object to copy from
 */
Sparse2D::Sparse2D(const Sparse2D &other)
    : row_indices(other.row_indices), rows(other.rows),
      maxRow(other.maxRow), maxCol(other.maxCol) {}

/**
 * Assignment operator.
 * Assigns the contents of another Sparse2D object to this one.
 * Implements self-assignment check for efficiency.
 * 
 * @param other The Sparse2D object to assign from
 * @return Reference to this object after assignment
 */
Sparse2D& Sparse2D::operator=(const Sparse2D &other) {
    if (this != &other) {
        row_indices = other.row_indices;
        rows = other.rows;
        maxRow = other.maxRow;
        maxCol = other.maxCol;
    }
    return *this;
}

/**
 * Binary addition operator: adds two sparse matrices.
 * Combines corresponding rows from both matrices using Sparse1D addition.
 * Handles rows that exist in either or both matrices appropriately.
 * Complexity: O(r log r) where r is the total number of rows in both matrices.
 * 
 * @param other The Sparse2D to add to this one
 * @return A new Sparse2D containing the sum
 */
Sparse2D Sparse2D::operator+(const Sparse2D &other) const {
    Sparse2D result;
    
    // Collect all unique row indices from both matrices
    std::set<int> allRows;
    for (int r : row_indices)
        allRows.insert(r);
    for (int r : other.row_indices)
        allRows.insert(r);
    
    // Process each row index
    for (int r : allRows) {
        Sparse1D newRow;
        bool foundThis = false, foundOther = false;
        Sparse1D rowA, rowB;
        
        // Find row r in current matrix
        for (size_t i = 0; i < row_indices.size(); i++) {
            if (row_indices[i] == r) {
                rowA = rows[i];
                foundThis = true;
                break;
            }
        }
        
        // Find row r in other matrix
        for (size_t i = 0; i < other.row_indices.size(); i++) {
            if (other.row_indices[i] == r) {
                rowB = other.rows[i];
                foundOther = true;
                break;
            }
        }
        
        // Determine the result row based on which matrices contain row r
        if (foundThis && foundOther)
            newRow = rowA + rowB;  // Row exists in both matrices
        else if (foundThis)
            newRow = rowA;  // Row only exists in this matrix
        else
            newRow = rowB;  // Row only exists in other matrix
        
        // Only add non-empty rows to the result
        std::ostringstream oss;
        oss << newRow;
        if (!oss.str().empty()) {
            result.row_indices.push_back(r);
            result.rows.push_back(newRow);
        }
    }
    
    // Update maximum dimensions of the result matrix
    result.maxRow = std::max(maxRow, other.maxRow);
    result.maxCol = std::max(maxCol, other.maxCol);
    return result;
}

/**
 * Binary subtraction operator: subtracts one sparse matrix from another.
 * Similar to addition but uses Sparse1D subtraction for corresponding rows.
 * Special handling for rows that exist in only one matrix.
 * Complexity: O(r log r) where r is the total number of rows in both matrices.
 * 
 * @param other The Sparse2D to subtract from this one
 * @return A new Sparse2D containing the difference
 */
Sparse2D Sparse2D::operator-(const Sparse2D &other) const {
    Sparse2D result;
    
    // Collect all unique row indices
    std::set<int> allRows;
    for (int r : row_indices)
        allRows.insert(r);
    for (int r : other.row_indices)
        allRows.insert(r);
    
    // Process each row index
    for (int r : allRows) {
        Sparse1D newRow;
        bool foundThis = false, foundOther = false;
        Sparse1D rowA, rowB;
        
        // Find row r in current matrix
        for (size_t i = 0; i < row_indices.size(); i++) {
            if (row_indices[i] == r) {
                rowA = rows[i];
                foundThis = true;
                break;
            }
        }
        
        // Find row r in other matrix
        for (size_t i = 0; i < other.row_indices.size(); i++) {
            if (other.row_indices[i] == r) {
                rowB = other.rows[i];
                foundOther = true;
                break;
            }
        }
        
        // Determine the result row based on which matrices contain row r
        if (foundThis && foundOther)
            newRow = rowA - rowB;  // Row exists in both matrices
        else if (foundThis)
            newRow = rowA;  // Row only exists in this matrix
        else
            newRow = -rowB;  // Row only exists in other matrix (negate it)
        
        // Only add non-empty rows to the result
        std::ostringstream oss;
        oss << newRow;
        if (!oss.str().empty()) {
            result.row_indices.push_back(r);
            result.rows.push_back(newRow);
        }
    }
    
    // Update maximum dimensions
    result.maxRow = std::max(maxRow, other.maxRow);
    result.maxCol = std::max(maxCol, other.maxCol);
    return result;
}

/**
 * Unary minus operator: negates every element in the matrix.
 * Creates a new matrix with all values negated row by row.
 * 
 * @return A new Sparse2D with negated values
 */
Sparse2D Sparse2D::operator-() const {
    Sparse2D result(*this);  // Create a copy first
    
    // Negate each row in the result
    for (size_t i = 0; i < result.rows.size(); i++) {
        result.rows[i] = -result.rows[i];
    }
    return result;
}

/**
 * Matrix multiplication operator (regular matrix multiplication).
 * For each row in this matrix and each column in the other matrix,
 * computes the dot product to form the resulting element.
 * Only stores non-zero results to maintain sparsity.
 * Complexity: O(r₁ × c₂) where r₁ is the number of rows in this matrix
 * and c₂ is the maximum column index in the other matrix.
 * 
 * @param other The Sparse2D to multiply with this one
 * @return A new Sparse2D containing the matrix product
 */
Sparse2D Sparse2D::operator*(const Sparse2D &other) const {
    Sparse2D result;
    
    // Generate a set of all possible column indices in the other matrix
    // For sparse matrices, this could be optimized to only include columns with data
    std::set<int> otherColumns;
    for (int col = 0; col <= other.maxCol; col++)
        otherColumns.insert(col);

    // Process each row in the current matrix
    for (size_t i = 0; i < row_indices.size(); i++) {
        int rIndex = row_indices[i];  // Current row index
        Sparse1D newRow;  // Will store the resulting row
        
        // For each column in the other matrix
        for (int col : otherColumns) {
            // Calculate dot product between current row and col-th column of other
            double val = rows[i].dot(other.column(col));
            
            // Only store non-zero values (using a small epsilon for floating-point comparison)
            if (std::fabs(val) > 1e-9)
                newRow.insert(col, val);
        }
        
        // Only add non-empty rows to the result
        std::ostringstream oss;
        oss << newRow;
        if (!oss.str().empty()) {
            result.row_indices.push_back(rIndex);
            result.rows.push_back(newRow);
        }
    }
    
    // Set dimensions of the result matrix
    result.maxRow = maxRow;
    result.maxCol = other.maxCol;
    return result;
}

/**
 * Transpose: returns a new Sparse2D that is the transpose of this matrix.
 * Converts rows to columns and columns to rows.
 * Uses a map to efficiently collect elements for each new row.
 * Complexity: O(n log n) where n is the total number of non-zero elements.
 * 
 * @return A new Sparse2D containing the transpose
 */
Sparse2D Sparse2D::transpose() const {
    Sparse2D transposed;
    
    // Use a map to collect new rows keyed by the column indices of the original matrix
    std::map<int, Sparse1D> tempRows;
    
    // Process each row in the current matrix
    for (size_t i = 0; i < row_indices.size(); i++) {
        int currentRow = row_indices[i];
        const auto &elems = rows[i].getElements();
        
        // For each element in the current row
        for (const auto &p : elems) {
            // In the transpose, the column index becomes the row index
            // and the current row index becomes the column index
            tempRows[p.index].insert(currentRow, p.value);
        }
    }
    
    // Build the transposed matrix from the temporary map
    for (const auto &entry : tempRows) {
        transposed.row_indices.push_back(entry.first);
        transposed.rows.push_back(entry.second);
    }
    
    // Swap max row and column indices for the transpose
    transposed.maxRow = maxCol;
    transposed.maxCol = maxRow;
    return transposed;
}

/**
 * Returns a reference to the row corresponding to the given row index.
 * Allows direct modification of a specific row in the matrix.
 * 
 * @param row_index The index of the row to retrieve
 * @return Reference to the Sparse1D object representing the requested row
 * @throws std::out_of_range if the row is not found in the matrix
 */
Sparse1D& Sparse2D::row(int row_index) {
    for (size_t i = 0; i < row_indices.size(); i++) {
        if (row_indices[i] == row_index)
            return rows[i];
    }
    throw std::out_of_range("Row index not found.");
}

/**
 * Returns a Sparse1D representing the specified column of the matrix.
 * Constructs a new Sparse1D by collecting elements from each row at the specified column index.
 * Note: This returns by value since columns are not stored directly in the matrix.
 * 
 * @param column_index The index of the column to retrieve
 * @return A new Sparse1D object representing the requested column
 */
Sparse1D Sparse2D::column(int column_index) const {
    Sparse1D col;
    
    // Go through each row
    for (size_t i = 0; i < row_indices.size(); i++) {
        const auto &elems = rows[i].getElements();
        
        // Find the element with the matching column index in this row
        for (const auto &p : elems) {
            if (p.index == column_index) {
                // In the column vector, the row index becomes the element index
                col.insert(row_indices[i], p.value);
                break;  // Move to next row after finding the element
            }
        }
    }
    return col;
}

/**
 * Stream insertion operator: outputs the matrix in the format:
 * <row_index> <index>|<data> <index>|<data> ...
 * Each row is on a new line, with the row index followed by the row data.
 * 
 * @param os The output stream to write to
 * @param matrix The Sparse2D object to output
 * @return Reference to the output stream
 */
std::ostream& operator<<(std::ostream &os, const Sparse2D &matrix) {
    for (size_t i = 0; i < matrix.rows.size(); i++) {
        os << matrix.row_indices[i] << " " << matrix.rows[i] << "\n";
    }
    return os;
}

/**
 * Helper function: loads matrix data from a file.
 * Expected file format per line: "<row_index> <index>|<data> <index>|<data> ..."
 * For example: "3 3|24.6 4|5.5"
 * 
 * @param filename Path to the file to load data from
 * @throws std::runtime_error if the file cannot be opened
 */
void Sparse2D::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open file: " + filename);
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // Skip empty lines
        
        std::istringstream iss(line);
        int rIndex;
        if (!(iss >> rIndex))  // Read the row index
            continue;  // Skip malformed lines
        
        Sparse1D row;
        std::string token;
        
        // Process each index|value token on the current line
        while (iss >> token) {
            std::istringstream tokenStream(token);
            int colIdx;
            double val;
            char delim;
            
            // Parse token in the format "index|value"
            if (tokenStream >> colIdx >> delim >> val) {
                row.insert(colIdx, val);  // Add element to the current row
                
                // Update maximum column index if necessary
                if (colIdx > maxCol)
                    maxCol = colIdx;
            }
        }
        
        // Add the row to the matrix
        row_indices.push_back(rIndex);
        rows.push_back(row);
        
        // Update maximum row index if necessary
        if (rIndex > maxRow)
            maxRow = rIndex;
    }
    file.close();
}