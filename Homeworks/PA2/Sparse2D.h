#ifndef SPARSE2D_H
#define SPARSE2D_H

#include <iostream>
#include <vector>
#include <string>
#include "Sparse1D.h"
#include <stdexcept>
#include <map>
#include <set>

/*
 * Class: Sparse2D
 * Description: Represents a two-dimensional sparse matrix.
 * Uses the Sparse1D class to store non-zero elements for each row.
 * Supports matrix addition, subtraction, unary negation, multiplication,
 * transposition, and access to specific rows and columns.
 */
class Sparse2D {
public:
    // Default constructor.
    Sparse2D();

    // Constructor that reads matrix data from a file.
    explicit Sparse2D(const std::string &filename);

    // Copy constructor.
    Sparse2D(const Sparse2D &other);

    // Assignment operator.
    Sparse2D& operator=(const Sparse2D &other);

    // Binary addition operator.
    Sparse2D operator+(const Sparse2D &other) const;

    // Binary subtraction operator.
    Sparse2D operator-(const Sparse2D &other) const;

    // Unary minus operator: negates every element in the matrix.
    Sparse2D operator-() const;

    // Matrix multiplication operator.
    Sparse2D operator*(const Sparse2D &other) const;

    // Returns the transpose of the matrix.
    Sparse2D transpose() const;

    // Returns a reference to the Sparse1D corresponding to a specific row.
    // If the row does not exist, throws std::out_of_range.
    Sparse1D& row(int row_index);

    // Returns a Sparse1D representing a specific column.
    Sparse1D column(int column_index) const;

    // Output operator: sends the matrix contents to an output stream in the specified format.
    friend std::ostream& operator<<(std::ostream &os, const Sparse2D &matrix);

    // Get the maximum row and column indices.
    int getMaxRow() const { return maxRow; }
    int getMaxCol() const { return maxCol; }

private:
    // Contains the row indices for rows with non-zero data.
    std::vector<int> row_indices;

    // Stores each row (as a Sparse1D object).
    std::vector<Sparse1D> rows;

    // Maximum row and column indices.
    int maxRow;
    int maxCol;

    // Helper function: loads matrix data from a file.
    // Expected file format per line:
    // <row_index> <index>|<data> <index>|<data> ...
    void loadFromFile(const std::string &filename);
};

#endif // SPARSE2D_H