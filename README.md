# 🍽️ Online Food Monitoring System

A terminal-based **C project** for managing an online food ordering system. Built using **modular code**, this project supports different user roles (Admin, Staff, Customer), with functionality to manage inventory, process orders, and collect customer feedback.

---

## 📌 Features

### 👤 User Management
- User Registration (Admin / Staff / Customer)
- User Login
- Role-based menu access

### 🛒 Inventory Management
- Add new food items
- Update stock quantity
- View full inventory with low-stock alerts

### 📦 Order Processing
- Customers can place orders
- Staff can view and update order statuses
- Orders are saved with status tracking (Preparing, Delivered, etc.)

### 💬 Customer Feedback
- Customers can give feedback
- Admins and Staff can view all feedback

### 💾 Data Persistence
All data is stored in `.txt` files for persistence:
- `users.txt`
- `inventory.txt`
- `orders.txt`
- `feedback.txt`

---

## 💻 Technologies Used

- **C Language**
- **Modular Code Structure**
- **ANSI Escape Codes** for colorful terminal output
- **VS Code** + **WSL2 (Ubuntu)** for development

---
