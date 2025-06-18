#include "Sparse1D.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
 * Default constructor.
 * Initializes an empty sparse vector with maximum index set to 0.
 */
Sparse1D::Sparse1D() : maxIndex(0) {}

/**
 * Constructor that reads data from a file.
 * Expected file format: each line contains entries in the format "index|value" separated by whitespace
 * For example: "4|23.8 7|10.7 10|34"
 * 
 * @param filename Path to the file containing sparse vector data
 * @throws std::runtime_error if the file cannot be opened
 */
Sparse1D::Sparse1D(const std::string &filename) : maxIndex(0) {
    loadFromFile(filename);
}

/**
 * Copy constructor.
 * Creates a deep copy of another Sparse1D object.
 * 
 * @param other The Sparse1D object to copy from
 */
Sparse1D::Sparse1D(const Sparse1D &other)
    : elements(other.elements), maxIndex(other.maxIndex) {}

/**
 * Assignment operator.
 * Assigns the contents of another Sparse1D object to this one.
 * Implements self-assignment check for efficiency.
 * 
 * @param other The Sparse1D object to assign from
 * @return Reference to this object after assignment
 */
Sparse1D& Sparse1D::operator=(const Sparse1D &other) {
    if (this != &other) {
        elements = other.elements;
        maxIndex = other.maxIndex;
    }
    return *this;
}

/**
 * Insertion function: adds or updates an element at the specified index.
 * Only stores non-zero values to maintain sparsity.
 * Maintains the sorted order of elements by index for efficient operations.
 * 
 * @param index The index at which to insert the value (must be non-negative)
 * @param value The value to insert (if value is effectively zero, no insertion occurs)
 */
void Sparse1D::insert(int index, double value) {
    if (std::fabs(value) < 1e-9) // Ignore values that are effectively zero (prevents storing zeros)
        return;
    
    // Use binary search to find the insertion point in the sorted vector
    auto it = std::lower_bound(elements.begin(), elements.end(), index,
        [](const Pair &p, int idx) { return p.index < idx; });
    
    if (it != elements.end() && it->index == index) {
        // Update existing value if the index already exists
        it->value = value;
    } else {
        // Insert new element at the correct position to maintain sorted order
        elements.insert(it, {index, value});
    }
    
    // Update the maximum index if necessary
    if (index > maxIndex) {
        maxIndex = index;
    }
}

/**
 * Binary addition operator: adds two sparse vectors.
 * Efficiently merges two sorted vectors by traversing both simultaneously.
 * Complexity: O(n+m) where n and m are the number of non-zero elements.
 * 
 * @param other The Sparse1D to add to this one
 * @return A new Sparse1D containing the sum
 */
Sparse1D Sparse1D::operator+(const Sparse1D &other) const {
    Sparse1D result;
    size_t i = 0, j = 0;
    
    // Merge two sorted sequences of elements
    while (i < elements.size() || j < other.elements.size()) {
        if (i < elements.size() && j < other.elements.size()) {
            if (elements[i].index == other.elements[j].index) {
                // Same index - add values
                double sum = elements[i].value + other.elements[j].value;
                if (std::fabs(sum) > 1e-9)  // Only store non-zero results
                    result.insert(elements[i].index, sum);
                ++i;
                ++j;
            } else if (elements[i].index < other.elements[j].index) {
                // Index from first vector comes first - copy it
                result.insert(elements[i].index, elements[i].value);
                ++i;
            } else {
                // Index from second vector comes first - copy it
                result.insert(other.elements[j].index, other.elements[j].value);
                ++j;
            }
        } else if (i < elements.size()) {
            // Remaining elements from first vector
            result.insert(elements[i].index, elements[i].value);
            ++i;
        } else {
            // Remaining elements from second vector
            result.insert(other.elements[j].index, other.elements[j].value);
            ++j;
        }
    }
    return result;
}

/**
 * Binary subtraction operator: subtracts one sparse vector from another.
 * Uses a similar efficient merging algorithm as addition but subtracts values instead.
 * Complexity: O(n+m) where n and m are the number of non-zero elements.
 * 
 * @param other The Sparse1D to subtract from this one
 * @return A new Sparse1D containing the difference
 */
Sparse1D Sparse1D::operator-(const Sparse1D &other) const {
    Sparse1D result;
    size_t i = 0, j = 0;
    
    while (i < elements.size() || j < other.elements.size()) {
        if (i < elements.size() && j < other.elements.size()) {
            if (elements[i].index == other.elements[j].index) {
                // Same index - subtract values
                double diff = elements[i].value - other.elements[j].value;
                if (std::fabs(diff) > 1e-9)  // Only store non-zero results
                    result.insert(elements[i].index, diff);
                ++i;
                ++j;
            } else if (elements[i].index < other.elements[j].index) {
                // Index from first vector comes first - copy it
                result.insert(elements[i].index, elements[i].value);
                ++i;
            } else {
                // Index from second vector comes first - add its negation
                result.insert(other.elements[j].index, -other.elements[j].value);
                ++j;
            }
        } else if (i < elements.size()) {
            // Remaining elements from first vector
            result.insert(elements[i].index, elements[i].value);
            ++i;
        } else {
            // Remaining elements from second vector (negated)
            result.insert(other.elements[j].index, -other.elements[j].value);
            ++j;
        }
    }
    return result;
}

/**
 * Unary minus operator: negates every element in the vector.
 * Creates a new vector with all values negated.
 * 
 * @return A new Sparse1D with negated values
 */
Sparse1D Sparse1D::operator-() const {
    Sparse1D result;
    for (const auto &p : elements) {
        result.insert(p.index, -p.value);
    }
    return result;
}

/**
 * Dot product method: computes the inner product of two sparse vectors.
 * Optimized for sparse vectors by only multiplying matching indices.
 * Complexity: O(min(n,m)) where n and m are the number of non-zero elements.
 * 
 * @param other The Sparse1D to compute dot product with
 * @return The scalar dot product result
 */
double Sparse1D::dot(const Sparse1D &other) const {
    double resultVal = 0.0;
    size_t i = 0, j = 0;
    
    // Only multiply elements with matching indices (others multiply by zero)
    while (i < elements.size() && j < other.elements.size()) {
        if (elements[i].index == other.elements[j].index) {
            resultVal += elements[i].value * other.elements[j].value;
            ++i;
            ++j;
        } else if (elements[i].index < other.elements[j].index) {
            ++i;
        } else {
            ++j;
        }
    }
    return resultVal;
}

/**
 * Stream insertion operator: outputs the vector in the format:
 * <index>|<data> <index>|<data> ... (indices in ascending order)
 * 
 * @param os The output stream to write to
 * @param vec The Sparse1D object to output
 * @return Reference to the output stream
 */
std::ostream& operator<<(std::ostream &os, const Sparse1D &vec) {
    for (std::size_t i = 0; i < vec.elements.size(); ++i) {
        os << vec.elements[i].index << "|" << vec.elements[i].value;
        if (i != vec.elements.size() - 1)
            os << " ";
    }
    return os;
}

/**
 * Public getter to access the underlying elements.
 * Used by Sparse2D class to access the elements directly.
 * 
 * @return Constant reference to the vector of element pairs
 */
const std::vector<Sparse1D::Pair>& Sparse1D::getElements() const {
    return elements;
}

/**
 * Helper function: loads vector data from a file.
 * Expected file format: lines containing "index|value" tokens separated by whitespace
 * 
 * @param filename Path to the file to load data from
 * @throws std::runtime_error if the file cannot be opened
 */
void Sparse1D::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open file: " + filename);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // Skip empty lines
        
        std::istringstream iss(line);
        std::string token;
        
        // Process each token on the current line
        while (iss >> token) {
            std::istringstream tokenStream(token);
            int idx;
            double val;
            char delim;
            
            // Parse the token in the format "index|value"
            if (tokenStream >> idx >> delim >> val) {
                insert(idx, val);  // Insert the parsed value
            }
        }
    }
    file.close();
}