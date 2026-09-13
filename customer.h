#ifndef CUSTOMER_H
#define CUSTOMER_H

// ==========================================
// customer.h - Customer ke functions ke declarations
// Koi class nahi, koi struct nahi, sirf functions
// ==========================================

// --- Helper Functions ---
void copyString(char* dest, const char* src);
int compareStrings(const char* a, const char* b);
int stringLength(const char* s);
void intToString(int num, char* buffer);
void doubleToString(double num, char* buffer, int decimals);
int stringToInt(const char* s);
double stringToDouble(const char* s);
void getCurrentTimestamp(char* buffer);

// --- Security Functions ---
void encryptPassword(const char* plainText, char* encrypted, int shift);
void decryptPassword(const char* encrypted, char* decrypted, int shift);
void logSuspiciousActivity(const char* username, int attempts);
void checkFraud(const char* userId, int quantity, double amount);

// --- Customer Functions ---
void registerCustomer();
int loginCustomer(char* loggedInUser, char* loggedInName);
void browseProducts();
void searchProducts();
void addToCart(const char* currentUser);
void viewCart(const char* currentUser);
void removeFromCart(const char* currentUser);
void checkout(const char* currentUser);
void addToWishlist(const char* currentUser);
void viewWishlist(const char* currentUser);
void viewOrderHistory(const char* currentUser);
void giveFeedback(const char* currentUser);
void submitSupportRequest(const char* currentUser);
void viewAnnouncements();
void customerMenu(const char* currentUser);

#endif
