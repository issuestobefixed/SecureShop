# SecureShop

A console-based shopping platform built in C++ as a Programming Fundamentals course project. SecureShop simulates an e-commerce system with three roles **Customer**, **Employee**, and **Admin** each with their own menu and permissions, using only core C++ (no classes/structs, no external libraries) and flat-file storage for persistence.

## Features

**Customer**
- Registration and login (with password encryption)
- Browse and search products
- Cart management (add/view/remove)
- Checkout, order history
- Wishlist
- Feedback and support requests
- View announcements

**Employee**
- Login
- Manage support requests and respond to feedback
- Add / edit / delete products
- Low-stock alerts and sales reports
- Send announcements

**Admin**
- Login with OTP verification
- User management (add/modify/remove/view)
- Bulk import/export of products
- Discount management
- Activity logs, suspicious activity monitoring, fraud reports
- Sales and revenue reports
- Audit trail logging and viewing

**Security**
- Caesar-cipher based password encryption/decryption
- Suspicious login activity logging
- Basic fraud checks on orders

## Project Structure

```
SecureShop/
├── main.cpp            # Entry point, main menu
├── customer.cpp/.h     # Customer features + shared helper/security functions
├── employee.cpp/.h     # Employee features
├── admin.cpp/.h        # Admin features
└── Products.txt        # Seed product data
```

Data files such as `Users.txt`, orders, cart, wishlist, etc. are generated at runtime and are not tracked in this repo (see `.gitignore`).

## Building

```bash
g++ main.cpp customer.cpp employee.cpp admin.cpp -o secureshop
./secureshop
```

On first run, a default admin account is created:
- **Username:** `admin`
- **Password:** `admin123`

## Notes

This was built as a learning project to practice core C++ concepts (functions, file I/O, string handling, control flow) without OOP constructs, per course requirements.
