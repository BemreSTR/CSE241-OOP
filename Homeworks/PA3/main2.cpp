#include <string>
#include <iostream>
#include "WordString.h"

int main() {
    // Yapıcıları test edelim
    WordString ws1, ws2;
    // C-string ile başlatma
    WordString ws3("This is a c-string literal");
    // std::string ile başlatma
    std::string boringString = "Just another literal living in this main function";
    WordString ws4(boringString);
    
    // Çıktı operatörünü test edelim
    std::cout << "ws4: " << ws4 << std::endl;
    
    // toString() fonksiyonu testi
    std::string catcher = ws3.toString();
    std::cout << "ws3 toString: " << catcher << std::endl;
    
    // Atama operatörünü test edelim
    ws3 = boringString;
    std::cout << "ws3 after assignment: " << ws3 << std::endl;
    
    // numberOfWords() fonksiyonunu test edelim
    int c = ws4.numberOfWords();
    std::cout << "Number of words in ws4: " << c << std::endl;
    
    // get() fonksiyonunu test edelim
    std::string word5 = ws4.get(4);
    std::cout << "Fifth word in ws4: " << word5 << std::endl;
    
    // replace() fonksiyonunu test edelim
    std::string new_word = "new";
    ws4.replace(3, new_word);
    std::cout << "ws4 after replacing 'living' with 'new': " << ws4 << std::endl;
    std::cout << "Third word now: " << ws4.get(3) << std::endl;
    
    // insertAfter() fonksiyonunu test edelim
    ws4.insertAfter(3, std::string("inserted string"));
    std::cout << "ws4 after insertion: " << ws4 << std::endl;
    
    // remove() fonksiyonunu test edelim
    ws4.remove(4);
    std::cout << "ws4 after removing fourth word: " << ws4 << std::endl;
    
    // strip() fonksiyonunu test edelim
    WordString ws5("  word1   word2    word3  ");
    std::cout << "ws5 before strip: '" << ws5 << "'" << std::endl;
    ws5.strip();
    std::cout << "ws5 after strip: '" << ws5 << "'" << std::endl;
    
    // + operatörü testi (WordString + WordString)
    ws2 = ws4 + ws3;
    std::cout << "ws4 + ws3: " << ws2 << std::endl;
    
    // + operatörü testi (WordString + string)
    std::string additional_string("additional data");
    ws2 = ws4 + additional_string;
    std::cout << "ws4 + additional_string: " << ws2 << std::endl;
    
    // == operatörü testi
    WordString ws6("Just another literal new string in this main function");
    if (ws4 == ws6) {
        std::cout << "ws4 and ws6 are equal" << std::endl;
    } else {
        std::cout << "ws4 and ws6 are not equal" << std::endl;
    }
    
    // Copy constructor testi
    WordString ws7 = ws4;
    std::cout << "ws7 (copy of ws4): " << ws7 << std::endl;
    
    // Dinamik bellek yönetimi testi - deep copy kontrolü
    ws7.replace(0, "Modified");
    std::cout << "ws7 after modification: " << ws7 << std::endl;
    std::cout << "Original ws4 unchanged: " << ws4 << std::endl;
    
    // cin operatörü testi
    std::cout << "Enter a word: ";
    std::cin >> ws1;
    std::cout << "You entered: " << ws1 << std::endl;
    
    return 0;
}