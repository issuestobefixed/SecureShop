// ==========================================
// employee.cpp
// Employee ke sare functions yahan hain
// Sirf basic C++: variables, loops, if/else, file handling
// Koi class nahi, koi struct nahi, koi banned library nahi
// ==========================================

#include <iostream>
#include <fstream>
#include "customer.h"
#include "employee.h"

using namespace std;

// ============================================================
// EMPLOYEE LOGIN
// ============================================================
int loginEmployee(char* loggedInUser, char* loggedInName) {
    char inputUser[50];
    char inputPass[100];
    char decryptedPass[100];
    int failedAttempts = 0;

    cout << "\n===== EMPLOYEE LOGIN =====\n";
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
                compareStrings(storedRole, "employee") == 0) {

                decryptPassword(storedPass, decryptedPass, 3);

                if (compareStrings(decryptedPass, inputPass) == 0) {
                    copyString(loggedInUser, storedUser);
                    copyString(loggedInName, storedName);
                    found = 1;
                    userFile.close();

                    ofstream actLog("ActivityLogs.txt", ios::app);
                    if (actLog.is_open()) {
                        char ts[20];
                        getCurrentTimestamp(ts);
                        actLog << inputUser << "|LOGIN_SUCCESS|employee|" << ts << "\n";
                        actLog.close();
                    }

                    cout << "\nLogin successful! Welcome Employee, " << storedName << "!\n";
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
                cout << "\n[!] Too many failed attempts! Exiting for security.\n";
                exit(0);
            }
        }
    }
    return 0;
}

// ============================================================
// MANAGE SUPPORT REQUESTS
// ============================================================
void manageSupportRequests() {
    cout << "\n===== SUPPORT REQUESTS =====\n";

    ifstream supportFile("Support.txt");
    if (!supportFile.is_open()) {
        cout << "No support requests found.\n";
        return;
    }

    char line[300];
    int count = 0;

    while (supportFile.getline(line, 300)) {
        char user[50];
        char request[300];
        char ts[20];
        char status[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { user[idx] = line[i]; idx++; i++; }
        user[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { request[idx] = line[i]; idx++; i++; }
        request[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { status[idx] = line[i]; idx++; i++; }
        status[idx] = '\0';

        count++;
        cout << count << ". [" << status << "] User: " << user
             << " | Request: " << request << " | Date: " << ts << "\n";
    }
    supportFile.close();

    if (count == 0) {
        cout << "No support requests at the moment.\n";
        return;
    }

    cout << "\nEnter request number to respond to (0 to go back): ";
    int reqNum;
    cin >> reqNum;

    if (cin.fail() || reqNum <= 0 || reqNum > count) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    char response[300];
    cout << "Enter your response: ";
    cin.ignore();
    cin.getline(response, 300);

    // File update karo - matching line ko RESOLVED mark karo
    ifstream sIn("Support.txt");
    ofstream sOut("Support_Temp.txt");

    if (sIn.is_open() && sOut.is_open()) {
        char line2[300];
        int lineNum = 0;

        while (sIn.getline(line2, 300)) {
            lineNum++;
            if (lineNum == reqNum) {
                char user2[50];
                char req2[300];
                char ts2[20];
                int idx2 = 0;
                int i2 = 0;

                while (line2[i2] != '|' && line2[i2] != '\0') { user2[idx2] = line2[i2]; idx2++; i2++; }
                user2[idx2] = '\0'; i2++;
                idx2 = 0;
                while (line2[i2] != '|' && line2[i2] != '\0') { req2[idx2] = line2[i2]; idx2++; i2++; }
                req2[idx2] = '\0'; i2++;
                idx2 = 0;
                while (line2[i2] != '|' && line2[i2] != '\0') { ts2[idx2] = line2[i2]; idx2++; i2++; }
                ts2[idx2] = '\0';

                sOut << user2 << "|" << req2 << "|" << ts2 << "|RESOLVED|" << response << "\n";
                cout << "Request marked as RESOLVED.\n";
            } else {
                sOut << line2 << "\n";
            }
        }
        sIn.close();
        sOut.close();
        remove("Support.txt");
        rename("Support_Temp.txt", "Support.txt");
    }
}

// ============================================================
// ADD PRODUCT
// ============================================================
void addProduct(const char* empUsername) {
    char pid[20];
    char pname[50];
    char pprice[20];
    char pstock[10];
    char pcat[30];

    cout << "\n===== ADD PRODUCT =====\n";
    cout << "Enter Product ID: ";
    cin >> pid;

    // Check karo ke ID already exist karta hai ya nahi
    ifstream checkProd("Products.txt");
    if (checkProd.is_open()) {
        char line[300];
        while (checkProd.getline(line, 300)) {
            char existId[20];
            int idx = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') { existId[idx] = line[i]; idx++; i++; }
            existId[idx] = '\0';

            if (compareStrings(existId, pid) == 0) {
                cout << "Product ID already exists!\n";
                checkProd.close();
                return;
            }
        }
        checkProd.close();
    }

    cout << "Enter Product Name: ";
    cin.ignore();
    cin.getline(pname, 50);
    cout << "Enter Price (Rs.): ";
    cin >> pprice;
    cout << "Enter Stock Quantity: ";
    cin >> pstock;
    cout << "Enter Category: ";
    cin.ignore();
    cin.getline(pcat, 30);

    // Validate price aur stock
    double price = stringToDouble(pprice);
    int stock = stringToInt(pstock);

    if (price <= 0) {
        cout << "Invalid price. Must be greater than 0.\n";
        return;
    }
    if (stock < 0) {
        cout << "Invalid stock. Cannot be negative.\n";
        return;
    }

    ofstream prodFile("Products.txt", ios::app);
    if (prodFile.is_open()) {
        // Format: productId|name|price|stock|category
        prodFile << pid << "|" << pname << "|" << pprice << "|" << pstock << "|" << pcat << "\n";
        prodFile.close();
        cout << "Product added successfully!\n";

        ofstream actLog("ActivityLogs.txt", ios::app);
        if (actLog.is_open()) {
            char ts[20];
            getCurrentTimestamp(ts);
            actLog << empUsername << "|ADD_PRODUCT|" << pid << "|" << ts << "\n";
            actLog.close();
        }
    } else {
        cout << "Error adding product.\n";
    }
}

// ============================================================
// EDIT PRODUCT
// ============================================================
void editProduct(const char* empUsername) {
    cout << "\n===== EDIT PRODUCT =====\n";

    // Pehle sare products dikhao
    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "No products found.\n";
        return;
    }
    char line[300];
    cout << "Current Products:\n";
    cout << "--------------------------------------------------------------\n";
    while (prodFile.getline(line, 300)) {
        cout << line << "\n";
    }
    prodFile.close();
    cout << "--------------------------------------------------------------\n";

    char editId[20];
    cout << "Enter Product ID to edit: ";
    cin >> editId;

    char newName[50];
    char newPrice[20];
    char newStock[10];
    char newCat[30];

    cout << "Enter new Name: ";
    cin.ignore();
    cin.getline(newName, 50);
    cout << "Enter new Price: ";
    cin >> newPrice;
    cout << "Enter new Stock: ";
    cin >> newStock;
    cout << "Enter new Category: ";
    cin.ignore();
    cin.getline(newCat, 30);

    // Price aur stock validate karo
    if (stringToDouble(newPrice) <= 0) {
        cout << "Invalid price.\n";
        return;
    }
    if (stringToInt(newStock) < 0) {
        cout << "Invalid stock.\n";
        return;
    }

    ifstream pIn("Products.txt");
    ofstream pOut("Products_Temp.txt");
    int found = 0;

    if (pIn.is_open() && pOut.is_open()) {
        char line2[300];
        while (pIn.getline(line2, 300)) {
            char ppid[20];
            int idx = 0;
            int i = 0;
            while (line2[i] != '|' && line2[i] != '\0') { ppid[idx] = line2[i]; idx++; i++; }
            ppid[idx] = '\0';

            if (compareStrings(ppid, editId) == 0) {
                pOut << ppid << "|" << newName << "|" << newPrice << "|" << newStock << "|" << newCat << "\n";
                found = 1;
            } else {
                pOut << line2 << "\n";
            }
        }
        pIn.close();
        pOut.close();
        remove("Products.txt");
        rename("Products_Temp.txt", "Products.txt");
    }

    if (found == 1) {
        cout << "Product updated successfully!\n";
        ofstream actLog("ActivityLogs.txt", ios::app);
        if (actLog.is_open()) {
            char ts[20];
            getCurrentTimestamp(ts);
            actLog << empUsername << "|EDIT_PRODUCT|" << editId << "|" << ts << "\n";
            actLog.close();
        }
    } else {
        cout << "Product ID not found.\n";
    }
}

// ============================================================
// DELETE PRODUCT
// ============================================================
void deleteProduct(const char* empUsername) {
    char delId[20];
    cout << "\n===== DELETE PRODUCT =====\n";
    cout << "Enter Product ID to delete: ";
    cin >> delId;

    ifstream pIn("Products.txt");
    ofstream pOut("Products_Temp.txt");
    int found = 0;

    if (pIn.is_open() && pOut.is_open()) {
        char line[300];
        while (pIn.getline(line, 300)) {
            char ppid[20];
            int idx = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') { ppid[idx] = line[i]; idx++; i++; }
            ppid[idx] = '\0';

            if (compareStrings(ppid, delId) == 0) {
                found = 1; // Is line ko skip karo (delete ho gaya)
            } else {
                pOut << line << "\n";
            }
        }
        pIn.close();
        pOut.close();
        remove("Products.txt");
        rename("Products_Temp.txt", "Products.txt");
    }

    if (found == 1) {
        cout << "Product deleted!\n";
        ofstream actLog("ActivityLogs.txt", ios::app);
        if (actLog.is_open()) {
            char ts[20];
            getCurrentTimestamp(ts);
            actLog << empUsername << "|DELETE_PRODUCT|" << delId << "|" << ts << "\n";
            actLog.close();
        }
    } else {
        cout << "Product ID not found.\n";
    }
}

// ============================================================
// LOW STOCK ALERTS
// ============================================================
void viewLowStockAlerts() {
    int threshold = 5; // 5 se kam stock = alert

    cout << "\n===== LOW STOCK ALERTS (Stock < " << threshold << ") =====\n";

    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "No products found.\n";
        return;
    }

    char line[300];
    int count = 0;

    while (prodFile.getline(line, 300)) {
        char pid[20];
        char pname[50];
        char pprice[20];
        char pstock[10];
        char pcat[30];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { pid[idx] = line[i]; idx++; i++; }
        pid[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pname[idx] = line[i]; idx++; i++; }
        pname[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pprice[idx] = line[i]; idx++; i++; }
        pprice[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pstock[idx] = line[i]; idx++; i++; }
        pstock[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { pcat[idx] = line[i]; idx++; i++; }
        pcat[idx] = '\0';

        int stock = stringToInt(pstock);
        if (stock < threshold) {
            cout << "[!] LOW STOCK: " << pname << " (ID: " << pid
                 << ") | Current Stock: " << pstock << "\n";
            count++;
        }
    }
    prodFile.close();

    if (count == 0) cout << "All products have sufficient stock.\n";
}

// ============================================================
// VIEW SALES REPORT
// ============================================================
void viewSalesReport() {
    cout << "\n===== SALES REPORT =====\n";

    ifstream ordersFile("Orders.txt");
    if (!ordersFile.is_open()) {
        cout << "No sales data found.\n";
        return;
    }

    cout << "Customer | Product          | Price     | Qty | Date\n";
    cout << "--------------------------------------------------------------\n";

    double totalRevenue = 0;
    int orderCount = 0;
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
        totalRevenue = totalRevenue + (price * q);
        orderCount++;

        cout << user << " | " << pname << " | Rs." << pprice
             << " | " << qty << " | " << ts << "\n";
    }
    ordersFile.close();

    char revStr[20];
    doubleToString(totalRevenue, revStr, 2);
    cout << "--------------------------------------------------------------\n";
    cout << "Total Orders: " << orderCount << " | Total Revenue: Rs." << revStr << "\n";
}

// ============================================================
// RESPOND TO FEEDBACK
// ============================================================
void respondToFeedback() {
    cout << "\n===== CUSTOMER FEEDBACK =====\n";

    ifstream fbFile("Feedback.txt");
    if (!fbFile.is_open()) {
        cout << "No feedback found.\n";
        return;
    }

    char line[400];
    int count = 0;

    while (fbFile.getline(line, 400)) {
        char user[50];
        char pid[20];
        char fb[300];
        char ts[20];
        char status[20];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { user[idx] = line[i]; idx++; i++; }
        user[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pid[idx] = line[i]; idx++; i++; }
        pid[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { fb[idx] = line[i]; idx++; i++; }
        fb[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { ts[idx] = line[i]; idx++; i++; }
        ts[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { status[idx] = line[i]; idx++; i++; }
        status[idx] = '\0';

        count++;
        cout << count << ". [" << status << "] By: " << user
             << " | Product: " << pid << " | Feedback: " << fb << "\n";
    }
    fbFile.close();

    if (count == 0) {
        cout << "No feedback to respond to.\n";
        return;
    }

    cout << "\nEnter feedback number to respond to (0 to go back): ";
    int fbNum;
    cin >> fbNum;

    if (cin.fail() || fbNum <= 0 || fbNum > count) {
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    char response[300];
    cout << "Enter your response: ";
    cin.ignore();
    cin.getline(response, 300);

    ifstream fIn("Feedback.txt");
    ofstream fOut("Feedback_Temp.txt");

    if (fIn.is_open() && fOut.is_open()) {
        char line2[400];
        int lineNum = 0;

        while (fIn.getline(line2, 400)) {
            lineNum++;
            if (lineNum == fbNum) {
                char user2[50];
                char pid2[20];
                char fb2[300];
                char ts2[20];
                int idx2 = 0;
                int i2 = 0;

                while (line2[i2] != '|' && line2[i2] != '\0') { user2[idx2] = line2[i2]; idx2++; i2++; }
                user2[idx2] = '\0'; i2++;
                idx2 = 0;
                while (line2[i2] != '|' && line2[i2] != '\0') { pid2[idx2] = line2[i2]; idx2++; i2++; }
                pid2[idx2] = '\0'; i2++;
                idx2 = 0;
                while (line2[i2] != '|' && line2[i2] != '\0') { fb2[idx2] = line2[i2]; idx2++; i2++; }
                fb2[idx2] = '\0'; i2++;
                idx2 = 0;
                while (line2[i2] != '|' && line2[i2] != '\0') { ts2[idx2] = line2[i2]; idx2++; i2++; }
                ts2[idx2] = '\0';

                fOut << user2 << "|" << pid2 << "|" << fb2 << "|" << ts2 << "|RESPONDED|" << response << "\n";
                cout << "Response submitted!\n";
            } else {
                fOut << line2 << "\n";
            }
        }
        fIn.close();
        fOut.close();
        remove("Feedback.txt");
        rename("Feedback_Temp.txt", "Feedback.txt");
    }
}

// ============================================================
// SEND ANNOUNCEMENT
// ============================================================
void sendAnnouncement(const char* empUsername) {
    char announcement[300];

    cout << "\n===== SEND ANNOUNCEMENT =====\n";
    cout << "Enter announcement text: ";
    cin.ignore();
    cin.getline(announcement, 300);

    ofstream annFile("Announcements.txt", ios::app);
    if (annFile.is_open()) {
        char ts[20];
        getCurrentTimestamp(ts);
        annFile << "[" << ts << "] " << empUsername << ": " << announcement << "\n";
        annFile.close();
        cout << "Announcement published!\n";
    } else {
        cout << "Error saving announcement.\n";
    }
}

// ============================================================
// EMPLOYEE MENU
// ============================================================
void employeeMenu(const char* currentUser) {
    int choice = 0;

    while (1) {
        cout << "\n===== EMPLOYEE MENU =====\n";
        cout << "1. Manage Support Requests\n";
        cout << "2. Add Product\n";
        cout << "3. Edit Product\n";
        cout << "4. Delete Product\n";
        cout << "5. View Low Stock Alerts\n";
        cout << "6. View Sales Report\n";
        cout << "7. Respond to Customer Feedback\n";
        cout << "8. Send Announcement\n";
        cout << "9. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        if (choice == 1)      manageSupportRequests();
        else if (choice == 2) addProduct(currentUser);
        else if (choice == 3) editProduct(currentUser);
        else if (choice == 4) deleteProduct(currentUser);
        else if (choice == 5) viewLowStockAlerts();
        else if (choice == 6) viewSalesReport();
        else if (choice == 7) respondToFeedback();
        else if (choice == 8) sendAnnouncement(currentUser);
        else if (choice == 9) {
            cout << "Logged out.\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
