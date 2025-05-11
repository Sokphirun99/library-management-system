# Library Management System

A comprehensive command-line application for managing library resources, students, and transactions.

## Features

### Book Management
- Add new books with details (title, author, ISBN)
- View all books with availability status
- Search books by title, author, or ISBN
- Update book information
- Delete books (when not currently borrowed)

### Student Management
- Register new students
- View all registered students

### Transaction Management
- Borrow books (links student ID with book ID)
- Return books
- View complete transaction history with book and student details

### System Features
- Operation history logging with timestamps
- Data persistence using text files
- Simple and intuitive console interface

## Project Structure

The system uses the following data structures:
- `Book`: Contains book ID, title, author, ISBN, and availability status
- `Student`: Contains student ID and name
- `Transaction`: Records borrowing and returning operations

## Data Storage

All data is stored persistently in text files:
- `books.txt`: Stores book information
- `students.txt`: Stores student information
- `transactions.txt`: Records borrowing and returning transactions
- `operation_history.txt`: Logs all system operations

## Getting Started

### Prerequisites
- C++ compiler (C++11 or higher)
- Standard libraries (iostream, fstream, vector, string, etc.)

### Compilation
```bash
g++ -std=c++11 main.cpp -o library_system
```

### Running the Application
```bash
./library_system
```

## Usage Guide

The system provides a user-friendly menu:

1. Add Book - Register a new book with details
2. Display Books - View all books in the library
3. Search Books - Find books by title, author, or ISBN
4. Update Book - Modify book details
5. Delete Book - Remove a book from the library
6. Add Student - Register a new student
7. Display Students - View all registered students
8. Borrow Book - Issue a book to a student
9. Return Book - Process returned books
10. Display Transactions - View borrowing/returning history
11. Display Operation History - View system activity log
0. Exit - Save all data and close the application

## Implementation Details

The system implements various functions for data management:
- CRUD operations (Create, Read, Update, Delete) for books and students
- Transaction recording for borrowing and returning
- Data validation to prevent errors
- File I/O operations for persistent storage
- User interface for easy interaction

## Author

Phirun

## License

This project is available for educational purposes.