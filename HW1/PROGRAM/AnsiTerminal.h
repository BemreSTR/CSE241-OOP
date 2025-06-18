#ifndef ANSI_TERMINAL_H
#define ANSI_TERMINAL_H

#include <iostream>
#include <string>
#include <termios.h>

// Class to handle terminal functionalities with ANSI escape sequences
class AnsiTerminal {
public:
    // Constructor: Sets up the terminal for capturing keystrokes
    AnsiTerminal();

    // Destructor: Restores the terminal settings to the original state
    ~AnsiTerminal();

    // Print text at a specified row and column
    void printAt(int row, int col, const std::string &text);

    // Print text with inverted background at a specified row and column
    void printInvertedAt(int row, int col, const std::string &text);

    // Clear the terminal screen
    void clearScreen();

    // Get a single keystroke from the terminal
    char getKeystroke();

    // Get the arrow key or special key input ('U', 'D', 'L', 'R' for Up, Down, Left, Right),
    // or detect other key combinations such as Alt+Key, Ctrl+Key, etc.
    char getSpecialKey();

    // Capture user input with backspace support (for editing inputs)
    std::string getInputWithEditing();

private:
    struct termios original_tio; // Holds the original terminal settings
};

#endif // ANSI_TERMINAL_H
