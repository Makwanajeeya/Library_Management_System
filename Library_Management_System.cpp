#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

class Book {
public:
    int bookID;
    string title;
    string author;
    bool isAvailable;

    Book(int id, string t, string a) : bookID(id), title(t), author(a), isAvailable(true) {}
};

class User {
public:
    int userID;
    string userName;
    vector<string> borrowedBooks;
    vector<string> history;

    User(int id, string name) : userID(id), userName(name) {}

    void addBorrowedBook(string bookTitle) {
        borrowedBooks.push_back(bookTitle);
        history.push_back("Borrowed: " + bookTitle);
    }

    void removeBorrowedBook(string bookTitle) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (*it == bookTitle) {
                borrowedBooks.erase(it);
                history.push_back("Returned: " + bookTitle);
                break;
            }
        }
    }

    void displayHistory() {
        cout << "History for user " << userName << " (" << userID << "):" << endl;
        if (history.empty()) {
            cout << "No actions performed yet." << endl;
        } else {
            for (const auto& action : history) {
                cout << action << endl;
            }
        }
    }
};

class LibraryManager {
private:
    unordered_map<int, User*> users;
    unordered_map<int, Book*> books;
    queue<Book*> availableBooks;
    stack<Book*> borrowedBooks;

public:

    void addBook(int id, string title, string author) {
        if (books.find(id) != books.end()) {
            cout << "Book with ID " << id << " already exists." << endl;
            return;
        }
        Book* newBook = new Book(id, title, author);
        books[id] = newBook;
        availableBooks.push(newBook);
        cout << "Book added: " << title << endl;
    }

    void addUser(int id, string name) {
        if (users.find(id) != users.end()) {
            cout << "User with ID " << id << " already exists." << endl;
            return;
        }
        User* newUser = new User(id, name);
        users[id] = newUser;
        cout << "User added: " << name << endl;
    }

    void borrowBook(int userID, int bookID) {
        if (users.find(userID) == users.end()) {
            cout << "User not found!" << endl;
            return;
        }

        User* user = users[userID];

        if (books.find(bookID) == books.end()) {
            cout << "Book with ID " << bookID << " does not exist in the library." << endl;
            return;
        }

        Book* selectedBook = books[bookID];
        if (!selectedBook->isAvailable) {
            cout << "The book '" << selectedBook->title << "' is currently not available." << endl;
            return;
        }

        selectedBook->isAvailable = false;
        borrowedBooks.push(selectedBook);
        user->addBorrowedBook(selectedBook->title);
        cout << user->userName << " borrowed: " << selectedBook->title << endl;
    }

    void returnBook(int userID) {
        if (users.find(userID) == users.end()) {
            cout << "User not found!" << endl;
            return;
        }

        User* user = users[userID];
        if (borrowedBooks.empty()) {
            cout << "No books are borrowed." << endl;
            return;
        }

        Book* returnedBook = borrowedBooks.top();
        borrowedBooks.pop();
        returnedBook->isAvailable = true;
        availableBooks.push(returnedBook);
        user->removeBorrowedBook(returnedBook->title);
        cout << user->userName << " returned: " << returnedBook->title << endl;
    }

    void displayAvailableBooks() {
        if (availableBooks.empty()) {
            cout << "No books are currently available." << endl;
            return;
        }

        cout << "Available Books:" << endl;
        queue<Book*> tempQueue = availableBooks;
        while (!tempQueue.empty()) {
            Book* book = tempQueue.front();
            tempQueue.pop();
            if (book->isAvailable) {
                cout << "ID: " << book->bookID << ", Title: " << book->title << ", Author: " << book->author << endl;
            }
        }
    }

    void displayBorrowedBooks(int userID) {
        if (users.find(userID) == users.end()) {
            cout << "User not found!" << endl;
            return;
        }

        User* user = users[userID];
        if (user->borrowedBooks.empty()) {
            cout << user->userName << " has not borrowed any books." << endl;
            return;
        }

        cout << user->userName << "'s Borrowed Books:" << endl;
        for (const auto& book : user->borrowedBooks) {
            cout << book << endl;
        }
    }

    void displayUserHistory(int userID) {
        if (users.find(userID) == users.end()) {
            cout << "User not found!" << endl;
            return;
        }

        User* user = users[userID];
        user->displayHistory();
    }
};

int main() {
    LibraryManager manager;
    int choice, userID, bookID;
    string title, author, userName;

    do {
        cout << "\nLibrary Management System (Manager)\n";
        cout << "1. Add Book\n";
        cout << "2. Add User\n";
        cout << "3. Display Available Books\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Display Borrowed Books\n";
        cout << "7. Display User History\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Book ID: ";
                cin >> bookID;
                cin.ignore();
                cout << "Enter Book Title: ";
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                manager.addBook(bookID, title, author);
                break;

            case 2:
                cout << "Enter User ID: ";
                cin >> userID;
                cin.ignore();
                cout << "Enter User Name: ";
                getline(cin, userName);
                manager.addUser(userID, userName);
                break;

            case 3:
                manager.displayAvailableBooks();
                break;

            case 4:
                cout << "Enter User ID: ";
                cin >> userID;
                manager.displayAvailableBooks();
                cout << "Enter Book ID to borrow: ";
                cin >> bookID;
                manager.borrowBook(userID, bookID);
                break;

            case 5:
                cout << "Enter User ID: ";
                cin >> userID;
                manager.returnBook(userID);
                break;

            case 6:
                cout << "Enter User ID: ";
                cin >> userID;
                manager.displayBorrowedBooks(userID);
                break;

            case 7:
                cout << "Enter User ID: ";
                cin >> userID;
                manager.displayUserHistory(userID);
                break;

            case 8:
                cout << "Exiting... Thank you for using the Library Management System!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);

    return 0;
}
