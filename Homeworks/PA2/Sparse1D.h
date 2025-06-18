#ifndef SPARSE1D_H
#define SPARSE1D_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

/*
 * Class: Sparse1D
 * Description: Represents a one-dimensional sparse vector.
 * Only non-zero values are stored. Supports addition, subtraction, unary negation,
 * dot product calculation, insertion, and output to std::ostream.
 */
class Sparse1D {
public:
    // Make the Pair structure public so that other modules can use it.
    struct Pair {
        int index;
        double value;
    };

    // Default constructor.
    Sparse1D();

    // Constructor that takes a filename, reads the file, and populates the vector.
    explicit Sparse1D(const std::string &filename);

    // Copy constructor.
    Sparse1D(const Sparse1D &other);

    // Assignment operator.
    Sparse1D& operator=(const Sparse1D &other);

    // Binary addition operator.
    Sparse1D operator+(const Sparse1D &other) const;

    // Binary subtraction operator.
    Sparse1D operator-(const Sparse1D &other) const;

    // Unary minus operator: negates each element.
    Sparse1D operator-() const;

    // Dot product: returns the inner product of two Sparse1D objects.
    double dot(const Sparse1D &other) const;

    // Inserts a non-zero element at the given index (if value==0, it is ignored).
    void insert(int index, double value);

    // Output operator: sends the text representation to an output stream.
    friend std::ostream& operator<<(std::ostream &os, const Sparse1D &vec);

    // Public getter to access the underlying elements (needed by Sparse2D).
    const std::vector<Pair>& getElements() const;

private:
    // Container for non-zero elements. Kept sorted by index.
    std::vector<Pair> elements;

    // Maximum index value.
    int maxIndex;

    // Helper function to load data from a file.
    void loadFromFile(const std::string &filename);
};

#endif // SPARSE1D_H