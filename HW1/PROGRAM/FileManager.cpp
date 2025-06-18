#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Save the spreadsheet to a CSV file
bool FileManager::saveToFile(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return false;
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
    return true;
}

// Load spreadsheet data from a CSV file
bool FileManager::loadFromFile(const std::string& filename, std::vector<std::vector<std::string>>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for reading." << std::endl;
        return false;
    }

    data.clear();
    std::string line;
    size_t maxColumns = 0; // To track the maximum number of columns

    // Read file line by line
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::istringstream stream(line);
        std::string cell;

        // Split the line by commas
        while (std::getline(stream, cell, ',')) {
            row.push_back(cell);
        }

        // Update maximum column count
        maxColumns = std::max(maxColumns, row.size());
        data.push_back(row);
    }

    // Ensure all rows have the same number of columns by padding empty cells
    for (auto& row : data) {
        while (row.size() < maxColumns) {
            row.push_back(""); // Add empty cells to match the maximum column count
        }
    }

    file.close();
    return true;
}
// Create a new empty file
bool FileManager::createNewFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filename << "." << std::endl;
        return false;
    }
    file.close();
    return true;
}

// Save the spreadsheet to a new file
bool FileManager::saveAs(const std::string& newFilename, const std::vector<std::vector<std::string>>& data) {
    return saveToFile(newFilename, data);
}
