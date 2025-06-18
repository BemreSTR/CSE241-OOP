#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

class FormulaParser {
public:
    // Parses a formula string and evaluates its value using the provided cell values
    double parse(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

    // Adds spaces around operators (+, -, *, /) in the formula for easier parsing
    std::string addSpacesAroundOperators(const std::string& formula);

    // Evaluates an expression (e.g., "A1 + B2") and computes its value using the provided cell values
    double evaluateExpression(const std::string& expression, const std::unordered_map<std::string, double>& cellValues);

    // Gets the numeric value of a specific cell reference (e.g., "A1") from the provided cell values
    double getCellValue(const std::string& cellRef, const std::unordered_map<std::string, double>& cellValues);

    // Evaluates a function (e.g., SUM, AVER, MAX) over a range of cells using the provided cell values
    double evaluateFunction(const std::string& funcName, const std::string& range, const std::unordered_map<std::string, double>& cellValues);

    // Extracts all cell references (e.g., A1, B2) from a formula
    std::set<std::string> extractCellReferences(const std::string& formula);

    // Retrieves the column letter from a cell reference (e.g., "A1" -> 'A')
    char getColumnLetter(const std::string& cellRef);

    // Retrieves the row number from a cell reference (e.g., "A1" -> 1)
    int getRowNumber(const std::string& cellRef);

private:
    // Parses a range of cells (e.g., "A1..B2") and returns a list of individual cell references
    std::vector<std::string> parseRange(const std::string& range);
};

#endif
