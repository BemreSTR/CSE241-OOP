#include <iostream>
#include <cstring>
using namespace std;

const int MAX_DIGITS = 20;

int main() {
    char number1[MAX_DIGITS + 1], number2[MAX_DIGITS + 1], choice;
    
    do {
        int num1[MAX_DIGITS] = {0}, num2[MAX_DIGITS] = {0}, sumArray[MAX_DIGITS] = {0};
        int len1, len2;
        
        cout << "Enter the first number: ";
        cin >> number1;
        
        cout << "Enter the second number: ";
        cin >> number2;
        
        // Get the lengths of the numbers
        len1 = strlen(number1);
        len2 = strlen(number2);
        
        // Convert characters to integers and store in reverse order
        for (int i = 0; i < len1; i++)
            num1[i] = number1[len1 - 1 - i] - '0';
        for (int i = 0; i < len2; i++)
            num2[i] = number2[len2 - 1 - i] - '0';
        
        int carry = 0;
        int resultLength = (len1 > len2 ? len1 : len2);
        
        // Perform addition digit by digit
        for (int i = 0; i < resultLength; i++) {
            int total = num1[i] + num2[i] + carry;
            sumArray[i] = total % 10;
            carry = total / 10;
        }
        
        // Handle the final carry
        if (carry) {
            if (resultLength >= MAX_DIGITS)
                resultLength = -1; // Overflow
            else
                sumArray[resultLength++] = carry;
        }
        
        // Check for overflow
        if (resultLength == -1)
            cout << "Integer overflow" << endl;
        else {
            for (int i = resultLength - 1; i >= 0; i--)
                cout << sumArray[i];
            cout << endl;
        }
        
        cout << "Do you want to add more numbers? (y/n): ";
        cin >> choice;
        
    } while (choice == 'y' || choice == 'Y');
    
    return 0;
}