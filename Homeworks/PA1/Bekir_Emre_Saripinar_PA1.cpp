/*
 * GAME: SECRET
 *
 * DESCRIPTION:
 *   This program implements the game "SECRET". The goal of the game is to guess the secret word.
 *   The secret word is composed of unique characters obtained from a file named "alphabet.txt".
 *   At each turn, the player enters a guess (a word) and the program returns two hints:
 *     - Cexact: The count of characters that match the secret word in the correct position.
 *     - Cmisplaced: The count of characters that are present in the secret word but in the wrong position.
 *
 * INPUT:
 *   The program is executed using command-line arguments.
 *   - "-r N": Creates a random secret word with N distinct characters.
 *   - "-u word": Uses the provided "word" as the secret word.
 *   Additionally, during the game, the player enters guess words through standard input.
 *
 * OUTPUT:
 *   - Initially, the alphabet (read from the file "alphabet.txt") is printed to the screen.
 *   - For each guess, the program prints the hints (first the Cexact count and then the Cmisplaced count).
 *   - If the player correctly guesses the secret word, the program prints:
 *         FOUND <guessCount>
 *   - If the player fails to guess the word in 100 attempts, the program prints:
 *         FAILED
 *
 * COMPILATION:
 *   The program can be compiled with the following command:
 *       g++ -std=c++11 <filename>.cpp -o SECRET
 *   It is developed to run in a Linux environment (Ubuntu) using the GCC compiler.
 *
 * NOTES:
 *   - In case of invalid input or parameters, the program outputs "INPUT ERROR" and exits.
 *
 * Bekir Emre Saripinar
 */

 #include <iostream>  // for input-output operations
 #include <fstream>   // for file operations
 #include <cstdlib>   // for atoi, rand, srand
 #include <ctime>     // for time (to seed srand)
 #include <cstring>   // for strlen, strcpy, strcmp
 #include <cctype>    // for tolower
 
 using namespace std;
 
 // Constant for maximum number of alphabet characters.
 const int MAX_ALPHABET = 100;
 
 // Maximum word length limit (as specified in the assignment: N <= length of alphabet).
 const int MAX_WORD_LENGTH = 100;
 
 // Function: Reads alphabet characters from the given file (alphabet.txt).
 // Excludes spaces, commas, and newline characters.
 // Ensures that the same character (in lowercase) is not added more than once.
 int readAlphabet(char alphabet[], const char* filename) {
     ifstream file(filename);
     if (!file.is_open()) {
         return -1;
     }
     int count = 0;
     char ch;
     // Read characters until the end of the file.
     while (file.get(ch) && count < MAX_ALPHABET) {
         // Convert character to lowercase (to treat uppercase and lowercase the same).
         ch = tolower(ch);
         // Skip space and comma characters.
         if (ch == ' ' || ch == ',') {
             continue;
         }
         // Skip newline characters.
         if (ch == '\n' || ch == '\r') {
             continue;
         }
         // Skip if the character has already been added.
         bool alreadyAdded = false;
         for (int i = 0; i < count; i++) {
             if (alphabet[i] == ch) {
                 alreadyAdded = true;
                 break;
             }
         }
         if (alreadyAdded) {
             continue;
         }
         // Add the new character.
         alphabet[count] = ch;
         count++;
     }
     file.close();
     return count;
 }
 
 // Function: Checks whether the given character exists in the alphabet.
 bool isCharInAlphabet(char ch, const char alphabet[], int alphabetCount) {
     for (int i = 0; i < alphabetCount; i++) {
         if (alphabet[i] == ch)
             return true;
     }
     return false;
 }
 
 // Function: Checks whether the given word contains repeated characters.
 bool hasRepeatedCharacters(const char word[]) {
     int length = strlen(word);
     for (int i = 0; i < length; i++) {
         for (int j = i + 1; j < length; j++) {
             if (word[i] == word[j])
                 return true;
         }
     }
     return false;
 }
 
 // Function: Validates the guessed word.
 // Conditions: correct length, does not contain characters outside the alphabet, and has no repeating characters.
 bool isValidWord(const char word[], int length, const char alphabet[], int alphabetCount) {
     // Check length.
     if (strlen(word) != (unsigned) length)
         return false;
     // Check for repeated characters.
     if (hasRepeatedCharacters(word))
         return false;
     // Check if each character is in the alphabet.
     for (int i = 0; i < length; i++) {
         if (!isCharInAlphabet(word[i], alphabet, alphabetCount))
             return false;
     }
     return true;
 }
 
 // Function: Calculates the exact and misplaced hints between the secret word and the guess.
 // exactCount: Number of characters that match in the correct position.
 // misplacedCount: Number of characters that exist in the secret word but in a different position.
 void computeHints(const char secretWord[], const char guess[], int length, int &exactCount, int &misplacedCount) {
     exactCount = 0;
     misplacedCount = 0;
 
     // First, count characters that are exactly in the correct position.
     for (int i = 0; i < length; i++) {
         if (guess[i] == secretWord[i])
             exactCount++;
     }
 
     // Then, count characters that exist in the secret word but in a different position.
     for (int i = 0; i < length; i++) {
         if (guess[i] != secretWord[i]) {
             for (int j = 0; j < length; j++) {
                 if (i != j && guess[i] == secretWord[j]) {
                     misplacedCount++;
                     break;  // Avoid counting the same character more than once.
                 }
             }
         }
     }
 }
 
 // Function: Generates a random secret word composed of non-repeating characters for the -r parameter.
 bool generateRandomWord(char secretWord[], int wordLength, const char alphabet[], int alphabetCount) {
     // The desired word length cannot be greater than the size of the alphabet.
     if (wordLength > alphabetCount)
         return false;
 
     // Array used to indicate which characters have already been chosen.
     bool chosen[MAX_ALPHABET] = { false };
     int count = 0;
     while (count < wordLength) {
         int randomIndex = rand() % alphabetCount;
         if (!chosen[randomIndex]) {
             secretWord[count] = alphabet[randomIndex];
             chosen[randomIndex] = true;
             count++;
         }
     }
     secretWord[wordLength] = '\0';
     return true;
 }
 
 int main(int argc, char* argv[]) {
     // Seed the random number generator.
     srand((unsigned) time(NULL));
 
     // Check command-line arguments.
     if (argc != 3) {
         cout << "INPUT ERROR" << endl;
         return 1;
     }
 
     // Array to store alphabet characters.
     char alphabet[MAX_ALPHABET];
     int alphabetCount = readAlphabet(alphabet, "alphabet.txt");
     if (alphabetCount <= 0) {
         cout << "INPUT ERROR" << endl;
         return 1;
     }
 
     // Array for the secret word.
     char secretWord[MAX_WORD_LENGTH];
     int wordLength = 0;
 
     // Process based on the command-line parameter.
     // If the -r parameter is used.
     if (strcmp(argv[1], "-r") == 0) {
         // argv[2] should be the number N.
         wordLength = atoi(argv[2]);
         if (wordLength <= 0 || wordLength > alphabetCount) {
             cout << "INPUT ERROR" << endl;
             return 1;
         }
         // Generate a valid random secret word.
         if (!generateRandomWord(secretWord, wordLength, alphabet, alphabetCount)) {
             cout << "INPUT ERROR" << endl;
             return 1;
         }
     }
     // If the -u parameter is used.
     else if (strcmp(argv[1], "-u") == 0) {
         // Use argv[2] directly as the secret word.
         wordLength = strlen(argv[2]);
         if (wordLength <= 0 || wordLength > alphabetCount) {
             cout << "INPUT ERROR" << endl;
             return 1;
         }
         strcpy(secretWord, argv[2]);
         // Validate the secret word.
         if (!isValidWord(secretWord, wordLength, alphabet, alphabetCount)) {
             cout << "INPUT ERROR" << endl;
             return 1;
         }
     }
     // If an invalid parameter is provided.
     else {
         cout << "INPUT ERROR" << endl;
         return 1;
     }
 
     // Print the alphabet to the screen.
     cout << "[";
     for (int i = 0; i < alphabetCount; i++) {
         cout << alphabet[i];
         if (i < alphabetCount - 1)
             cout << ",";
     }
     cout << "]" << endl;
 
     // Game loop: Allow up to 100 attempts.
     int guessCount = 0;
     char userGuess[MAX_WORD_LENGTH];
     while (guessCount < 100) {
         // Read the user's guess.
         cin >> userGuess;
         // Validate the input word.
         if (!isValidWord(userGuess, wordLength, alphabet, alphabetCount)) {
             cout << "INPUT ERROR" << endl;
             return 1;
         }
         
         // Increment the guess counter.
         guessCount++;
         
         // Calculate the hints.
         int exactCount = 0, misplacedCount = 0;
         computeHints(secretWord, userGuess, wordLength, exactCount, misplacedCount);
         
         // Print the hints (first the exact count, then the misplaced count).
         cout << exactCount << " " << misplacedCount << endl;
         
         // If the guess exactly matches the secret word, end the game successfully.
         if (exactCount == wordLength) {
             cout << "FOUND " << guessCount << endl;
             return 0;
         }
     }
     
     // If 100 attempts are exceeded, the game is considered failed.
     cout << "FAILED" << endl;
     return 0;
 }