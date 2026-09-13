// ==========================================
// customer.cpp
// Customer ke sare functions yahan hain
// Sirf basic C++: variables, loops, if/else, file handling
// Koi class nahi, koi struct nahi, koi banned library nahi
// ==========================================

#include <iostream>
#include <fstream>
#include "customer.h"

using namespace std;

// ============================================================
// HELPER FUNCTIONS
// ============================================================

// Ek string ko doosri mein copy karna
void copyString(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Do strings ko compare karna
// 0 return hoga agar dono same hain
int compareStrings(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i++;
    }
    return a[i] - b[i];
}

// String ki length nikalna
int stringLength(const char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Integer ko string mein convert karna
void intToString(int num, char* buffer) {
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    char temp[20];
    int i = 0;
    int isNeg = 0;
    if (num < 0) {
        isNeg = 1;
        num = -num;
    }
    while (num > 0) {
        temp[i] = '0' + (num % 10);
        i++;
        num = num / 10;
    }
    int start = 0;
    if (isNeg) {
        buffer[0] = '-';
        start = 1;
    }
    int j = 0;
    while (i > 0) {
        i--;
        buffer[start] = temp[i];
        start++;
    }
    buffer[start] = '\0';
}

// Double ko string mein convert karna
void doubleToString(double num, char* buffer, int decimals) {
    int intPart = (int)num;
    double fracPart = num - intPart;
    if (fracPart < 0) fracPart = -fracPart;

    char intBuf[20];
    intToString(intPart, intBuf);
    copyString(buffer, intBuf);

    int len = stringLength(buffer);
    buffer[len] = '.';
    len++;

    int i = 0;
    while (i < decimals) {
        fracPart = fracPart * 10;
        int digit = (int)fracPart;
        buffer[len] = '0' + digit;
        len++;
        fracPart = fracPart - digit;
        i++;
    }
    buffer[len] = '\0';
}

// String ko integer mein convert karna
int stringToInt(const char* s) {
    int result = 0;
    int i = 0;
    int neg = 0;
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    }
    while (s[i] >= '0' && s[i] <= '9') {
        result = result * 10 + (s[i] - '0');
        i++;
    }
    if (neg == 1) return -result;
    return result;
}

// String ko double mein convert karna
double stringToDouble(const char* s) {
    double result = 0;
    double frac = 0;
    int fracDigits = 0;
    int inFrac = 0;
    int i = 0;
    int neg = 0;
    if (s[0] == '-') {
        neg = 1;
        i = 1;
    }
    while (s[i] != '\0') {
        if (s[i] == '.') {
            inFrac = 1;
        } else if (s[i] >= '0' && s[i] <= '9') {
            if (inFrac == 0) {
                result = result * 10 + (s[i] - '0');
            } else {
                frac = frac * 10 + (s[i] - '0');
                fracDigits++;
            }
        }
        i++;
    }
    double divisor = 1;
    int j = 0;
    while (j < fracDigits) {
        divisor = divisor * 10;
        j++;
    }
    result = result + frac / divisor;
    if (neg == 1) return -result;
    return result;
}

// Simple date placeholder
// (ctime nahi use kar sakte kyunki restricted hai)
void getCurrentTimestamp(char* buffer) {
    copyString(buffer, "2026-05-09");
}

// ============================================================
// SECURITY FUNCTIONS
// ============================================================

// Caesar cipher se password encrypt karna
// Har letter ko shift ke barabar aage karo
void encryptPassword(const char* plainText, char* encrypted, int shift) {
    int i = 0;
    while (plainText[i] != '\0') {
        char c = plainText[i];
        if (c >= 'a' && c <= 'z') {
            encrypted[i] = (char)(((c - 'a' + shift) % 26) + 'a');
        } else if (c >= 'A' && c <= 'Z') {
            encrypted[i] = (char)(((c - 'A' + shift) % 26) + 'A');
        } else if (c >= '0' && c <= '9') {
            encrypted[i] = (char)(((c - '0' + shift) % 10) + '0');
        } else {
            encrypted[i] = c;
        }
        i++;
    }
    encrypted[i] = '\0';
}

// Caesar cipher se password decrypt karna
void decryptPassword(const char* encrypted, char* decrypted, int shift) {
    int i = 0;
    while (encrypted[i] != '\0') {
        char c = encrypted[i];
        if (c >= 'a' && c <= 'z') {
            decrypted[i] = (char)(((c - 'a' - shift + 26) % 26) + 'a');
        } else if (c >= 'A' && c <= 'Z') {
            decrypted[i] = (char)(((c - 'A' - shift + 26) % 26) + 'A');
        } else if (c >= '0' && c <= '9') {
            decrypted[i] = (char)(((c - '0' - shift + 10) % 10) + '0');
        } else {
            decrypted[i] = c;
        }
        i++;
    }
    decrypted[i] = '\0';
}

// 3 baar galat password - SuspiciousActivity.txt mein log karo
void logSuspiciousActivity(const char* username, int attempts) {
    ofstream logFile("SuspiciousActivity.txt", ios::app);
    if (logFile.is_open()) {
        char timestamp[20];
        getCurrentTimestamp(timestamp);
        // Format: username|attempts|timestamp
        logFile << username << "|" << attempts << "|" << timestamp << "\n";
        logFile.close();
    }
}

// Fraud check: bara amount ya zyada quantity
void checkFraud(const char* userId, int quantity, double amount) {
    int isFraud = 0;
    char reason[100];

    if (amount > 50000.0) {
        isFraud = 1;
        copyString(reason, "Amount exceeds Rs.50000");
    }
    if (quantity > 5) {
        isFraud = 1;
        copyString(reason, "Quantity exceeds 5 items");
    }

    if (isFraud == 1) {
        ofstream fraudFile("FraudReport.txt", ios::app);
        if (fraudFile.is_open()) {
            char timestamp[20];
            getCurrentTimestamp(timestamp);
            char amtStr[20];
            doubleToString(amount, amtStr, 2);
            char qtyStr[10];
            intToString(quantity, qtyStr);
            // Format: userId|quantity|amount|timestamp|reason
            fraudFile << userId << "|" << qtyStr << "|" << amtStr << "|" << timestamp << "|" << reason << "\n";
            fraudFile.close();
        }
        cout << "\n[!] Suspicious transaction flagged. Logged in FraudReport.txt\n";
    }
}

// ============================================================
// REGISTER CUSTOMER
// ============================================================
void registerCustomer() {
    char newUser[50];
    char newPass[100];
    char newName[100];
    char encPass[100];

    cout << "\n===== CUSTOMER REGISTRATION =====\n";

    cout << "Enter full name: ";
    cin.ignore();
    cin.getline(newName, 100);

    cout << "Enter username (no spaces): ";
    cin >> newUser;

    // Pehle check karo ke username exist karta hai ya nahi
    ifstream checkFile("Users.txt");
    if (checkFile.is_open()) {
        char line[300];
        while (checkFile.getline(line, 300)) {
            // Format: username|password|name|role
            char storedUser[50];
            int j = 0;
            int i = 0;
            while (line[i] != '|' && line[i] != '\0') {
                storedUser[j] = line[i];
                j++;
                i++;
            }
            storedUser[j] = '\0';

            if (compareStrings(storedUser, newUser) == 0) {
                cout << "Username already exists! Please choose another.\n";
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }

    cout << "Enter password: ";
    cin >> newPass;

    // Password ko Caesar cipher se encrypt karo (shift = 3)
    encryptPassword(newPass, encPass, 3);

    // Users.txt mein save karo
    ofstream userFile("Users.txt", ios::app);
    if (userFile.is_open()) {
        userFile << newUser << "|" << encPass << "|" << newName << "|customer\n";
        userFile.close();
        cout << "Registration successful! You can now login.\n";
    } else {
        cout << "Error: Could not save user data.\n";
    }
}

// ============================================================
// LOGIN CUSTOMER
// ============================================================
int loginCustomer(char* loggedInUser, char* loggedInName) {
    char inputUser[50];
    char inputPass[100];
    char decryptedPass[100];
    int failedAttempts = 0;

    cout << "\n===== CUSTOMER LOGIN =====\n";
    cout << "Enter username: ";
    cin >> inputUser;

    // 3 attempts milte hain, phir lockout
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

            // username nikalo
            while (line[i] != '|' && line[i] != '\0') {
                storedUser[idx] = line[i];
                idx++;
                i++;
            }
            storedUser[idx] = '\0';
            i++; // | skip

            // password nikalo
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') {
                storedPass[idx] = line[i];
                idx++;
                i++;
            }
            storedPass[idx] = '\0';
            i++;

            // name nikalo
            idx = 0;
            while (line[i] != '|' && line[i] != '\0') {
                storedName[idx] = line[i];
                idx++;
                i++;
            }
            storedName[idx] = '\0';
            i++;

            // role nikalo
            idx = 0;
            while (line[i] != '\0') {
                storedRole[idx] = line[i];
                idx++;
                i++;
            }
            storedRole[idx] = '\0';

            if (compareStrings(storedUser, inputUser) == 0 &&
                compareStrings(storedRole, "customer") == 0) {

                decryptPassword(storedPass, decryptedPass, 3);

                if (compareStrings(decryptedPass, inputPass) == 0) {
                    copyString(loggedInUser, storedUser);
                    copyString(loggedInName, storedName);
                    found = 1;
                    userFile.close();

                    // Activity log mein login record karo
                    ofstream actLog("ActivityLogs.txt", ios::app);
                    if (actLog.is_open()) {
                        char ts[20];
                        getCurrentTimestamp(ts);
                        actLog << inputUser << "|LOGIN_SUCCESS|customer|" << ts << "\n";
                        actLog.close();
                    }

                    cout << "\nLogin successful! Welcome, " << storedName << "!\n";
                    return 1;
                }
            }
        }
        userFile.close();

        if (found == 0) {
            failedAttempts++;
            cout << "Wrong username or password. Attempts left: " << (3 - failedAttempts) << "\n";

            if (failedAttempts >= 3) {
                // Suspicious activity log karo aur program band karo
                logSuspiciousActivity(inputUser, failedAttempts);
                cout << "\n[!] Too many failed attempts! Event logged. Exiting for security.\n";
                exit(0);
            }
        }
    }
    return 0;
}

// ============================================================
// BROWSE PRODUCTS
// ============================================================
void browseProducts() {
    cout << "\n===== PRODUCT CATALOG =====\n";
    cout << "--------------------------------------------------------------\n";
    cout << "ID     | Name                | Price(Rs.) | Stock | Category\n";
    cout << "--------------------------------------------------------------\n";

    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "No products available at the moment.\n";
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

        cout << pid << " | " << pname << " | Rs." << pprice
             << " | " << pstock << " | " << pcat << "\n";
        count++;
    }
    prodFile.close();

    if (count == 0) cout << "No products found.\n";
    cout << "--------------------------------------------------------------\n";
}

// ============================================================
// SEARCH PRODUCTS
// ============================================================
void searchProducts() {
    cout << "\n===== SEARCH PRODUCTS =====\n";
    cout << "1. Search by Category\n";
    cout << "2. Search by Price Range\n";
    cout << "3. Show In-Stock Products Only\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input.\n";
        return;
    }

    char searchCat[30];
    double minPrice = 0;
    double maxPrice = 0;

    if (choice == 1) {
        cout << "Enter category name: ";
        cin.ignore();
        cin.getline(searchCat, 30);
    } else if (choice == 2) {
        cout << "Enter minimum price: ";
        cin >> minPrice;
        cout << "Enter maximum price: ";
        cin >> maxPrice;
        if (minPrice < 0 || maxPrice < minPrice) {
            cout << "Invalid price range.\n";
            return;
        }
    } else if (choice == 3) {
        // sirf in-stock dikhana hai
    } else {
        cout << "Invalid choice.\n";
        return;
    }

    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "No products available.\n";
        return;
    }

    cout << "\n--- Search Results ---\n";
    cout << "--------------------------------------------------------------\n";

    char line[300];
    int found = 0;

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

        double price = stringToDouble(pprice);
        int stock = stringToInt(pstock);

        int show = 0;
        if (choice == 1 && compareStrings(pcat, searchCat) == 0) show = 1;
        else if (choice == 2 && price >= minPrice && price <= maxPrice) show = 1;
        else if (choice == 3 && stock > 0) show = 1;

        if (show == 1) {
            cout << pid << " | " << pname << " | Rs." << pprice
                 << " | " << pstock << " | " << pcat << "\n";
            found++;
        }
    }
    prodFile.close();

    if (found == 0) cout << "No matching products found.\n";
    cout << "--------------------------------------------------------------\n";
}

// ============================================================
// CART FILE NAME BANANA
// Har user ka alag cart file hoga
// ============================================================
void buildCartFileName(const char* currentUser, char* cartFileName) {
    copyString(cartFileName, "Cart_");
    int len = stringLength(cartFileName);
    int i = 0;
    while (currentUser[i] != '\0') {
        cartFileName[len] = currentUser[i];
        len++;
        i++;
    }
    cartFileName[len] = '.'; len++;
    cartFileName[len] = 't'; len++;
    cartFileName[len] = 'x'; len++;
    cartFileName[len] = 't'; len++;
    cartFileName[len] = '\0';
}

// ============================================================
// ADD TO CART
// ============================================================
void addToCart(const char* currentUser) {
    browseProducts();

    char productId[20];
    int qty;

    cout << "\nEnter Product ID to add to cart: ";
    cin >> productId;
    cout << "Enter quantity: ";
    cin >> qty;

    if (cin.fail() || qty <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid quantity. Must be greater than 0.\n";
        return;
    }

    // Product Products.txt se dhundho
    ifstream prodFile("Products.txt");
    if (!prodFile.is_open()) {
        cout << "Product database unavailable.\n";
        return;
    }

    char line[300];
    int found = 0;
    char pname[50];
    char pprice[20];

    while (prodFile.getline(line, 300)) {
        char pid[20];
        char pn[50];
        char pp[20];
        char ps[10];
        char pc[30];
        int idx = 0;
        int i = 0;

        while (line[i] != '|' && line[i] != '\0') { pid[idx] = line[i]; idx++; i++; }
        pid[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pn[idx] = line[i]; idx++; i++; }
        pn[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { pp[idx] = line[i]; idx++; i++; }
        pp[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '|' && line[i] != '\0') { ps[idx] = line[i]; idx++; i++; }
        ps[idx] = '\0'; i++;
        idx = 0;
        while (line[i] != '\0') { pc[idx] = line[i]; idx++; i++; }
        pc[idx] = '\0';

        if (compareStrings(pid, productId) == 0) {
            int stock = stringToInt(ps);
            if (stock < qty) {
                cout << "Not enough stock! Available: " << ps << "\n";
                prodFile.close();
                return;
            }
            copyString(pname, pn);
            copyString(pprice, pp);
            found = 1;
            break;
        }
    }
    prodFile.close();

    if (found == 0) {
        cout << "Product ID not found.\n";
        return;
    }

    // Cart file mein likhna
    char cartFileName[100];
    buildCartFileName(currentUser, cartFileName);

    ofstream cart(cartFileName, ios::app);
    if (cart.is_open()) {
        char qtyStr[10];
        intToString(qty, qtyStr);
        // Format: username|productId|productName|price|quantity
        cart << currentUser << "|" << productId << "|" << pname << "|" << pprice << "|" << qtyStr << "\n";
        cart.close();
        cout << "Item added to cart successfully!\n";
    } else {
        cout << "Error: Could not update cart.\n";
    }
}

// ============================================================
// VIEW CART
// ============================================================
void viewCart(const char* currentUser) {
    char cartFileName[100];
    buildCartFileName(currentUser, cartFileName);

    ifstream cart(cartFileName);
    if (!cart.is_open()) {
        cout << "\nYour cart is empty.\n";
        return;
    }

    cout << "\n===== YOUR CART =====\n";
    cout << "--------------------------------------------------\n";
    cout << "ProductID | Name              | Price     | Qty\n";
    cout << "--------------------------------------------------\n";

    double total = 0;
    char line[300];
    int count = 0;

    while (cart.getline(line, 300)) {
        char user[50];
        char pid[20];
        char pname[50];
        char pprice[20];
        char qty[10];
        int idx = 0;
        int j = 0;

        while (line[j] != '|' && line[j] != '\0') { user[idx] = line[j]; idx++; j++; }
        user[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pid[idx] = line[j]; idx++; j++; }
        pid[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pname[idx] = line[j]; idx++; j++; }
        pname[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pprice[idx] = line[j]; idx++; j++; }
        pprice[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '\0') { qty[idx] = line[j]; idx++; j++; }
        qty[idx] = '\0';

        double price = stringToDouble(pprice);
        int q = stringToInt(qty);
        total = total + (price * q);

        cout << pid << " | " << pname << " | Rs." << pprice << " | " << qty << "\n";
        count++;
    }
    cart.close();

    if (count == 0) {
        cout << "Your cart is empty.\n";
    } else {
        char totalStr[20];
        doubleToString(total, totalStr, 2);
        cout << "--------------------------------------------------\n";
        cout << "TOTAL: Rs." << totalStr << "\n";
    }
}

// ============================================================
// REMOVE FROM CART
// ============================================================
void removeFromCart(const char* currentUser) {
    viewCart(currentUser);

    char removePid[20];
    cout << "\nEnter Product ID to remove from cart: ";
    cin >> removePid;

    char cartFileName[100];
    buildCartFileName(currentUser, cartFileName);

    ifstream cart(cartFileName);
    if (!cart.is_open()) {
        cout << "Cart is already empty.\n";
        return;
    }

    ofstream temp("Cart_Temp.txt");
    char line[300];
    int removed = 0;

    while (cart.getline(line, 300)) {
        char user[50];
        char pid[20];
        int idx = 0;
        int j = 0;

        while (line[j] != '|' && line[j] != '\0') { user[idx] = line[j]; idx++; j++; }
        user[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pid[idx] = line[j]; idx++; j++; }
        pid[idx] = '\0';

        if (compareStrings(pid, removePid) == 0 && removed == 0) {
            removed = 1; // Sirf pehli matching entry hatao
        } else {
            temp << line << "\n";
        }
    }
    cart.close();
    temp.close();

    remove(cartFileName);
    rename("Cart_Temp.txt", cartFileName);

    if (removed == 1) cout << "Item removed from cart.\n";
    else cout << "Product ID not found in cart.\n";
}

// ============================================================
// CHECKOUT
// ============================================================
void checkout(const char* currentUser) {
    viewCart(currentUser);

    char cartFileName[100];
    buildCartFileName(currentUser, cartFileName);

    ifstream cart(cartFileName);
    if (!cart.is_open()) {
        cout << "Your cart is empty. Nothing to checkout.\n";
        return;
    }

    // Cart items arrays mein load karo (max 50 items)
    char pids[50][20];
    char pnames[50][50];
    char pprices[50][20];
    char qtys[50][10];
    int itemCount = 0;
    double grandTotal = 0;
    char line[300];

    while (cart.getline(line, 300) && itemCount < 50) {
        char user[50];
        int idx = 0;
        int j = 0;

        while (line[j] != '|' && line[j] != '\0') { user[idx] = line[j]; idx++; j++; }
        user[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pids[itemCount][idx] = line[j]; idx++; j++; }
        pids[itemCount][idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pnames[itemCount][idx] = line[j]; idx++; j++; }
        pnames[itemCount][idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pprices[itemCount][idx] = line[j]; idx++; j++; }
        pprices[itemCount][idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '\0') { qtys[itemCount][idx] = line[j]; idx++; j++; }
        qtys[itemCount][idx] = '\0';

        double price = stringToDouble(pprices[itemCount]);
        int qty = stringToInt(qtys[itemCount]);
        grandTotal = grandTotal + (price * qty);
        itemCount++;
    }
    cart.close();

    if (itemCount == 0) {
        cout << "Cart is empty.\n";
        return;
    }

    // Discount code check karo
    double discountAmt = 0;
    char discountCode[30];
    cout << "\nDo you have a discount code? (y/n): ";
    char yn;
    cin >> yn;

    if (yn == 'y' || yn == 'Y') {
        cout << "Enter discount code: ";
        cin >> discountCode;

        ifstream discFile("Discounts.txt");
        if (discFile.is_open()) {
            char dline[200];
            while (discFile.getline(dline, 200)) {
                char dcode[30];
                char dval[20];
                int idx = 0;
                int j = 0;

                while (dline[j] != '|' && dline[j] != '\0') { dcode[idx] = dline[j]; idx++; j++; }
                dcode[idx] = '\0'; j++;
                idx = 0;
                while (dline[j] != '\0') { dval[idx] = dline[j]; idx++; j++; }
                dval[idx] = '\0';

                if (compareStrings(dcode, discountCode) == 0) {
                    discountAmt = stringToDouble(dval);
                    cout << "Discount applied: Rs." << dval << "\n";
                    break;
                }
            }
            discFile.close();
        } else {
            cout << "No discounts available.\n";
        }
    }

    double finalTotal = grandTotal - discountAmt;
    if (finalTotal < 0) finalTotal = 0;

    char finalStr[20];
    doubleToString(finalTotal, finalStr, 2);
    cout << "Final Total (after discount): Rs." << finalStr << "\n";

    cout << "Confirm purchase? (y/n): ";
    cin >> yn;
    if (yn != 'y' && yn != 'Y') {
        cout << "Checkout cancelled.\n";
        return;
    }

    // Fraud check
    checkFraud(currentUser, itemCount, finalTotal);

    // Orders aur Receipts mein save karo
    char timestamp[20];
    getCurrentTimestamp(timestamp);

    ofstream ordersFile("Orders.txt", ios::app);
    ofstream receiptFile("Receipts.txt", ios::app);

    if (ordersFile.is_open() && receiptFile.is_open()) {
        receiptFile << "========== RECEIPT ==========\n";
        receiptFile << "Customer: " << currentUser << "\n";
        receiptFile << "Date: " << timestamp << "\n";
        receiptFile << "-----------------------------\n";

        int k = 0;
        while (k < itemCount) {
            // Orders file mein har item log karo
            ordersFile << currentUser << "|" << pids[k] << "|" << pnames[k]
                       << "|" << pprices[k] << "|" << qtys[k] << "|" << timestamp << "\n";

            // Receipt mein likho
            receiptFile << pnames[k] << " x" << qtys[k] << " @ Rs." << pprices[k] << "\n";

            // Stock update karo Products.txt mein
            ifstream prodIn("Products.txt");
            ofstream prodOut("Products_Temp.txt");

            if (prodIn.is_open() && prodOut.is_open()) {
                char pline[300];
                while (prodIn.getline(pline, 300)) {
                    char ppid[20];
                    char ppname[50];
                    char ppprice[20];
                    char ppstock[10];
                    char ppcat[30];
                    int idx2 = 0;
                    int j2 = 0;

                    while (pline[j2] != '|' && pline[j2] != '\0') { ppid[idx2] = pline[j2]; idx2++; j2++; }
                    ppid[idx2] = '\0'; j2++;
                    idx2 = 0;
                    while (pline[j2] != '|' && pline[j2] != '\0') { ppname[idx2] = pline[j2]; idx2++; j2++; }
                    ppname[idx2] = '\0'; j2++;
                    idx2 = 0;
                    while (pline[j2] != '|' && pline[j2] != '\0') { ppprice[idx2] = pline[j2]; idx2++; j2++; }
                    ppprice[idx2] = '\0'; j2++;
                    idx2 = 0;
                    while (pline[j2] != '|' && pline[j2] != '\0') { ppstock[idx2] = pline[j2]; idx2++; j2++; }
                    ppstock[idx2] = '\0'; j2++;
                    idx2 = 0;
                    while (pline[j2] != '\0') { ppcat[idx2] = pline[j2]; idx2++; j2++; }
                    ppcat[idx2] = '\0';

                    if (compareStrings(ppid, pids[k]) == 0) {
                        int newStock = stringToInt(ppstock) - stringToInt(qtys[k]);
                        if (newStock < 0) newStock = 0;
                        char newStockStr[10];
                        intToString(newStock, newStockStr);
                        prodOut << ppid << "|" << ppname << "|" << ppprice
                                << "|" << newStockStr << "|" << ppcat << "\n";
                    } else {
                        prodOut << pline << "\n";
                    }
                }
                prodIn.close();
                prodOut.close();
                remove("Products.txt");
                rename("Products_Temp.txt", "Products.txt");
            }
            k++;
        }

        char discStr[20];
        doubleToString(discountAmt, discStr, 2);
        receiptFile << "-----------------------------\n";
        receiptFile << "Discount: Rs." << discStr << "\n";
        receiptFile << "TOTAL PAID: Rs." << finalStr << "\n";
        receiptFile << "=============================\n\n";

        ordersFile.close();
        receiptFile.close();
    }

    // Cart saaf karo
    remove(cartFileName);

    cout << "\n=== Purchase Successful! ===\n";
    cout << "Receipt saved to Receipts.txt\n";
    cout << "Total Paid: Rs." << finalStr << "\n";
}

// ============================================================
// WISHLIST
// ============================================================
void buildWishlistFileName(const char* currentUser, char* wishFile) {
    copyString(wishFile, "Wishlist_");
    int len = stringLength(wishFile);
    int i = 0;
    while (currentUser[i] != '\0') {
        wishFile[len] = currentUser[i];
        len++;
        i++;
    }
    wishFile[len] = '.'; len++;
    wishFile[len] = 't'; len++;
    wishFile[len] = 'x'; len++;
    wishFile[len] = 't'; len++;
    wishFile[len] = '\0';
}

void addToWishlist(const char* currentUser) {
    browseProducts();
    char productId[20];
    cout << "\nEnter Product ID to add to wishlist: ";
    cin >> productId;

    char wishFile[100];
    buildWishlistFileName(currentUser, wishFile);

    ofstream wish(wishFile, ios::app);
    if (wish.is_open()) {
        // Format: username|productId
        wish << currentUser << "|" << productId << "\n";
        wish.close();
        cout << "Added to wishlist!\n";
    } else {
        cout << "Error saving to wishlist.\n";
    }
}

void viewWishlist(const char* currentUser) {
    char wishFile[100];
    buildWishlistFileName(currentUser, wishFile);

    ifstream wish(wishFile);
    if (!wish.is_open()) {
        cout << "Your wishlist is empty.\n";
        return;
    }

    cout << "\n===== YOUR WISHLIST =====\n";
    char line[200];
    int count = 0;

    while (wish.getline(line, 200)) {
        char user[50];
        char pid[20];
        int idx = 0;
        int j = 0;

        while (line[j] != '|' && line[j] != '\0') { user[idx] = line[j]; idx++; j++; }
        user[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '\0') { pid[idx] = line[j]; idx++; j++; }
        pid[idx] = '\0';

        cout << "Product ID: " << pid << "\n";
        count++;
    }
    wish.close();
    if (count == 0) cout << "Wishlist is empty.\n";
}

// ============================================================
// ORDER HISTORY
// ============================================================
void viewOrderHistory(const char* currentUser) {
    cout << "\n===== ORDER HISTORY =====\n";

    ifstream ordersFile("Orders.txt");
    if (!ordersFile.is_open()) {
        cout << "No orders found.\n";
        return;
    }

    char line[300];
    int count = 0;

    while (ordersFile.getline(line, 300)) {
        char user[50];
        char pid[20];
        char pname[50];
        char pprice[20];
        char qty[10];
        char timestamp[30];
        int idx = 0;
        int j = 0;

        while (line[j] != '|' && line[j] != '\0') { user[idx] = line[j]; idx++; j++; }
        user[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pid[idx] = line[j]; idx++; j++; }
        pid[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pname[idx] = line[j]; idx++; j++; }
        pname[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { pprice[idx] = line[j]; idx++; j++; }
        pprice[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '|' && line[j] != '\0') { qty[idx] = line[j]; idx++; j++; }
        qty[idx] = '\0'; j++;
        idx = 0;
        while (line[j] != '\0') { timestamp[idx] = line[j]; idx++; j++; }
        timestamp[idx] = '\0';

        if (compareStrings(user, currentUser) == 0) {
            cout << "Product: " << pname << " | Qty: " << qty
                 << " | Price: Rs." << pprice << " | Date: " << timestamp << "\n";
            count++;
        }
    }
    ordersFile.close();

    if (count == 0) cout << "No orders found for your account.\n";
}

// ============================================================
// FEEDBACK
// ============================================================
void giveFeedback(const char* currentUser) {
    char productId[20];
    char feedbackText[300];

    cout << "\n===== GIVE FEEDBACK =====\n";
    cout << "Enter Product ID: ";
    cin >> productId;
    cout << "Enter your feedback: ";
    cin.ignore();
    cin.getline(feedbackText, 300);

    ofstream feedbackFile("Feedback.txt", ios::app);
    if (feedbackFile.is_open()) {
        char timestamp[20];
        getCurrentTimestamp(timestamp);
        // Format: username|productId|feedback|timestamp|status
        feedbackFile << currentUser << "|" << productId << "|"
                     << feedbackText << "|" << timestamp << "|PENDING\n";
        feedbackFile.close();
        cout << "Feedback submitted!\n";
    } else {
        cout << "Error saving feedback.\n";
    }
}

// ============================================================
// SUPPORT REQUEST
// ============================================================
void submitSupportRequest(const char* currentUser) {
    char requestText[300];

    cout << "\n===== SUBMIT SUPPORT REQUEST =====\n";
    cout << "Describe your issue: ";
    cin.ignore();
    cin.getline(requestText, 300);

    ofstream supportFile("Support.txt", ios::app);
    if (supportFile.is_open()) {
        char timestamp[20];
        getCurrentTimestamp(timestamp);
        // Format: username|request|timestamp|status
        supportFile << currentUser << "|" << requestText << "|" << timestamp << "|PENDING\n";
        supportFile.close();
        cout << "Support request submitted! An employee will respond soon.\n";
    } else {
        cout << "Error submitting request.\n";
    }
}

// ============================================================
// ANNOUNCEMENTS
// ============================================================
void viewAnnouncements() {
    cout << "\n===== ANNOUNCEMENTS =====\n";

    ifstream annFile("Announcements.txt");
    if (!annFile.is_open()) {
        cout << "No announcements at this time.\n";
        return;
    }

    char line[300];
    int count = 0;
    while (annFile.getline(line, 300)) {
        cout << ">> " << line << "\n";
        count++;
    }
    annFile.close();

    if (count == 0) cout << "No announcements.\n";
}

// ============================================================
// CUSTOMER MENU
// ============================================================
void customerMenu(const char* currentUser) {
    // Login hone par announcements dikhao
    viewAnnouncements();

    int choice = 0;

    while (1) {
        cout << "\n===== CUSTOMER MENU =====\n";
        cout << "1.  Browse Products\n";
        cout << "2.  Search Products\n";
        cout << "3.  Add to Cart\n";
        cout << "4.  View Cart\n";
        cout << "5.  Remove from Cart\n";
        cout << "6.  Checkout\n";
        cout << "7.  Add to Wishlist\n";
        cout << "8.  View Wishlist\n";
        cout << "9.  Order History\n";
        cout << "10. Give Feedback\n";
        cout << "11. Submit Support Request\n";
        cout << "12. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1)       browseProducts();
        else if (choice == 2)  searchProducts();
        else if (choice == 3)  addToCart(currentUser);
        else if (choice == 4)  viewCart(currentUser);
        else if (choice == 5)  removeFromCart(currentUser);
        else if (choice == 6)  checkout(currentUser);
        else if (choice == 7)  addToWishlist(currentUser);
        else if (choice == 8)  viewWishlist(currentUser);
        else if (choice == 9)  viewOrderHistory(currentUser);
        else if (choice == 10) giveFeedback(currentUser);
        else if (choice == 11) submitSupportRequest(currentUser);
        else if (choice == 12) {
            cout << "Logged out successfully.\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
