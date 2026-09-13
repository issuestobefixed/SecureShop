// ==========================================
// admin.cpp
// Admin ke sare functions yahan hain
// Sirf basic C++: variables, loops, if/else, file handling
// Koi class nahi, koi struct nahi, koi banned library nahi
// ==========================================

#include <iostream>
#include <fstream>
#include "customer.h"
#include "admin.h"

using namespace std;

// ============================================================
// OTP GENERATE - 2FA ke liye
// Username se simple OTP nikalna aur file mein save karna
// ============================================================
void generateOTP(const char* adminUser) {
    // Username ke characters se ek number banana
    int otp = 0;
    int i = 0;
    while (adminUser[i] != '\0') {
        otp = otp + (adminUser[i] * (i + 1));
        i++;
    }
    // 6 digit ka OTP banana
    otp = (otp % 900000) + 100000;
    if (otp < 100000) otp = 100000;

    // OTP file mein save karo
    char otpFileName[100];
    copyString(otpFileName, "OTP_");
    int len = stringLength(otpFileName);
    i = 0;
    while (adminUser[i] != '\0') {
        otpFileName[len] = adminUser[i];
        len++;
        i++;
    }
    otpFileName[len] = '.'; len++;
    otpFileName[len] = 't'; len++;
    otpFileName[len] = 'x'; len++;
    otpFileName[len] = 't'; len++;
    otpFileName[len] = '\0';

    ofstream otpFile(otpFileName);
    if (otpFile.is_open()) {
        char otpStr[10];
        intToString(otp, otpStr);
        otpFile << otpStr << "\n";
        otpFile.close();
    }

    // Admin ko OTP dikhao
    // Real system mein ye SMS ya email se jata
    char otpStr[10];
    intToString(otp, otpStr);
    cout << "\n[2FA] Your One-Time Password (OTP) is: " << otpStr << "\n";
    cout << "(In a real system this would be sent via SMS/Email)\n";
}

// ============================================================
// OTP VERIFY
// ============================================================
int verifyOTP(const char* adminUser) {
    char otpFileName[100];
    copyString(otpFileName, "OTP_");
    int len = stringLength(otpFileName);
    int i = 0;
    while (adminUser[i] != '\0') {
        otpFileName[len] = adminUser[i];
        len++;
        i++;
    }
    otpFileName[len] = '.'; len++;
    otpFileName[len] = 't'; len++;
    otpFileName[len] = 'x'; len++;
    otpFileName[len] = 't'; len++;
    otpFileName[len] = '\0';

    ifstream otpFile(otpFileName);
    if (!otpFile.is_open()) {
        cout << "OTP file not found.\n";
        return 0;
    }

    char storedOTP[10];
    otpFile.getline(storedOTP, 10);
    otpFile.close();

    char inputOTP[10];
    cout << "Enter OTP to verify: ";
    cin >> inputOTP;

    if (compareStrings(storedOTP, inputOTP) == 0) {
        remove(otpFileName); // OTP use ho gaya, delete karo
        return 1;
    }
    return 0;
}

// ============================================================
// ADMIN LOGIN - 2FA ke saath
// ============================================================
int loginAdmin(char* loggedInUser, char* loggedInName) {
    char inputUser[50];
    char inputPass[100];
    char decryptedPass[100];
    int failedAttempts = 0;

    cout << "\n===== ADMIN LOGIN =====\n";
    cout << "Enter username: ";
    cin >> inputUser;

    while (failedAttempts < 3) {
        cout << "Enter password: ";
        cin >> inputPass;

        ifstream userFile("Users.txt");
        if (!userFile.is_open()) {
            cout << "Error: Cannot open user database.\n";
            return 0;
        }

        int found = 0;
        char line[300];

        while (userFile.getline(line, 300)) {
            char storedUser[50];
            char storedPass[100];
            char storedName[100];
            char storedRole[20];
            int idx = 0;
            int i = 0;

            while (line[i] != '|' && line[i] != '\0') { storedUser[idx] = line[i]; idx++; i++; }
            storedUser[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') { storedPass[idx] = line[i]; idx++; i++; }
            storedPass[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') { storedName[idx] = line[i]; idx++; i++; }
            storedName[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '\0') { storedRole[idx] = line[i]; idx++; i++; }
            storedRole[idx] = '\0';

            if (compareStrings(storedUser, inputUser) == 0 &&
                compareStrings(storedRole, "admin") == 0) {

                decryptPassword(storedPass, decryptedPass, 3);

                if (compareStrings(decryptedPass, inputPass) == 0) {
                    found = 1;
                    userFile.close();

                    // 2FA - OTP generate karo aur verify karo
                    generateOTP(inputUser);
                    if (verifyOTP(inputUser) == 0) {
                        cout << "Invalid OTP! Access denied.\n";
                        return 0;
                    }

                    copyString(loggedInUser, storedUser);
                    copyString(loggedInName, storedName);

                    ofstream actLog("ActivityLogs.txt", ios::app);
                    if (actLog.is_open()) {
                        char ts[20];
                        getCurrentTimestamp(ts);
                        actLog << inputUser << "|LOGIN_SUCCESS|admin|" << ts << "\n";
                        actLog.close();
                    }

                    cout << "\nAdmin login successful! Welcome, " << storedName << "!\n";
                    return 1;
                }
            }
        }
        userFile.close();

        if (found == 0) {
            failedAttempts++;
            cout << "Wrong credentials. Attempts left: " << (3 - failedAttempts) << "\n";

            if (failedAttempts >= 3) {
                logSuspiciousActivity(inputUser, failedAttempts);
                cout << "\n[!] Too many failed attempts! Exiting.\n";
                exit(0);
            }
        }
    }
    return 0;
}

// ============================================================
// ADD USER
// ============================================================
void addUser() {
    char newUser[50];
    char newPass[100];
    char newName[100];
    char newRole[20];
    char encPass[100];

    cout << "\n===== ADD USER =====\n";
    cout << "Enter username: ";
    cin >> newUser;
    cout << "Enter password: ";
    cin >> newPass;
    cout << "Enter full name: ";
    cin.ignore();
    cin.getline(newName, 100);
    cout << "Enter role (customer/employee/admin): ";
    cin >> newRole;

    // Role validate karo
    if (compareStrings(newRole, "customer") != 0 &&
        compareStrings(newRole, "employee") != 0 &&
        compareStrings(newRole, "admin") != 0) {
        cout << "Invalid role! Must be customer, employee, or admin.\n";
        return;
    }

    // Duplicate username check karo
    ifstream checkFile("Users.txt");
    if (checkFile.is_open()) {
        char line[300];
        while (checkFile.getline(line, 300)) {
            char storedUser[50];
            int idx = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') { storedUser[idx] = line[i]; idx++; i++; }
            storedUser[idx] = '\0';

            if (compareStrings(storedUser, newUser) == 0) {
                cout << "Username already exists!\n";
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }

    encryptPassword(newPass, encPass, 3);

    ofstream userFile("Users.txt", ios::app);
    if (userFile.is_open()) {
        userFile << newUser << "|" << encPass << "|" << newName << "|" << newRole << "\n";
        userFile.close();
        cout << "User added successfully!\n";
    } else {
        cout << "Error saving user.\n";
    }
}

// ============================================================
// MODIFY USER
// ============================================================
void modifyUser() {
    cout << "\n===== MODIFY USER =====\n";
    viewAllUsers();

    char editUser[50];
    cout << "Enter username to modify: ";
    cin >> editUser;

    char newPass[100];
    char newName[100];
    char newRole[20];
    char encPass[100];

    cout << "Enter new password (type x to keep old): ";
    cin >> newPass;
    cout << "Enter new full name: ";
    cin.ignore();
    cin.getline(newName, 100);
    cout << "Enter new role (customer/employee/admin): ";
    cin >> newRole;

    if (compareStrings(newRole, "customer") != 0 &&
        compareStrings(newRole, "employee") != 0 &&
        compareStrings(newRole, "admin") != 0) {
        cout << "Invalid role!\n";
        return;
    }

    ifstream uIn("Users.txt");
    ofstream uOut("Users_Temp.txt");
    int found = 0;

    if (uIn.is_open() && uOut.is_open()) {
        char line[300];
        while (uIn.getline(line, 300)) {
            char storedUser[50];
            char storedPass[100];
            char storedName[100];
            char storedRole[20];
            int idx = 0;
            int i = 0;

            while (line[i] != '|' && line[i] != '\0') { storedUser[idx] = line[i]; idx++; i++; }
            storedUser[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') { storedPass[idx] = line[i]; idx++; i++; }
            storedPass[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') { storedName[idx] = line[i]; idx++; i++; }
            storedName[idx] = '\0'; i++;
            idx = 0;
            while (line[i] != '\0') { storedRole[idx] = line[i]; idx++; i++; }
            storedRole[idx] = '\0';

            if (compareStrings(storedUser, editUser) == 0) {
                char finalPass[100];
                if (compareStrings(newPass, "x") == 0) {
                    // Purana password rakhna hai
                    copyString(finalPass, storedPass);
                } else {
                    // Naya password encrypt karo
                    encryptPassword(newPass, encPass, 3);
                    copyString(finalPass, encPass);
                }
                uOut << storedUser << "|" << finalPass << "|" << newName << "|" << newRole << "\n";
                found = 1;
            } else {
                uOut << line << "\n";
            }
        }
        uIn.close();
        uOut.close();
        remove("Users.txt");
        rename("Users_Temp.txt", "Users.txt");
    }

    if (found == 1) cout << "User modified successfully!\n";
    else cout << "User not found.\n";
}

// ============================================================
// REMOVE USER
// ============================================================
void removeUser() {
    cout << "\n===== REMOVE USER =====\n";
    viewAllUsers();

    char delUser[50];
    cout << "Enter username to remove: ";
    cin >> delUser;

    ifstream uIn("Users.txt");
    ofstream uOut("Users_Temp.txt");
    int found = 0;

    if (uIn.is_open() && uOut.is_open()) {
        char line[300];
        while (uIn.getline(line, 300)) {
            char storedUser[50];
            int idx = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') { storedUser[idx] = line[i]; idx++; i++; }
            storedUser[idx] = '\0';

            if (compareStrings(storedUser, delUser) == 0) {
                found = 1; // Is line ko skip karo = delete
            } else {
                uOut << line << "\n";
            }
        }
        uIn.close();
        uOut.close();
        remove("Users.txt");
        rename("Users_Temp.txt", "Users.txt");
    }

    if (found == 1) cout << "User removed!\n";
    else cout << "User not found.\n";
}

// ============================================================
// VIEW ALL USERS
// ============================================================
void viewAllUsers() {
    cout << "\n===== ALL USERS =====\n";
    cout << "----------------------------------------------\n";
    cout << "Username       | Name                | Role\n";
    cout << "----------------------------------------------\n";

    ifstream userFile("Users.txt");
    if (!userFile.is_open()) {
        cout << "No users found.\n";
        return;
    }

    char line[300];
    while (userFile.getline(line, 300)) {
        char user[50];
        char pass[100];
        char name[100];
        char role[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { user[idx] = line[i]; idx++; i++; }
        user[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pass[idx] = line[i]; idx++; i++; }
        pass[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { name[idx] = line[i]; idx++; i++; }
        name[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { role[idx] = line[i]; idx++; i++; }
        role[idx] = '\0';

        cout << user << " | " << name << " | " << role << "\n";
    }
    userFile.close();
    cout << "----------------------------------------------\n";
}

// ============================================================
// BULK IMPORT PRODUCTS
// File se products padhna aur Products.txt mein add karna
// Import file format: pid|name|price|stock|category
// ============================================================
void bulkImportProducts() {
    char importFileName[100];

    cout << "\n===== BULK IMPORT PRODUCTS =====\n";
    cout << "Enter import filename (e.g. import.txt): ";
    cin >> importFileName;

    ifstream impFile(importFileName);
    if (!impFile.is_open()) {
        cout << "File not found: " << importFileName << "\n";
        return;
    }

    ofstream prodFile("Products.txt", ios::app);
    if (!prodFile.is_open()) {
        cout << "Cannot open Products.txt\n";
        impFile.close();
        return;
    }

    char line[300];
    int count = 0;

    while (impFile.getline(line, 300)) {
        // Count pipes to validate format (need exactly 4 pipes)
        int pipes = 0;
        int i = 0;
        while (line[i] != '\0') {
            if (line[i] == '|') pipes++;
            i++;
        }
        if (pipes == 4) {
            prodFile << line << "\n";
            count++;
        }
    }

    impFile.close();
    prodFile.close();

    cout << count << " product(s) imported successfully!\n";
}

// ============================================================
// BULK EXPORT PRODUCTS
// Products.txt ka sara data ek export file mein copy karna
// ============================================================
void bulkExportProducts() {
    char exportFileName[100];

    cout << "\n===== BULK EXPORT PRODUCTS =====\n";
    cout << "Enter export filename (e.g. export.txt): ";
    cin >> exportFileName;

    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "No products to export.\n";
        return;
    }

    ofstream expFile(exportFileName);
    if (!expFile.is_open()) {
        cout << "Cannot create export file.\n";
        prodFile.close();
        return;
    }

    char line[300];
    int count = 0;
    while (prodFile.getline(line, 300)) {
        expFile << line << "\n";
        count++;
    }
    prodFile.close();
    expFile.close();

    cout << count << " product(s) exported to " << exportFileName << "\n";
}

// ============================================================
// DISCOUNTS
// ============================================================
void createDiscount() {
    char code[30];
    char amount[20];

    cout << "\n===== CREATE DISCOUNT =====\n";
    cout << "Enter discount code: ";
    cin >> code;
    cout << "Enter discount amount (Rs.): ";
    cin >> amount;

    if (stringToDouble(amount) <= 0) {
        cout << "Invalid amount.\n";
        return;
    }

    ofstream discFile("Discounts.txt", ios::app);
    if (discFile.is_open()) {
        // Format: code|amount
        discFile << code << "|" << amount << "\n";
        discFile.close();
        cout << "Discount code '" << code << "' created for Rs." << amount << "\n";
    } else {
        cout << "Error saving discount.\n";
    }
}

void removeDiscount() {
    cout << "\n===== REMOVE DISCOUNT =====\n";
    viewDiscounts();

    char delCode[30];
    cout << "Enter discount code to remove: ";
    cin >> delCode;

    ifstream dIn("Discounts.txt");
    ofstream dOut("Discounts_Temp.txt");
    int found = 0;

    if (dIn.is_open() && dOut.is_open()) {
        char line[200];
        while (dIn.getline(line, 200)) {
            char code[30];
            int idx = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') { code[idx] = line[i]; idx++; i++; }
            code[idx] = '\0';

            if (compareStrings(code, delCode) == 0) {
                found = 1; // Skip this line = delete
            } else {
                dOut << line << "\n";
            }
        }
        dIn.close();
        dOut.close();
        remove("Discounts.txt");
        rename("Discounts_Temp.txt", "Discounts.txt");
    }

    if (found == 1) cout << "Discount removed!\n";
    else cout << "Discount code not found.\n";
}

void viewDiscounts() {
    cout << "\n===== ACTIVE DISCOUNTS =====\n";

    ifstream discFile("Discounts.txt");
    if (!discFile.is_open()) {
        cout << "No discounts available.\n";
        return;
    }

    char line[200];
    int count = 0;
    while (discFile.getline(line, 200)) {
        char code[30];
        char amount[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { code[idx] = line[i]; idx++; i++; }
        code[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { amount[idx] = line[i]; idx++; i++; }
        amount[idx] = '\0';

        cout << "Code: " << code << " | Discount: Rs." << amount << "\n";
        count++;
    }
    discFile.close();

    if (count == 0) cout << "No discounts found.\n";
}

// ============================================================
// VIEW ACTIVITY LOGS
// ============================================================
void viewActivityLogs() {
    cout << "\n===== ACTIVITY LOGS =====\n";

    ifstream logFile("ActivityLogs.txt");
    if (!logFile.is_open()) {
        cout << "No activity logs found.\n";
        return;
    }

    char line[300];
    int count = 0;
    while (logFile.getline(line, 300)) {
        cout << line << "\n";
        count++;
    }
    logFile.close();

    if (count == 0) cout << "No logs yet.\n";
}

// ============================================================
// VIEW SUSPICIOUS ACTIVITY
// ============================================================
void viewSuspiciousActivity() {
    cout << "\n===== SUSPICIOUS ACTIVITY LOG =====\n";

    ifstream suspFile("SuspiciousActivity.txt");
    if (!suspFile.is_open()) {
        cout << "No suspicious activity recorded.\n";
        return;
    }

    char line[300];
    int count = 0;

    while (suspFile.getline(line, 300)) {
        char user[50];
        char attempts[5];
        char ts[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { user[idx] = line[i]; idx++; i++; }
        user[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { attempts[idx] = line[i]; idx++; i++; }
        attempts[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0';

        cout << "User: " << user << " | Failed Attempts: " << attempts
             << " | Timestamp: " << ts << "\n";
        count++;
    }
    suspFile.close();

    if (count == 0) cout << "No suspicious activity found.\n";
}

// ============================================================
// VIEW FRAUD REPORT
// ============================================================
void viewFraudReport() {
    cout << "\n===== FRAUD REPORT =====\n";

    ifstream fraudFile("FraudReport.txt");
    if (!fraudFile.is_open()) {
        cout << "No fraud reports found.\n";
        return;
    }

    char line[300];
    int count = 0;

    while (fraudFile.getline(line, 300)) {
        char uid[50];
        char qty[10];
        char amt[20];
        char ts[20];
        char reason[100];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { uid[idx] = line[i]; idx++; i++; }
        uid[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { qty[idx] = line[i]; idx++; i++; }
        qty[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { amt[idx] = line[i]; idx++; i++; }
        amt[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { reason[idx] = line[i]; idx++; i++; }
        reason[idx] = '\0';

        count++;
        cout << count << ". User: " << uid << " | Qty: " << qty
             << " | Amount: Rs." << amt << " | Date: " << ts
             << " | Reason: " << reason << "\n";
    }
    fraudFile.close();

    if (count == 0) cout << "No fraud reports.\n";
}

// ============================================================
// SALES AND REVENUE ANALYSIS
// ============================================================
void viewSalesAndRevenue() {
    cout << "\n===== SALES & REVENUE ANALYSIS =====\n";

    ifstream ordersFile("Orders.txt");
    if (!ordersFile.is_open()) {
        cout << "No sales data found.\n";
        return;
    }

    double totalRevenue = 0;
    int totalOrders = 0;

    // Top selling products track karne ke liye arrays
    char productIds[100][20];
    int productCounts[100];
    double productRevenue[100];
    int uniqueProducts = 0;

    // Sab initialize karo
    int k = 0;
    while (k < 100) {
        productIds[k][0] = '\0';
        productCounts[k] = 0;
        productRevenue[k] = 0;
        k++;
    }

    char line[300];
    while (ordersFile.getline(line, 300)) {
        char user[50];
        char pid[20];
        char pname[50];
        char pprice[20];
        char qty[10];
        char ts[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { user[idx] = line[i]; idx++; i++; }
        user[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pid[idx] = line[i]; idx++; i++; }
        pid[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pname[idx] = line[i]; idx++; i++; }
        pname[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pprice[idx] = line[i]; idx++; i++; }
        pprice[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { qty[idx] = line[i]; idx++; i++; }
        qty[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0';

        double price = stringToDouble(pprice);
        int q = stringToInt(qty);
        double lineRev = price * q;
        totalRevenue = totalRevenue + lineRev;
        totalOrders++;

        // Product entry update karo
        int found = 0;
        int j = 0;
        while (j < uniqueProducts) {
            if (compareStrings(productIds[j], pid) == 0) {
                productCounts[j] = productCounts[j] + q;
                productRevenue[j] = productRevenue[j] + lineRev;
                found = 1;
                break;
            }
            j++;
        }
        if (found == 0 && uniqueProducts < 100) {
            copyString(productIds[uniqueProducts], pid);
            productCounts[uniqueProducts] = q;
            productRevenue[uniqueProducts] = lineRev;
            uniqueProducts++;
        }
    }
    ordersFile.close();

    char revStr[20];
    doubleToString(totalRevenue, revStr, 2);
    cout << "Total Orders: " << totalOrders << "\n";
    cout << "Total Revenue: Rs." << revStr << "\n\n";

    // Bubble sort - sabse zyada bikne wale products upar
    int a = 0;
    while (a < uniqueProducts - 1) {
        int b = 0;
        while (b < uniqueProducts - a - 1) {
            if (productCounts[b] < productCounts[b + 1]) {
                // Swap karo
                char tempId[20];
                copyString(tempId, productIds[b]);
                copyString(productIds[b], productIds[b + 1]);
                copyString(productIds[b + 1], tempId);

                int tempCount = productCounts[b];
                productCounts[b] = productCounts[b + 1];
                productCounts[b + 1] = tempCount;

                double tempRev = productRevenue[b];
                productRevenue[b] = productRevenue[b + 1];
                productRevenue[b + 1] = tempRev;
            }
            b++;
        }
        a++;
    }

    cout << "--- Top Selling Products ---\n";
    int m = 0;
    while (m < uniqueProducts && m < 5) {
        char pRevStr[20];
        doubleToString(productRevenue[m], pRevStr, 2);
        char pcStr[10];
        intToString(productCounts[m], pcStr);
        cout << m + 1 << ". Product ID: " << productIds[m]
             << " | Units Sold: " << pcStr
             << " | Revenue: Rs." << pRevStr << "\n";
        m++;
    }
}

// ============================================================
// AUDIT TRAIL
// ============================================================
void logAuditTrail(const char* adminUser, const char* action) {
    ofstream auditFile("AuditTrail.txt", ios::app);
    if (auditFile.is_open()) {
        char ts[20];
        getCurrentTimestamp(ts);
        // Format: adminUser|action|timestamp
        auditFile << adminUser << "|" << action << "|" << ts << "\n";
        auditFile.close();
    }
}

void viewAuditTrail() {
    cout << "\n===== AUDIT TRAIL =====\n";

    ifstream auditFile("AuditTrail.txt");
    if (!auditFile.is_open()) {
        cout << "No audit records found.\n";
        return;
    }

    char line[300];
    int count = 0;
    while (auditFile.getline(line, 300)) {
        char admin[50];
        char action[50];
        char ts[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { admin[idx] = line[i]; idx++; i++; }
        admin[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { action[idx] = line[i]; idx++; i++; }
        action[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0';

        cout << "[" << ts << "] Admin: " << admin << " | Action: " << action << "\n";
        count++;
    }
    auditFile.close();

    if (count == 0) cout << "No audit records.\n";
}

// ============================================================
// ADMIN MENU
// ============================================================
void adminMenu(const char* currentUser) {
    int choice = 0;

    while (1) {
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1.  Add User\n";
        cout << "2.  Modify User\n";
        cout << "3.  Remove User\n";
        cout << "4.  View All Users\n";
        cout << "5.  Bulk Import Products\n";
        cout << "6.  Bulk Export Products\n";
        cout << "7.  Create Discount\n";
        cout << "8.  Remove Discount\n";
        cout << "9.  View Discounts\n";
        cout << "10. View Activity Logs\n";
        cout << "11. View Suspicious Activity\n";
        cout << "12. View Fraud Report\n";
        cout << "13. Sales & Revenue Analysis\n";
        cout << "14. View Audit Trail\n";
        cout << "15. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1) {
            addUser();
            logAuditTrail(currentUser, "ADD_USER");
        }
        else if (choice == 2) {
            modifyUser();
            logAuditTrail(currentUser, "MODIFY_USER");
        }
        else if (choice == 3) {
            removeUser();
            logAuditTrail(currentUser, "REMOVE_USER");
        }
        else if (choice == 4)  viewAllUsers();
        else if (choice == 5) {
            bulkImportProducts();
            logAuditTrail(currentUser, "BULK_IMPORT");
        }
        else if (choice == 6) {
            bulkExportProducts();
            logAuditTrail(currentUser, "BULK_EXPORT");
        }
        else if (choice == 7) {
            createDiscount();
            logAuditTrail(currentUser, "CREATE_DISCOUNT");
        }
        else if (choice == 8) {
            removeDiscount();
            logAuditTrail(currentUser, "REMOVE_DISCOUNT");
        }
        else if (choice == 9)  viewDiscounts();
        else if (choice == 10) viewActivityLogs();
        else if (choice == 11) viewSuspiciousActivity();
        else if (choice == 12) viewFraudReport();
        else if (choice == 13) viewSalesAndRevenue();
        else if (choice == 14) viewAuditTrail();
        else if (choice == 15) {
            logAuditTrail(currentUser, "LOGOUT");
            cout << "Admin logged out.\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
