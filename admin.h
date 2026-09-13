#ifndef ADMIN_H
#define ADMIN_H

// ==========================================
// admin.h - Admin ke functions ke declarations
// Koi class nahi, koi struct nahi, sirf functions
// ==========================================

int loginAdmin(char* loggedInUser, char* loggedInName);
void generateOTP(const char* adminUser);
int verifyOTP(const char* adminUser);
void addUser();
void modifyUser();
void removeUser();
void viewAllUsers();
void bulkImportProducts();
void bulkExportProducts();
void createDiscount();
void removeDiscount();
void viewDiscounts();
void viewActivityLogs();
void viewSuspiciousActivity();
void viewFraudReport();
void viewSalesAndRevenue();
void logAuditTrail(const char* adminUser, const char* action);
void viewAuditTrail();
void adminMenu(const char* currentUser);

#endif
