#ifndef WORDSTRING_H
#define WORDSTRING_H

#include <string>
#include <iostream>

class WordString {
private:
    std::string data;       // Raw string data
    
    // Structure to store word indices and lengths
    struct WordInfo { 
         size_t start_index;  // Starting position of the word
        size_t length;      // Length of the word
    };
    
    WordInfo* wordIndices;  // Dynamic array of word information
    size_t wordCount;       // Number of words
    size_t capacity;        // Capacity of the wordIndices array
    
    // Helper functions
    void parseWords();      // Parse the string and find words
    void resizeIndicesArray(size_t newCapacity); // Resize the indices array
    
public:
    // Constructors and destructor
    WordString();   // Default constructor
    WordString(const char* str); // Constructor with c-string
    WordString(const std::string& str); // Constructor with std::string
    WordString(const WordString& other); // Copy constructor
    ~WordString(); // Destructor
    
    // Assignment operator
    WordString& operator=(const WordString& other);
    
    // String operations
    std::string toString() const;
    size_t numberOfWords() const;
    std::string get(size_t index) const;
    void replace(size_t index, const std::string& newWord);
    void insertAfter(size_t index, const std::string& str);
    void remove(size_t index);
    void strip();
    
    // Operator overloads
    friend std::ostream& operator<<(std::ostream& os, const WordString& ws);
    friend std::istream& operator>>(std::istream& is, WordString& ws);

    WordString operator+(const WordString& other) const;
    WordString operator+(const std::string& str) const;
    bool operator==(const WordString& other) const;
    bool operator!=(const WordString& other) const;
    std::string operator[](size_t index) const;
    
};

#endif // WORDSTRING_H