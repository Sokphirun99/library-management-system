#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <limits>

// Structure to represent a Book
struct Book {
    int id;
    std::string title;
    std::string author;
    std::string isbn;
    bool available;
    
    // Constructor
    Book(int _id, std::string _title, std::string _author, std::string _isbn, bool _available = true)
        : id(_id), title(_title), author(_author), isbn(_isbn), available(_available) {}
};

// Structure to represent a Student
struct Student {
    int id;
    std::string name;
    
    // Constructor
    Student(int _id, std::string _name) : id(_id), name(_name) {}
};

// Structure to represent a Transaction
struct Transaction {
    int id;
    int bookId;
    int studentId;
    std::string type; // "borrow" or "return"
    std::string date;
    
    // Constructor
    Transaction(int _id, int _bookId, int _studentId, std::string _type)
        : id(_id), bookId(_bookId), studentId(_studentId), type(_type) {
        // Get current date/time
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        // Format date as YYYY-MM-DD
        date = std::to_string(1900 + ltm->tm_year) + "-" + 
               std::to_string(1 + ltm->tm_mon) + "-" +
               std::to_string(ltm->tm_mday);
    }
    
    // Constructor with date (for loading from file)
    Transaction(int _id, int _bookId, int _studentId, std::string _type, std::string _date)
        : id(_id), bookId(_bookId), studentId(_studentId), type(_type), date(_date) {}
};

// Global variables
std::vector<Book> books;
std::vector<Student> students;
std::vector<Transaction> transactions;
std::vector<std::string> operationHistory;
int nextBookId = 1;
int nextStudentId = 1;
int nextTransactionId = 1;

// Function to clear the screen (works on most systems)
void clearScreen() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape sequence to clear screen
}

// Function to log an operation
void logOperation(const std::string& operation) {
    // Get current date/time
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    // Format date and time
    std::string timestamp = 
        std::to_string(1900 + ltm->tm_year) + "-" + 
        std::to_string(1 + ltm->tm_mon) + "-" +
        std::to_string(ltm->tm_mday) + " " +
        std::to_string(ltm->tm_hour) + ":" +
        std::to_string(ltm->tm_min) + ":" +
        std::to_string(ltm->tm_sec);
    
    // Add to history
    operationHistory.push_back(timestamp + ": " + operation);
    
    // Save history to file
    std::ofstream historyFile("operation_history.txt", std::ios::app);
    if (historyFile.is_open()) {
        historyFile << timestamp << ": " << operation << std::endl;
        historyFile.close();
    }
}

// Function to save books to file
void saveBooksToFile() {
    std::ofstream file("books.txt");
    if (file.is_open()) {
        file << nextBookId << std::endl; // Save next ID
        for (const auto& book : books) {
            file << book.id << "|" << book.title << "|" << book.author << "|" 
                 << book.isbn << "|" << book.available << std::endl;
        }
        file.close();
        logOperation("Books saved to file");
    } else {
        std::cout << "Unable to open file for saving books." << std::endl;
    }
}

// Function to load books from file
void loadBooksFromFile() {
    std::ifstream file("books.txt");
    if (file.is_open()) {
        books.clear();
        file >> nextBookId;
        file.ignore(); // Skip newline
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            size_t pos = 0;
            std::string delimiter = "|";
            std::vector<std::string> tokens;
            
            while ((pos = line.find(delimiter)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line); // Add the last part
            
            if (tokens.size() >= 5) {
                int id = std::stoi(tokens[0]);
                std::string title = tokens[1];
                std::string author = tokens[2];
                std::string isbn = tokens[3];
                bool available = (tokens[4] == "1");
                
                books.push_back(Book(id, title, author, isbn, available));
            }
        }
        file.close();
        logOperation("Books loaded from file");
    }
}

// Function to save students to file
void saveStudentsToFile() {
    std::ofstream file("students.txt");
    if (file.is_open()) {
        file << nextStudentId << std::endl; // Save next ID
        for (const auto& student : students) {
            file << student.id << "|" << student.name << std::endl;
        }
        file.close();
        logOperation("Students saved to file");
    } else {
        std::cout << "Unable to open file for saving students." << std::endl;
    }
}

// Function to load students from file
void loadStudentsFromFile() {
    std::ifstream file("students.txt");
    if (file.is_open()) {
        students.clear();
        file >> nextStudentId;
        file.ignore(); // Skip newline
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            size_t pos = line.find("|");
            if (pos != std::string::npos) {
                int id = std::stoi(line.substr(0, pos));
                std::string name = line.substr(pos + 1);
                
                students.push_back(Student(id, name));
            }
        }
        file.close();
        logOperation("Students loaded from file");
    }
}

// Function to save transactions to file
void saveTransactionsToFile() {
    std::ofstream file("transactions.txt");
    if (file.is_open()) {
        file << nextTransactionId << std::endl; // Save next ID
        for (const auto& transaction : transactions) {
            file << transaction.id << "|" << transaction.bookId << "|" 
                 << transaction.studentId << "|" << transaction.type << "|" 
                 << transaction.date << std::endl;
        }
        file.close();
        logOperation("Transactions saved to file");
    } else {
        std::cout << "Unable to open file for saving transactions." << std::endl;
    }
}

// Function to load transactions from file
void loadTransactionsFromFile() {
    std::ifstream file("transactions.txt");
    if (file.is_open()) {
        transactions.clear();
        file >> nextTransactionId;
        file.ignore(); // Skip newline
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            size_t pos = 0;
            std::string delimiter = "|";
            std::vector<std::string> tokens;
            
            while ((pos = line.find(delimiter)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line); // Add the last part
            
            if (tokens.size() >= 5) {
                int id = std::stoi(tokens[0]);
                int bookId = std::stoi(tokens[1]);
                int studentId = std::stoi(tokens[2]);
                std::string type = tokens[3];
                std::string date = tokens[4];
                
                transactions.push_back(Transaction(id, bookId, studentId, type, date));
            }
        }
        file.close();
        logOperation("Transactions loaded from file");
    }
}

// Function to save all data to files
void saveAllData() {
    saveBooksToFile();
    saveStudentsToFile();
    saveTransactionsToFile();
}

// Function to load all data from files
void loadAllData() {
    loadBooksFromFile();
    loadStudentsFromFile();
    loadTransactionsFromFile();
}

// Function to add a new book
void addBook() {
    clearScreen();
    std::cout << "\n=== Add New Book ===\n";
    
    std::string title, author, isbn;
    
    std::cout << "Enter title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    
    std::cout << "Enter author: ";
    std::getline(std::cin, author);
    
    std::cout << "Enter ISBN: ";
    std::getline(std::cin, isbn);
    
    // Check if ISBN already exists
    for (const auto& book : books) {
        if (book.isbn == isbn) {
            std::cout << "A book with this ISBN already exists!" << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
    }
    
    // Add new book
    books.push_back(Book(nextBookId++, title, author, isbn));
    saveBooksToFile();
    
    std::cout << "Book added successfully!" << std::endl;
    logOperation("Added book: " + title);
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to display all books
void displayBooks() {
    clearScreen();
    std::cout << "\n=== Book List ===\n";
    
    if (books.empty()) {
        std::cout << "No books in the library." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(30) << "Title" 
                  << std::setw(20) << "Author" 
                  << std::setw(15) << "ISBN" 
                  << "Available" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& book : books) {
            std::cout << std::left << std::setw(5) << book.id 
                      << std::setw(30) << book.title.substr(0, 28) 
                      << std::setw(20) << book.author.substr(0, 18) 
                      << std::setw(15) << book.isbn
                      << (book.available ? "Yes" : "No") << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to search for books
void searchBooks() {
    clearScreen();
    std::cout << "\n=== Search Books ===\n";
    
    std::cout << "Search options:\n";
    std::cout << "1. Search by Title\n";
    std::cout << "2. Search by Author\n";
    std::cout << "3. Search by ISBN\n";
    std::cout << "Enter your choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::string searchTerm;
    std::cout << "Enter search term: ";
    std::getline(std::cin, searchTerm);
    
    // Convert search term to lowercase for case-insensitive search
    std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
    
    std::vector<Book> results;
    
    for (const auto& book : books) {
        std::string fieldToSearch;
        
        if (choice == 1) {
            fieldToSearch = book.title;
        } else if (choice == 2) {
            fieldToSearch = book.author;
        } else if (choice == 3) {
            fieldToSearch = book.isbn;
        } else {
            break;
        }
        
        // Convert to lowercase
        std::transform(fieldToSearch.begin(), fieldToSearch.end(), fieldToSearch.begin(), ::tolower);
        
        // Check if search term is in the field
        if (fieldToSearch.find(searchTerm) != std::string::npos) {
            results.push_back(book);
        }
    }
    
    clearScreen();
    std::cout << "\n=== Search Results ===\n";
    
    if (results.empty()) {
        std::cout << "No matching books found." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(30) << "Title" 
                  << std::setw(20) << "Author" 
                  << std::setw(15) << "ISBN" 
                  << "Available" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& book : results) {
            std::cout << std::left << std::setw(5) << book.id 
                      << std::setw(30) << book.title.substr(0, 28) 
                      << std::setw(20) << book.author.substr(0, 18) 
                      << std::setw(15) << book.isbn
                      << (book.available ? "Yes" : "No") << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// Function to update a book
void updateBook() {
    clearScreen();
    std::cout << "\n=== Update Book ===\n";
    
    int id;
    std::cout << "Enter book ID to update: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Find the book
    auto it = std::find_if(books.begin(), books.end(), [id](const Book& book) {
        return book.id == id;
    });
    
    if (it != books.end()) {
        std::cout << "Book found: " << it->title << " by " << it->author << std::endl;
        std::cout << "\nUpdate:\n";
        std::cout << "1. Title\n";
        std::cout << "2. Author\n";
        std::cout << "3. ISBN\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::string newValue;
        std::cout << "Enter new value: ";
        std::getline(std::cin, newValue);
        
        std::string oldValue;
        if (choice == 1) {
            oldValue = it->title;
            it->title = newValue;
        } else if (choice == 2) {
            oldValue = it->author;
            it->author = newValue;
        } else if (choice == 3) {
            // Check if ISBN already exists
            for (const auto& book : books) {
                if (book.isbn == newValue && book.id != id) {
                    std::cout << "A book with this ISBN already exists!" << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                    return;
                }
            }
            oldValue = it->isbn;
            it->isbn = newValue;
        }
        
        saveBooksToFile();
        
        std::cout << "Book updated successfully!" << std::endl;
        logOperation("Updated book ID " + std::to_string(id) + ": " + oldValue + " -> " + newValue);
    } else {
        std::cout << "Book not found." << std::endl;
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to delete a book
void deleteBook() {
    clearScreen();
    std::cout << "\n=== Delete Book ===\n";
    
    int id;
    std::cout << "Enter book ID to delete: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Find the book
    auto it = std::find_if(books.begin(), books.end(), [id](const Book& book) {
        return book.id == id;
    });
    
    if (it != books.end()) {
        // Check if the book is currently borrowed
        if (!it->available) {
            std::cout << "Cannot delete this book as it is currently borrowed." << std::endl;
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        std::string title = it->title;
        books.erase(it);
        saveBooksToFile();
        
        std::cout << "Book deleted successfully!" << std::endl;
        logOperation("Deleted book: " + title + " (ID: " + std::to_string(id) + ")");
    } else {
        std::cout << "Book not found." << std::endl;
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to add a new student
void addStudent() {
    clearScreen();
    std::cout << "\n=== Add New Student ===\n";
    
    std::string name;
    
    std::cout << "Enter student name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    
    // Add new student
    students.push_back(Student(nextStudentId++, name));
    saveStudentsToFile();
    
    std::cout << "Student added successfully!" << std::endl;
    logOperation("Added student: " + name);
    
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

// Function to display all students
void displayStudents() {
    clearScreen();
    std::cout << "\n=== Student List ===\n";
    
    if (students.empty()) {
        std::cout << "No students registered." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID" 
                  << "Name" << std::endl;
        std::cout << std::string(40, '-') << std::endl;
        
        for (const auto& student : students) {
            std::cout << std::left << std::setw(5) << student.id 
                      << student.name << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to borrow a book
void borrowBook() {
    clearScreen();
    std::cout << "\n=== Borrow Book ===\n";
    
    // Check if there are any students
    if (students.empty()) {
        std::cout << "No students registered. Please add a student first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }
    
    // Check if there are any books
    if (books.empty()) {
        std::cout << "No books in the library. Please add a book first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }
    
    int studentId;
    std::cout << "Enter student ID: ";
    std::cin >> studentId;
    
    // Find the student
    auto studentIt = std::find_if(students.begin(), students.end(), [studentId](const Student& student) {
        return student.id == studentId;
    });
    
    if (studentIt == students.end()) {
        std::cout << "Student not found." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }
    
    int bookId;
    std::cout << "Enter book ID: ";
    std::cin >> bookId;
    
    // Find the book
    auto bookIt = std::find_if(books.begin(), books.end(), [bookId](const Book& book) {
        return book.id == bookId;
    });
    
    if (bookIt == books.end()) {
        std::cout << "Book not found." << std::endl;
    } else if (!bookIt->available) {
        std::cout << "This book is already borrowed." << std::endl;
    } else {
        // Update book status
        bookIt->available = false;
        
        // Create transaction
        transactions.push_back(Transaction(nextTransactionId++, bookId, studentId, "borrow"));
        
        saveBooksToFile();
        saveTransactionsToFile();
        
        std::cout << "Book borrowed successfully!" << std::endl;
        logOperation("Student ID " + std::to_string(studentId) + 
                    " borrowed book: " + bookIt->title + " (ID: " + std::to_string(bookId) + ")");
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to return a book
void returnBook() {
    clearScreen();
    std::cout << "\n=== Return Book ===\n";
    
    int bookId;
    std::cout << "Enter book ID: ";
    std::cin >> bookId;
    
    // Find the book
    auto bookIt = std::find_if(books.begin(), books.end(), [bookId](const Book& book) {
        return book.id == bookId;
    });
    
    if (bookIt == books.end()) {
        std::cout << "Book not found." << std::endl;
    } else if (bookIt->available) {
        std::cout << "This book is not currently borrowed." << std::endl;
    } else {
        // Find the corresponding borrow transaction
        auto transactionIt = std::find_if(
            transactions.rbegin(), transactions.rend(), 
            [bookId](const Transaction& t) {
                return t.bookId == bookId && t.type == "borrow";
            }
        );
        
        if (transactionIt != transactions.rend()) {
            int studentId = transactionIt->studentId;
            
            // Update book status
            bookIt->available = true;
            
            // Create return transaction
            transactions.push_back(Transaction(nextTransactionId++, bookId, studentId, "return"));
            
            saveBooksToFile();
            saveTransactionsToFile();
            
            std::cout << "Book returned successfully!" << std::endl;
            logOperation("Student ID " + std::to_string(studentId) + 
                        " returned book: " + bookIt->title + " (ID: " + std::to_string(bookId) + ")");
        } else {
            std::cout << "Error: Could not find borrow transaction for this book." << std::endl;
        }
    }
    
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to display transaction history
void displayTransactions() {
    clearScreen();
    std::cout << "\n=== Transaction History ===\n";
    
    if (transactions.empty()) {
        std::cout << "No transactions recorded." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(10) << "Type" 
                  << std::setw(12) << "Date" 
                  << std::setw(12) << "Student ID" 
                  << std::setw(10) << "Book ID"
                  << "Book Title" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& transaction : transactions) {
            // Find book title
            std::string bookTitle = "Unknown";
            auto bookIt = std::find_if(books.begin(), books.end(), [&transaction](const Book& book) {
                return book.id == transaction.bookId;
            });
            
            if (bookIt != books.end()) {
                bookTitle = bookIt->title;
            }
            
            std::cout << std::left << std::setw(5) << transaction.id 
                      << std::setw(10) << transaction.type
                      << std::setw(12) << transaction.date
                      << std::setw(12) << transaction.studentId
                      << std::setw(10) << transaction.bookId
                      << bookTitle << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to display operation history
void displayHistory() {
    clearScreen();
    std::cout << "\n=== Operation History ===\n";
    
    if (operationHistory.empty()) {
        std::cout << "No operations recorded." << std::endl;
    } else {
        for (const auto& operation : operationHistory) {
            std::cout << operation << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Function to display the main menu
void displayMenu() {
    clearScreen();
    std::cout << "\n=== Library Management System ===\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Display Books\n";
    std::cout << "3. Search Books\n";
    std::cout << "4. Update Book\n";
    std::cout << "5. Delete Book\n";
    std::cout << "6. Add Student\n";
    std::cout << "7. Display Students\n";
    std::cout << "8. Borrow Book\n";
    std::cout << "9. Return Book\n";
    std::cout << "10. Display Transactions\n";
    std::cout << "11. Display Operation History\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    // Load data from files
    loadAllData();
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                searchBooks();
                break;
            case 4:
                updateBook();
                break;
            case 5:
                deleteBook();
                break;
            case 6:
                addStudent();
                break;
            case 7:
                displayStudents();
                break;
            case 8:
                borrowBook();
                break;
            case 9:
                returnBook();
                break;
            case 10:
                displayTransactions();
                break;
            case 11:
                displayHistory();
                break;
            case 0:
                saveAllData();
                running = false;
                std::cout << "Thank you for using the Library Management System!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
        }
    }
    
    return 0;
}