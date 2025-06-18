#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    // Saves the spreadsheet to a CSV file
    bool saveToFile(const std::string& filename, const std::vector<std::vector<std::string>>& data);

    // Loads spreadsheet data from a CSV file
    bool loadFromFile(const std::string& filename, std::vector<std::vector<std::string>>& data);

    // Creates a new empty file
    bool createNewFile(const std::string& filename);

    // Saves the spreadsheet to a new file
    bool saveAs(const std::string& newFilename, const std::vector<std::vector<std::string>>& data);
};

#endif // FILEMANAGER_H
