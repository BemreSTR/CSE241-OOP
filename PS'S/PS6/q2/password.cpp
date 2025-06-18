#include "password.h"
#include <iostream>
#include <cctype>
using namespace std;

namespace
{
    string password;

    bool isValid()
    {
        if (password.length() < 8)
            return false;

        for (char c : password)
        {
            if (!isalpha(c))
                return true; // Found a non-letter
        }
        return false; // All are letters
    }
}

namespace Authenticate
{
    void inputPassword()
    {
        do
        {
            cout << "Enter your password (at least 8 characters and at least one non-letter): " << endl;
            cin >> password;
        } while (!isValid());
    }

    string getPassword()
    {
        return password;
    }
}
