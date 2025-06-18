#include "AnsiTerminal.h"
#include <iostream>
#include <unistd.h>   // For read()
#include <termios.h>  // For terminal control

// Constructor: Configure terminal for non-canonical mode
// Saves the original terminal settings and disables canonical mode and echo for real-time input reading.
AnsiTerminal::AnsiTerminal() {
    // Save the original terminal settings
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;

    // Disable canonical mode and echo for real-time input reading
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Destructor: Restore the terminal settings to their original state
AnsiTerminal::~AnsiTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

// Method to print text at a specified position
// Moves the cursor to the given row and column and prints the provided text.
void AnsiTerminal::printAt(int row, int col, const std::string &text) {
    std::cout << "\033[" << row << ";" << col << "H" << text << std::flush;
}

// Method to print text with an inverted background at a specified position
// Moves the cursor to the given position, applies inverted background mode, and prints the text.
void AnsiTerminal::printInvertedAt(int row, int col, const std::string &text) {
    std::cout << "\033[" << row << ";" << col << "H\033[7m" << text << "\033[0m" << std::flush;
    // \033[7m enables reverse video mode, \033[0m resets to normal
}

// Method to clear the terminal screen
// Clears all content from the terminal and moves the cursor to the home position.
void AnsiTerminal::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush; // Clear screen and move cursor to home
}

// Method to get a single keystroke from the terminal
// Reads and returns a single character from the terminal input.
char AnsiTerminal::getKeystroke() {
    char ch;
    read(STDIN_FILENO, &ch, 1);  // Read the first character

    // If the character falls within ASCII range for control characters
    // Ctrl+A to Ctrl+Z are 0x01 to 0x1A, mapping to 'a' - '@'
    if (ch >= 1 && ch <= 26) {
        // Map Ctrl+A to Ctrl+Z to a more readable form
        return ch + 'A' - 1;
    }

    return ch;  // Return the character as-is if it's a regular key
}

// Method to handle arrow key sequences, Alt keys, and other special keys
// Processes special keys such as arrow keys or Alt combinations, and returns a corresponding character code.
char AnsiTerminal::getSpecialKey() {
    char ch = getKeystroke();

    // If it's an escape character, we might be dealing with an escape sequence
    if (ch == '\033') {
        // Peek at the next character to see if it's an arrow key or Alt key combination
        char next_ch;
        if (read(STDIN_FILENO, &next_ch, 1) == 0) return '\033';  // If no more input, it's just ESC

        // Arrow keys and some function keys produce sequences starting with '[' after '\033'
        if (next_ch == '[') {
            char arrow_key;
            if (read(STDIN_FILENO, &arrow_key, 1) == 1) {
                switch (arrow_key) {
                    case 'A': return 'U'; // Up arrow
                    case 'B': return 'D'; // Down arrow
                    case 'C': return 'R'; // Right arrow
                    case 'D': return 'L'; // Left arrow
                    // Add cases for other keys like Home, End, PgUp, PgDn, if needed
                }
            }
        } else {
            // If it's not an arrow sequence, it could be an Alt+Key combination
            // Return the second character as the key pressed with Alt
            return next_ch | 0x80;  // Set high bit to distinguish Alt
        }
    }

    // If it's a normal character or Ctrl combination, return as-is
    return ch;
}

// Method to get input from the terminal with basic editing support
// Reads characters from the terminal, processes backspace and Enter keys, and returns the final input as a string.
std::string AnsiTerminal::getInputWithEditing() {
    std::string input;
    char ch;

    // Configure terminal settings
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable immediate echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Process user input
    while (true) {
        ch = getchar();

        if (ch == '\n') { // Input completes with Enter
            std::cout << std::endl;
            break;
        } else if (ch == 127 || ch == 8) { // Backspace (127: Linux, 8: some terminals)
            if (!input.empty()) {
                input.pop_back(); // Remove the last character
                std::cout << "\b \b"; // Move cursor back and clear character
            }
        } else {
            input += ch; // Append character to input
            std::cout << ch; // Echo character to the terminal
        }
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return input;
}
