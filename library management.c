#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 50
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_NAME 50

// Structure to store book information
typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int quantity;
    int issued_count;
} Book;

// Structure to store student information
typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_NAME];
    int books_issued;
    int book_ids[5]; // Maximum 5 books per student
} Student;

// Structure to store transaction records
typedef struct {
    int transaction_id;
    int book_id;
    int student_id;
    char issue_date[20];
    char return_date[20];
    int status; // 0 = issued, 1 = returned
} Transaction;

// Global variables
Book books[MAX_BOOKS];
Student students[MAX_STUDENTS];
Transaction transactions[MAX_BOOKS * 5];
int book_count = 0;
int student_count = 0;
int transaction_count = 0;
int next_book_id = 1;
int next_student_id = 1;
int next_transaction_id = 1;

// Function prototypes
void addBook();
void displayBooks();
void searchBook();
void updateBook();
void deleteBook();
void addStudent();
void displayStudents();
void searchStudent();
void issueBook();
void returnBook();
void displayTransactions();
void saveData();
void loadData();
void adminMenu();
void studentMenu();
int findBookById(int id);
int findStudentById(int id);
void clearScreen();
void pressAnyKey();

int main() {
    int choice;
    
    loadData(); // Load existing data from file
    
    while(1) {
        printf("\n========== LIBRARY MANAGEMENT SYSTEM ==========\n");
        printf("1. Admin Login\n");
        printf("2. Student Section\n");
        printf("3. Exit\n");
        printf("===============================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                adminMenu();
                break;
            case 2:
                studentMenu();
                break;
            case 3:
                saveData();
                printf("Thank you for using Library Management System!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}

void adminMenu() {
    int choice;
    char password[20];
    
    printf("\n----- Admin Login -----\n");
    printf("Enter password: ");
    scanf("%s", password);
    
    // Simple password check (you can change this)
    if(strcmp(password, "admin123") != 0) {
        printf("Invalid password!\n");
        return;
    }
    
    while(1) {
        printf("\n========== ADMIN MENU ==========\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Update Book\n");
        printf("5. Delete Book\n");
        printf("6. Add Student\n");
        printf("7. Display All Students\n");
        printf("8. Search Student\n");
        printf("9. Issue Book\n");
        printf("10. Return Book\n");
        printf("11. View Transactions\n");
        printf("12. Save Data\n");
        printf("13. Back to Main Menu\n");
        printf("================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addBook();
                break;
            case 2:
                displayBooks();
                break;
            case 3:
                searchBook();
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
                searchStudent();
                break;
            case 9:
                issueBook();
                break;
            case 10:
                returnBook();
                break;
            case 11:
                displayTransactions();
                break;
            case 12:
                saveData();
                break;
            case 13:
                return;
            default:
                printf("Invalid choice!\n");
        }
        pressAnyKey();
    }
}

void studentMenu() {
    int student_id, choice;
    
    printf("\n----- Student Login -----\n");
    printf("Enter your Student ID: ");
    scanf("%d", &student_id);
    
    if(findStudentById(student_id) == -1) {
        printf("Student not found! Please contact admin.\n");
        return;
    }
    
    while(1) {
        printf("\n========== STUDENT MENU ==========\n");
        printf("1. View All Books\n");
        printf("2. Search Book\n");
        printf("3. My Issued Books\n");
        printf("4. Back to Main Menu\n");
        printf("==================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                displayBooks();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                displayStudentBooks(student_id);
                break;
            case 4:
                return;
            default:
                printf("Invalid choice!\n");
        }
        pressAnyKey();
    }
}

void addBook() {
    if(book_count >= MAX_BOOKS) {
        printf("Library is full! Cannot add more books.\n");
        return;
    }
    
    Book new_book;
    new_book.id = next_book_id++;
    
    printf("\n----- Add New Book -----\n");
    printf("Enter Book Title: ");
    getchar(); // Clear input buffer
    fgets(new_book.title, MAX_TITLE, stdin);
    new_book.title[strcspn(new_book.title, "\n")] = 0;
    
    printf("Enter Author Name: ");
    fgets(new_book.author, MAX_AUTHOR, stdin);
    new_book.author[strcspn(new_book.author, "\n")] = 0;
    
    printf("Enter Quantity: ");
    scanf("%d", &new_book.quantity);
    
    new_book.issued_count = 0;
    
    books[book_count++] = new_book;
    printf("Book added successfully! Book ID: %d\n", new_book.id);
}

void displayBooks() {
    if(book_count == 0) {
        printf("No books in the library.\n");
        return;
    }
    
    printf("\n%-10s %-30s %-20s %-10s %-10s\n", "Book ID", "Title", "Author", "Quantity", "Issued");
    printf("--------------------------------------------------------------------\n");
    
    for(int i = 0; i < book_count; i++) {
        printf("%-10d %-30s %-20s %-10d %-10d\n", 
               books[i].id, 
               books[i].title, 
               books[i].author, 
               books[i].quantity,
               books[i].issued_count);
    }
}

void searchBook() {
    int choice, id;
    char title[MAX_TITLE];
    int found = 0;
    
    printf("\n----- Search Book -----\n");
    printf("1. Search by ID\n");
    printf("2. Search by Title\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if(choice == 1) {
        printf("Enter Book ID: ");
        scanf("%d", &id);
        
        int index = findBookById(id);
        if(index != -1) {
            printf("\nBook Found:\n");
            printf("ID: %d\n", books[index].id);
            printf("Title: %s\n", books[index].title);
            printf("Author: %s\n", books[index].author);
            printf("Quantity: %d\n", books[index].quantity);
            printf("Issued: %d\n", books[index].issued_count);
            printf("Available: %d\n", books[index].quantity - books[index].issued_count);
        } else {
            printf("Book not found!\n");
        }
    } else if(choice == 2) {
        printf("Enter Book Title: ");
        getchar();
        fgets(title, MAX_TITLE, stdin);
        title[strcspn(title, "\n")] = 0;
        
        printf("\nSearch Results:\n");
        for(int i = 0; i < book_count; i++) {
            if(strstr(strlwr(books[i].title), strlwr(title))) {
                printf("\nID: %d\n", books[i].id);
                printf("Title: %s\n", books[i].title);
                printf("Author: %s\n", books[i].author);
                printf("Available: %d\n", books[i].quantity - books[i].issued_count);
                found = 1;
            }
        }
        if(!found) {
            printf("No books found with title containing '%s'\n", title);
        }
    }
}

void updateBook() {
    int id, index;
    
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    
    index = findBookById(id);
    if(index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    printf("\n----- Update Book (ID: %d) -----\n", id);
    printf("Current Title: %s\n", books[index].title);
    printf("New Title (press enter to keep): ");
    getchar();
    char new_title[MAX_TITLE];
    fgets(new_title, MAX_TITLE, stdin);
    if(strcmp(new_title, "\n") != 0) {
        new_title[strcspn(new_title, "\n")] = 0;
        strcpy(books[index].title, new_title);
    }
    
    printf("Current Author: %s\n", books[index].author);
    printf("New Author (press enter to keep): ");
    char new_author[MAX_AUTHOR];
    fgets(new_author, MAX_AUTHOR, stdin);
    if(strcmp(new_author, "\n") != 0) {
        new_author[strcspn(new_author, "\n")] = 0;
        strcpy(books[index].author, new_author);
    }
    
    printf("Current Quantity: %d\n", books[index].quantity);
    printf("New Quantity: ");
    int new_quantity;
    scanf("%d", &new_quantity);
    if(new_quantity > 0) {
        books[index].quantity = new_quantity;
    }
    
    printf("Book updated successfully!\n");
}

void deleteBook() {
    int id, index;
    
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    
    index = findBookById(id);
    if(index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    if(books[index].issued_count > 0) {
        printf("Cannot delete! %d copies of this book are currently issued.\n", books[index].issued_count);
        return;
    }
    
    // Shift all books after index to left
    for(int i = index; i < book_count - 1; i++) {
        books[i] = books[i + 1];
    }
    book_count--;
    
    printf("Book deleted successfully!\n");
}

void addStudent() {
    if(student_count >= MAX_STUDENTS) {
        printf("Cannot add more students. Limit reached.\n");
        return;
    }
    
    Student new_student;
    new_student.id = next_student_id++;
    new_student.books_issued = 0;
    
    printf("\n----- Add New Student -----\n");
    printf("Enter Student Name: ");
    getchar();
    fgets(new_student.name, MAX_NAME, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0;
    
    printf("Enter Student Email: ");
    fgets(new_student.email, MAX_NAME, stdin);
    new_student.email[strcspn(new_student.email, "\n")] = 0;
    
    students[student_count++] = new_student;
    printf("Student added successfully! Student ID: %d\n", new_student.id);
}

void displayStudents() {
    if(student_count == 0) {
        printf("No students registered.\n");
        return;
    }
    
    printf("\n%-10s %-20s %-25s %-10s\n", "Student ID", "Name", "Email", "Books Issued");
    printf("----------------------------------------------------------------\n");
    
    for(int i = 0; i < student_count; i++) {
        printf("%-10d %-20s %-25s %-10d\n", 
               students[i].id, 
               students[i].name, 
               students[i].email,
               students[i].books_issued);
    }
}

void searchStudent() {
    int id;
    
    printf("Enter Student ID: ");
    scanf("%d", &id);
    
    int index = findStudentById(id);
    if(index != -1) {
        printf("\nStudent Found:\n");
        printf("ID: %d\n", students[index].id);
        printf("Name: %s\n", students[index].name);
        printf("Email: %s\n", students[index].email);
        printf("Books Issued: %d\n", students[index].books_issued);
        
        if(students[index].books_issued > 0) {
            printf("\nIssued Books:\n");
            for(int i = 0; i < students[index].books_issued; i++) {
                int book_index = findBookById(students[index].book_ids[i]);
                if(book_index != -1) {
                    printf("  - %s by %s\n", books[book_index].title, books[book_index].author);
                }
            }
        }
    } else {
        printf("Student not found!\n");
    }
}

void issueBook() {
    int student_id, book_id;
    
    printf("\n----- Issue Book -----\n");
    printf("Enter Student ID: ");
    scanf("%d", &student_id);
    
    int student_index = findStudentById(student_id);
    if(student_index == -1) {
        printf("Student not found!\n");
        return;
    }
    
    if(students[student_index].books_issued >= 5) {
        printf("Student has already issued maximum number of books (5).\n");
        return;
    }
    
    printf("Enter Book ID: ");
    scanf("%d", &book_id);
    
    int book_index = findBookById(book_id);
    if(book_index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    if(books[book_index].issued_count >= books[book_index].quantity) {
        printf("No copies available! All %d copies are issued.\n", books[book_index].quantity);
        return;
    }
    
    // Check if student already has this book
    for(int i = 0; i < students[student_index].books_issued; i++) {
        if(students[student_index].book_ids[i] == book_id) {
            printf("Student already has this book issued!\n");
            return;
        }
    }
    
    // Issue the book
    students[student_index].book_ids[students[student_index].books_issued++] = book_id;
    books[book_index].issued_count++;
    
    // Record transaction
    Transaction trans;
    trans.transaction_id = next_transaction_id++;
    trans.book_id = book_id;
    trans.student_id = student_id;
    strcpy(trans.issue_date, "2024-01-01"); // You can implement date input
    strcpy(trans.return_date, "Not Returned");
    trans.status = 0;
    
    transactions[transaction_count++] = trans;
    
    printf("Book issued successfully!\n");
}

void returnBook() {
    int student_id, book_id;
    
    printf("\n----- Return Book -----\n");
    printf("Enter Student ID: ");
    scanf("%d", &student_id);
    
    int student_index = findStudentById(student_id);
    if(student_index == -1) {
        printf("Student not found!\n");
        return;
    }
    
    if(students[student_index].books_issued == 0) {
        printf("Student has no issued books.\n");
        return;
    }
    
    printf("Enter Book ID to return: ");
    scanf("%d", &book_id);
    
    int book_index = findBookById(book_id);
    if(book_index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    // Find and remove book from student's list
    int found = 0;
    for(int i = 0; i < students[student_index].books_issued; i++) {
        if(students[student_index].book_ids[i] == book_id) {
            // Shift remaining books left
            for(int j = i; j < students[student_index].books_issued - 1; j++) {
                students[student_index].book_ids[j] = students[student_index].book_ids[j + 1];
            }
            students[student_index].books_issued--;
            books[book_index].issued_count--;
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("This book was not issued to this student!\n");
        return;
    }
    
    // Update transaction
    for(int i = 0; i < transaction_count; i++) {
        if(transactions[i].book_id == book_id && 
           transactions[i].student_id == student_id && 
           transactions[i].status == 0) {
            strcpy(transactions[i].return_date, "2024-01-10"); // You can implement date input
            transactions[i].status = 1;
            break;
        }
    }
    
    printf("Book returned successfully!\n");
}

void displayTransactions() {
    if(transaction_count == 0) {
        printf("No transactions found.\n");
        return;
    }
    
    printf("\n%-15s %-10s %-12s %-12s %-15s %s\n", 
           "Trans ID", "Book ID", "Student ID", "Issue Date", "Return Date", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < transaction_count; i++) {
        printf("%-15d %-10d %-12d %-12s %-15s %s\n",
               transactions[i].transaction_id,
               transactions[i].book_id,
               transactions[i].student_id,
               transactions[i].issue_date,
               transactions[i].return_date,
               transactions[i].status == 0 ? "Issued" : "Returned");
    }
}

void displayStudentBooks(int student_id) {
    int index = findStudentById(student_id);
    if(index == -1) return;
    
    if(students[index].books_issued == 0) {
        printf("No books issued.\n");
        return;
    }
    
    printf("\nBooks issued to %s:\n", students[index].name);
    printf("%-10s %-30s %-20s\n", "Book ID", "Title", "Author");
    printf("----------------------------------------------\n");
    
    for(int i = 0; i < students[index].books_issued; i++) {
        int book_index = findBookById(students[index].book_ids[i]);
        if(book_index != -1) {
            printf("%-10d %-30s %-20s\n",
                   books[book_index].id,
                   books[book_index].title,
                   books[book_index].author);
        }
    }
}

int findBookById(int id) {
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findStudentById(int id) {
    for(int i = 0; i < student_count; i++) {
        if(students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void saveData() {
    FILE *file;
    
    // Save books
    file = fopen("books.dat", "wb");
    if(file != NULL) {
        fwrite(&book_count, sizeof(int), 1, file);
        fwrite(&next_book_id, sizeof(int), 1, file);
        fwrite(books, sizeof(Book), book_count, file);
        fclose(file);
    }
    
    // Save students
    file = fopen("students.dat", "wb");
    if(file != NULL) {
        fwrite(&student_count, sizeof(int), 1, file);
        fwrite(&next_student_id, sizeof(int), 1, file);
        fwrite(students, sizeof(Student), student_count, file);
        fclose(file);
    }
    
    // Save transactions
    file = fopen("transactions.dat", "wb");
    if(file != NULL) {
        fwrite(&transaction_count, sizeof(int), 1, file);
        fwrite(&next_transaction_id, sizeof(int), 1, file);
        fwrite(transactions, sizeof(Transaction), transaction_count, file);
        fclose(file);
    }
    
    printf("Data saved successfully!\n");
}

void loadData() {
    FILE *file;
    
    // Load books
    file = fopen("books.dat", "rb");
    if(file != NULL) {
        fread(&book_count, sizeof(int), 1, file);
        fread(&next_book_id, sizeof(int), 1, file);
        fread(books, sizeof(Book), book_count, file);
        fclose(file);
    }
    
    // Load students
    file = fopen("students.dat", "rb");
    if(file != NULL) {
        fread(&student_count, sizeof(int), 1, file);
        fread(&next_student_id, sizeof(int), 1, file);
        fread(students, sizeof(Student), student_count, file);
        fclose(file);
    }
    
    // Load transactions
    file = fopen("transactions.dat", "rb");
    if(file != NULL) {
        fread(&transaction_count, sizeof(int), 1, file);
        fread(&next_transaction_id, sizeof(int), 1, file);
        fread(transactions, sizeof(Transaction), transaction_count, file);
        fclose(file);
    }
}

void clearScreen() {
    system("clear || cls");
}

void pressAnyKey() {
    printf("\nPress any key to continue...");
    getchar();
    getchar();
}