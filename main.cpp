#include <iostream>
#include "user.h"
#include "BankAccounts.h"
using namespace std;

int main() 
{
    User a1;
    string username = "user123";
    string password = "pass@Word12";

    // Establish the MySQL connection
    // Fill the details, in below line
    string HOST_NAME = "", USER_NAME = "", PASSWORD = "", DATABASE_NAME = "";
    bool isConnected = a1.establish_connection(HOST_NAME, USER_NAME, PASSWORD, DATABASE_NAME);


    if(isConnected) 
    {
        // Register a new user
        bool isRegSuccess = a1.register_user(username, password);

        // Login user
        bool isLoginSuccess = a1.login_user(username, password);

        // Check if the account is logged in
        bool check = a1.isLoggedin();
        if(check) {
            cout << "User Logged in Successfully\n";
        }
        else {
            cout << "User Logged in Failed\n";
        }
    }

    return 0;
}
