//main for q2
#include <iostream>
#include "username.h"
#include "password.h"
using namespace std;
int main()
{
    Authenticate::inputUserName();
    Authenticate::inputPassword();

    cout << "Username: " << Authenticate::getUserName() << endl;
    cout << "Password: " << Authenticate::getPassword() << endl;
    return 0;
}
