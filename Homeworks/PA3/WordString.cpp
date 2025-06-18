#include "WordString.h"

// Default constructor
WordString::WordString() : data(""), wordIndices(nullptr), wordCount(0), capacity(0) {
}

// C-string constructor
WordString::WordString(const char* str) : data(str), wordIndices(nullptr), wordCount(0), capacity(0) {
    parseWords();
}

// std::string constructor
WordString::WordString(const std::string& str) : data(str), wordIndices(nullptr), wordCount(0), capacity(0) {
    parseWords();
}

// Copy constructor
WordString::WordString(const WordString& other) : data(other.data), wordIndices(nullptr), wordCount(other.wordCount), capacity(other.wordCount) {
    if (wordCount > 0) {
        wordIndices = new WordInfo[wordCount];
        for (size_t i = 0; i < wordCount; ++i) {
            wordIndices[i] = other.wordIndices[i];
        }
    }
}

// Destructor
WordString::~WordString() {
    delete[] wordIndices;
}

// Assignment operator
WordString& WordString::operator=(const WordString& other) {
    if (this != &other) {
        data = other.data;
        
        delete[] wordIndices;
        wordIndices = nullptr;
        wordCount = other.wordCount;
        capacity = other.wordCount;
        
        if (wordCount > 0) {
            wordIndices = new WordInfo[wordCount];
            for (size_t i = 0; i < wordCount; ++i) {
                wordIndices[i] = other.wordIndices[i];
            }
        }
    }
    return *this;
}

// Parse the string to find all words
void WordString::parseWords() {
    delete[] wordIndices;
    wordIndices = nullptr;
    wordCount = 0;
    capacity = 10; // Start with space for 10 words
    
    if (data.empty()) {
        return;
    }
    
    wordIndices = new WordInfo[capacity];
    
    bool inWord = false;
    size_t wordStart = 0;
    
    for (size_t i = 0; i < data.length(); ++i) {
        if (!std::isspace(data[i])) {
            if (!inWord) {      
                inWord = true;
                wordStart = i;
            }
        } else {
            if (inWord) {
                inWord = false;
                
                // Check if we need to resize the array
                if (wordCount >= capacity) {
                    resizeIndicesArray(capacity * 2);
                }
                
                // Add the word to our index
                wordIndices[wordCount].start_index = wordStart;
                wordIndices[wordCount].length = i - wordStart;
                ++wordCount;
            }
        }
    }
    
    // Check if the last character was part of a word
    if (inWord) {
        // Check if we need to resize the array
        if (wordCount >= capacity) {
            resizeIndicesArray(capacity * 2);
        }
        
        // Add the word to our index
        wordIndices[wordCount].start_index = wordStart;
        wordIndices[wordCount].length = data.length() - wordStart;
        ++wordCount;
    }
}

// Resize the indices array
void WordString::resizeIndicesArray(size_t newCapacity) {
    WordInfo* newArray = new WordInfo[newCapacity];
    
    for (size_t i = 0; i < wordCount; ++i) {
        newArray[i] = wordIndices[i];
    }
    
    delete[] wordIndices;
    wordIndices = newArray;
    capacity = newCapacity;
}

// Return string representation
std::string WordString::toString() const {
    return data;
}

// Return number of words
size_t WordString::numberOfWords() const {
    return wordCount;
}

// Get a specific word by index
std::string WordString::get(size_t index) const {
    if (index >= wordCount) {
        return "";
    }
    
    return data.substr(wordIndices[index].start_index, wordIndices[index].length);
}

// Replace a word at the specified index
void WordString::replace(size_t index, const std::string& newWord) {
    if (index >= wordCount) {
        return;
    }
    
    // Replace the word in the original string
    data.replace(wordIndices[index].start_index, wordIndices[index].length, newWord);
    
    // Update the indices since we've changed the string
    parseWords();
}

// Insert a string after the word at the specified index
void WordString::insertAfter(size_t index, const std::string& str) {
    if (index >= wordCount) {
        return;
    }
    
    // Calculate the position to insert the new string
    size_t insertPos = wordIndices[index].start_index + wordIndices[index].length;
    
    // Insert the string with a space before and after
    data.insert(insertPos, " " + str + " ");
    
    // Update the indices since we've changed the string
    parseWords();
}

// Remove the word at the specified index
void WordString::remove(size_t index) {
    if (index >= wordCount) {
        return;
    }
    
    size_t start, length;
    
    // Handle space removal according to requirements
    if (index == 0) {
        // Remove the space after if it's the first word
        start = wordIndices[index].start_index;
        length = wordIndices[index].length;
        
        // Check if there's a space after the word and it's not the only word
        if (wordCount > 1 && start + length < data.length() && std::isspace(data[start + length])) {
            length += 1; // Include the space after
        }
    } else {
        // Remove a space before for other words
        start = wordIndices[index].start_index - 1; // Include one space before
        length = wordIndices[index].length + 1;     // Include that space in the length
        
        // Handle edge case - no space before
        if (start >= data.length()) {
            start = wordIndices[index].start_index;
            length = wordIndices[index].length;
        }
    }
    
    // Remove the word from the string
    data.erase(start, length);
    
    // Update the indices
    parseWords();
}

// Strip excessive spaces
void WordString::strip() {
    if (data.empty()) {
        return;
    }
    
    std::string result;
    bool prevSpace = true; // Start   with  true to handle leading spaces
    
    for (size_t i = 0; i < data.length(); ++i) {
        if (std::isspace(data[i])) {
            if (!prevSpace) {
                result += ' ';
                prevSpace = true;
            }
        } else {
            result += data[i];
            prevSpace = false;
        }
    }
    
    // Remove trailing space if any
    if (!result.empty() && std::isspace(result.back())) {
        result.pop_back();
    }
    
    data = result;
    parseWords();
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const WordString& ws) {
    os << ws.data;
    return os;
}

// Input stream operator
std::istream& operator>>(std::istream& is, WordString& ws) {
    is >> ws.data;
    ws.parseWords();
    return is;
}

// Addition operator (WordString + WordString)
WordString WordString::operator+(const WordString& other) const {
    WordString result(*this);
    result.data += " " + other.data;
    result.parseWords();
    return result;
}

// Addition operator (WordString + string)
WordString WordString::operator+(const std::string& str) const {
    WordString result(*this);
    result.data += " " + str;
    result.parseWords();
    return result;
}

// Equality operator
bool WordString::operator==(const WordString& other) const {
    return data == other.data;
}

// Inequality operator
bool WordString::operator!=(const WordString& other) const {
    return !(*this == other);
}

// Index operator
std::string WordString::operator[](size_t index) const {
    if (index >= wordCount) {
        return "";
    }
    
    return data.substr(wordIndices[index].start_index, wordIndices[index].length);
}