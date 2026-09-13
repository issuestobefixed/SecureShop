#ifndef EMPLOYEE_H
#define EMPLOYEE_H

// ==========================================
// employee.h - Employee ke functions ke declarations
// Koi class nahi, koi struct nahi, sirf functions
// ==========================================

int loginEmployee(char* loggedInUser, char* loggedInName);
void manageSupportRequests();
void addProduct(const char* empUsername);
void editProduct(const char* empUsername);
void deleteProduct(const char* empUsername);
void viewLowStockAlerts();
void viewSalesReport();
void respondToFeedback();
void sendAnnouncement(const char* empUsername);
void employeeMenu(const char* currentUser);

#endif
