// FormulaParser.cpp
#include "FormulaParser.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <numeric>

// Helper function: Extracts the column letter from a cell reference
char FormulaParser::getColumnLetter(const std::string& cellRef) {
    return cellRef[0];
}

// Helper function: Extracts the row number from a cell reference
int FormulaParser::getRowNumber(const std::string& cellRef) {
    return std::stoi(cellRef.substr(1));
}

// Adds spaces around mathematical operators in a formula to facilitate parsing
std::string FormulaParser::addSpacesAroundOperators(const std::string& formula) {
    std::string processed;
    for (char ch : formula) {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            processed += " ";
            processed += ch;
            processed += " ";
        } else {
            processed += ch;
        }
    }
    return processed;
}

// Parses a formula and evaluates it using the provided cell values
double FormulaParser::parse(const std::string& formula, const std::unordered_map<std::string, double>& cellValues) {
    if (formula.empty() || formula[0] != '=') {
        throw std::invalid_argument("Invalid formula");
    }
    std::string expression = formula.substr(1); // Remove '=' from the formula
    expression = addSpacesAroundOperators(expression);
    return evaluateExpression(expression, cellValues);
}

// Evaluates an expression and computes its value based on the given cell values
double FormulaParser::evaluateExpression(const std::string& expression, const std::unordered_map<std::string, double>& cellValues) {
    std::istringstream stream(expression);
    double result = 0;
    char op = '+'; // Default operator is addition
    std::string token;

    while (stream >> token) {
        double value;
        if (std::isalpha(token[0]) && token.find("..") != std::string::npos) {
            // Handle functions like SUM, AVER, etc.
            auto funcEnd = token.find('(');
            std::string funcName = token.substr(0, funcEnd);
            std::string range = token.substr(funcEnd + 1, token.size() - funcEnd - 2);
            value = evaluateFunction(funcName, range, cellValues);
        } else if (std::isalpha(token[0])) {
            // Cell reference (e.g., A1)
            value = getCellValue(token, cellValues);
        } else {
            // Numeric value
            value = std::stod(token);
        }

        // Perform the operation
        switch (op) {
            case '+': result += value; break;
            case '-': result -= value; break;
            case '*': result *= value; break;
            case '/': 
                if (value == 0) throw std::runtime_error("Division by zero");
                result /= value; 
                break;
            default: throw std::invalid_argument("Invalid operator");
        }

        stream >> op; // Read the next operator
    }

    return result;
}

// Retrieves the numeric value of a cell based on its reference
double FormulaParser::getCellValue(const std::string& cellRef, const std::unordered_map<std::string, double>& cellValues) {
    auto it = cellValues.find(cellRef);
    if (it == cellValues.end()) {
        throw std::runtime_error("Cell reference not found: " + cellRef);
    }
    return it->second;
}

// Parses a range (e.g., A1..B2) and returns all the cell references within the range
std::vector<std::string> FormulaParser::parseRange(const std::string& range) {
    std::vector<std::string> cells;

    auto delimiterPos = range.find("..");
    if (delimiterPos == std::string::npos) {
        throw std::invalid_argument("Invalid range: " + range);
    }

    std::string startCell = range.substr(0, delimiterPos);
    std::string endCell = range.substr(delimiterPos + 2);

    char startCol = getColumnLetter(startCell);
    int startRow = getRowNumber(startCell);
    char endCol = getColumnLetter(endCell);
    int endRow = getRowNumber(endCell);

    // Generate all cells in the range
    for (char col = startCol; col <= endCol; ++col) {
        for (int row = startRow; row <= endRow; ++row) {
            cells.push_back(std::string(1, col) + std::to_string(row));
        }
    }

    return cells;
}

// Evaluates a function (e.g., SUM, AVER, etc.) over a given range
double FormulaParser::evaluateFunction(const std::string& funcName, 
                                       const std::string& range, 
                                       const std::unordered_map<std::string, double>& cellValues) {
    auto cells = parseRange(range);
    std::vector<double> values;
    values.reserve(cells.size());

    for (const auto& cell : cells) {
        values.push_back(getCellValue(cell, cellValues));
    }

    if (funcName == "SUM") {
        return std::accumulate(values.begin(), values.end(), 0.0); // Sum of all values
    } else if (funcName == "AVER") {
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size(); // Average
    } else if (funcName == "STDDEV") {
        double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        double variance = 0.0;
        for (const auto& v : values) {
            variance += (v - mean) * (v - mean);
        }
        variance /= values.size();
        return std::sqrt(variance); // Standard deviation
    } else if (funcName == "MAX") {
        return *std::max_element(values.begin(), values.end()); // Maximum value
    } else if (funcName == "MIN") {
        return *std::min_element(values.begin(), values.end()); // Minimum value
    } else {
        throw std::invalid_argument("Unsupported function: " + funcName);
    }
}

// Extracts all cell references from a formula (e.g., A1, B2, etc.)
std::set<std::string> FormulaParser::extractCellReferences(const std::string& formula) {
    std::set<std::string> references;

    // If the formula does not start with '=', return an empty set
    if (formula.empty() || formula[0] != '=') {
        return references;
    }

    std::string current;
    bool inFunction = false;

    for (size_t i = 1; i < formula.length(); ++i) {
        char c = formula[i];

        if (c == '(') {
            inFunction = true; // Inside a function
            continue;
        } else if (c == ')') {
            inFunction = false; // End of a function
            continue;
        }

        if (std::isalpha(c)) {
            current = std::string(1, c);
            while (i + 1 < formula.length() && std::isdigit(formula[i + 1])) {
                current += formula[++i];
            }

            if (i + 2 < formula.length() && formula[i + 1] == '.' && formula[i + 2] == '.') {
                // Range starting point
                std::string startCell = current;
                i += 2; // Skip ".."

                // Find the end of the range
                std::string endCell;
                if (i + 1 < formula.length() && std::isalpha(formula[i + 1])) {
                    endCell += formula[++i];
                    while (i + 1 < formula.length() && std::isdigit(formula[i + 1])) {
                        endCell += formula[++i];
                    }

                    // Add all cells in the range
                    char startCol = getColumnLetter(startCell);
                    int startRow = getRowNumber(startCell);
                    char endCol = getColumnLetter(endCell);
                    int endRow = getRowNumber(endCell);

                    for (char col = startCol; col <= endCol; ++col) {
                        for (int row = startRow; row <= endRow; ++row) {
                            references.insert(std::string(1, col) + std::to_string(row));
                        }
                    }
                }
            } else {
                // Single cell reference
                references.insert(current);
            }
        }
    }

    return references;
}
