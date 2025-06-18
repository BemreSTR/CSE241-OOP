//set.h

#include <iostream>
#include <vector>

template<class T>
class Set {
public:
    Set();
    void add(T newItem);   
    int getSize() const;
    T* getArray() const;
private:
    std::vector<T> data;
};

// set.cpp
#include <iostream>
#include <vector>
#include "set.h" // Başlık dosyasını dahil et
#include <string>    // For std::string (for testing)

template<class T>
Set<T>::Set() {}

template<class T>
void Set<T>::add(T newItem) {
    // Check if the item already exists in the set
    for (const auto& item : data) {
        if (item == newItem) {
            return; // Item already exists, do not add
        }
    }
    data.push_back(newItem); // Add the new item to the set
}

template<class T>
int Set<T>::getSize() const {
    return data.size();
}

template<class T>
T* Set<T>::getArray() const {
    T* arr = new T[data.size()];
    for (size_t i = 0; i < data.size(); ++i) {
        arr[i] = data[i];
    }
    return arr;
}

// main.cpp
int main() {
    // Test with integers
    Set<int> intSet;
    intSet.add(10);
    intSet.add(20);
    intSet.add(10); // Adding a duplicate, should not be added
    std::cout << "Integer Set Size: " << intSet.getSize() << std::endl;
    int* intArray = intSet.getArray();
    std::cout << "Integer Set Elements: ";
    for (int i = 0; i < intSet.getSize(); ++i) {
        std::cout << intArray[i] << " ";
    }
    std::cout << std::endl;
    delete[] intArray; // Belleği serbest bırakmayı unutmayın

    std::cout << std::endl;

    // Test with strings
    Set<std::string> stringSet;
    stringSet.add("apple");
    stringSet.add("banana");
    stringSet.add("cherry");
    stringSet.add("banana"); // Adding a duplicate
    std::cout << "String Set Size: " << stringSet.getSize() << std::endl;
    std::string* stringArray = stringSet.getArray();
    std::cout << "String Set Elements: ";
    for (int i = 0; i < stringSet.getSize(); ++i) {
        std::cout << stringArray[i] << " ";
    }
    std::cout << std::endl;
    delete[] stringArray; // Belleği serbest bırakmayı unutmayın

    return 0;
}