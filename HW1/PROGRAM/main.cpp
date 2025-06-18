#include "Spreadsheet.h"
#include "AnsiTerminal.h"
#include "FileManager.h"
#include <iostream>

int main() {
    const int visibleRows = 10; // Fixed number of visible rows on the screen
    const int visibleCols = 10; // Fixed number of visible columns on the screen

    int defaultRows = 20, defaultCols = 20; // Default grid size for the spreadsheet
    Spreadsheet sheet(defaultRows, defaultCols);
    FileManager fileManager;

    AnsiTerminal terminal; // Terminal object for handling screen output and input
    terminal.clearScreen();

    int row = 0, col = 0; // Initial cursor position
    std::string currentFile = "untitled.csv"; // Default filename for the spreadsheet

    while (true) {
        terminal.clearScreen();
        sheet.display(visibleRows, visibleCols, row, col); // Display the spreadsheet

        // Instructions for the user
        std::cout << "\nCommands: [U/D/L/R] Move, [e] Edit Cell, [s] Save, [l] Load, [n] New File, [a] Save As, [q] Quit\n";

        char key = terminal.getSpecialKey(); // Get the user's command input

        switch (key) {
            case 'U': { // Move cursor up
                row = (row > 0) ? row - 1 : row; // Prevent moving above the first row
                if (row < sheet.verticalOffset) {
                    sheet.verticalOffset--; // Adjust vertical offset if needed
                }
                break;
            }
            case 'D': { // Move cursor down
                row = (row < sheet.getRows() - 1) ? row + 1 : row; // Prevent moving below the last row
                if (row >= sheet.verticalOffset + visibleRows) {
                    sheet.verticalOffset++; // Adjust vertical offset if needed
                }
                sheet.autoExpandGrid(row, col); // Automatically expand grid if needed
                break;
            }
            case 'R': { // Move cursor right
                col = (col < sheet.getCols() - 1) ? col + 1 : col; // Prevent moving past the last column
                if (col >= sheet.horizontalOffset + visibleCols) {
                    sheet.horizontalOffset++; // Adjust horizontal offset if needed
                }
                sheet.autoExpandGrid(row, col); // Automatically expand grid if needed
                break;
            }
            case 'L': { // Move cursor left
                col = (col > 0) ? col - 1 : col; // Prevent moving before the first column
                if (col < sheet.horizontalOffset) {
                    sheet.horizontalOffset--; // Adjust horizontal offset if needed
                }
                break;
            }
            case 'e': { // Edit a cell
                terminal.clearScreen();
                sheet.display(visibleRows, visibleCols, row, col); // Redisplay the spreadsheet
                std::cout << "Enter value for cell " << columnName(col) << (row + 1) << ": ";
                std::string input = terminal.getInputWithEditing(); // Get the new value for the cell

                try {
                    sheet.setCell(row, col, input); // Update the cell with the input value
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    terminal.getKeystroke(); // Wait for user input before continuing
                }
                break;
            }
            case 's': { // Save the current spreadsheet
                if (!fileManager.saveToFile(currentFile, sheet.exportToData())) {
                    std::cerr << "Failed to save file: " << currentFile << std::endl;
                } else {
                    std::cout << "Spreadsheet saved to " << currentFile << std::endl;
                    terminal.getKeystroke();
                }
                break;
            }
            case 'l': { // Load a spreadsheet from a file
                std::cout << "Enter filename to load: ";
                std::string filename = terminal.getInputWithEditing();

                try {
                    sheet.loadSpreadsheet(filename); // Load the spreadsheet data
                    currentFile = filename; // Update the current file name
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

                terminal.getKeystroke(); // Wait for user input before proceeding
                break;
            }
            case 'n': { // Create a new spreadsheet
                std::cout << "Enter new filename: ";
                std::string filename = terminal.getInputWithEditing();
                if (fileManager.createNewFile(filename)) {
                    sheet.clear(); // Clear the current spreadsheet
                    currentFile = filename; // Update the current file name
                    std::cout << "New file created: " << filename << std::endl;
                } else {
                    std::cerr << "Failed to create file: " << filename << std::endl;
                }
                terminal.getKeystroke();
                break;
            }
            case 'a': { // Save the spreadsheet with a new filename
                std::cout << "Enter new filename to save as: ";
                std::string filename = terminal.getInputWithEditing();

                if (fileManager.saveAs(filename, sheet.exportToData())) {
                    currentFile = filename; // Update the current file name
                    std::cout << "Spreadsheet saved as " << filename << std::endl;
                } else {
                    std::cerr << "Failed to save file as: " << filename << std::endl;
                }
                terminal.getKeystroke();
                break;
            }
            case 'q': { // Quit the application
                terminal.clearScreen();
                std::cout << "Goodbye!" << std::endl;
                return 0;
            }
        }
    }
}
