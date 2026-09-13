// ==========================================
// main.cpp - SecureShop ka main driver file
// Yahan se pura program shuru hota hai
// Sirf basic C++: variables, loops, if/else, file handling
// Koi class nahi, koi struct nahi, koi banned library nahi
// ==========================================

#include <iostream>
#include <fstream>
#include "customer.h"
#include "employee.h"
#include "admin.h"

using namespace std;

// ============================================================
// SETUP DEFAULT DATA
// Pehli baar chalane par default admin aur sample products banana
// ============================================================
void setupDefaultData() {
    // Check karo ke Users.txt exist karta hai aur data hai ya nahi
    ifstream checkFile("Users.txt");
    if (checkFile.is_open()) {
        char firstLine[300];
        checkFile.getline(firstLine, 300);
        checkFile.close();
        if (firstLine[0] != '\0') {
            // Data pehle se hai, kuch nahi karna
            return;
        }
    } else {
        checkFile.close();
    }

    // Default admin banana
    // Password "admin123" ko Caesar cipher (shift 3) se encrypt karo
    char encPass[100];
    encryptPassword("admin123", encPass, 3);

    ofstream userFile("Users.txt");
    if (userFile.is_open()) {
        // Format: username|encryptedPassword|name|role
        userFile << "admin|" << encPass << "|System Administrator|admin\n";
        userFile.close();
        cout << "Default admin created.\n";
        cout << "Username: admin\n";
        cout << "Password: admin123\n\n";
    }

    // Sample products banana
    ifstream checkProd("Products.txt");
    if (checkProd.is_open()) {
        char firstLine[300];
        checkProd.getline(firstLine, 300);
        checkProd.close();
        if (firstLine[0] != '\0') return; // Products pehle se hain
    }
    checkProd.close();

    ofstream prodFile("Products.txt");
    if (prodFile.is_open()) {
        // Format: productId|name|price|stock|category
        prodFile << "P001|Laptop|75000|10|Electronics\n";
        prodFile << "P002|Wireless Mouse|1500|50|Electronics\n";
        prodFile << "P003|Mechanical Keyboard|2500|30|Electronics\n";
        prodFile << "P004|USB Hub|800|40|Electronics\n";
        prodFile << "P005|Monitor 24 inch|25000|15|Electronics\n";
        prodFile << "P006|T-Shirt|800|100|Clothing\n";
        prodFile << "P007|Jeans|2000|60|Clothing\n";
        prodFile << "P008|Jacket|3500|25|Clothing\n";
        prodFile << "P009|Notebook|150|200|Stationery\n";
        prodFile << "P010|Pen Set|350|150|Stationery\n";
        prodFile.close();
        cout << "Sample products loaded.\n\n";
    }
}

// ============================================================
// WELCOME BANNER
// ============================================================
void showBanner() {
    cout << "\n";
    cout << "  =============================================\n";
    cout << "  ||                                         ||\n";
    cout << "  ||    SECURESHOP - Secure Shopping         ||\n";
    cout << "  ||    Platform for Cybersecurity Students  ||\n";
    cout << "  ||                                         ||\n";
    cout << "  =============================================\n";
    cout << "\n";
}

// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {
    showBanner();
    setupDefaultData();

    int choice = 0;

    while (1) {
        cout << "===== MAIN MENU =====\n";
        cout << "1. Customer Login\n";
        cout << "2. Customer Registration\n";
        cout << "3. Employee Login\n";
        cout << "4. Admin Login\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        // Input validate karo
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number 1-5.\n\n";
            continue;
        }

        if (choice < 1 || choice > 5) {
            cout << "Invalid choice! Please enter 1-5.\n\n";
            continue;
        }

        // ---- CUSTOMER LOGIN ----
        if (choice == 1) {
            char loggedInUser[50];
            char loggedInName[100];

            // Login attempt karo
            int success = loginCustomer(loggedInUser, loggedInName);

            if (success == 1) {
                // Login ho gaya, menu dikhao
                customerMenu(loggedInUser);
            }
        }

        // ---- CUSTOMER REGISTRATION ----
        else if (choice == 2) {
            registerCustomer();
        }

        // ---- EMPLOYEE LOGIN ----
        else if (choice == 3) {
            char loggedInUser[50];
            char loggedInName[100];

            int success = loginEmployee(loggedInUser, loggedInName);

            if (success == 1) {
                employeeMenu(loggedInUser);
            }
        }

        // ---- ADMIN LOGIN ----
        else if (choice == 4) {
            char loggedInUser[50];
            char loggedInName[100];

            int success = loginAdmin(loggedInUser, loggedInName);

            if (success == 1) {
                adminMenu(loggedInUser);
            }
        }

        // ---- EXIT ----
        else if (choice == 5) {
            cout << "\nThank you for using SecureShop. Goodbye!\n\n";
            break;
        }

        cout << "\n";
    }

    return 0;
}
