#include <iostream>
#include <iomanip>  // For formatting output
#include <fstream>  // For file operations
#include <string>   // For string handling
#include <cctype>   // For checking digit characters
#include <conio.h>  // For _getch() to hide PIN input (Windows only)
using namespace std;

// Node structure for the BST
struct Node {
    string name;
    long long int card_no;
    int pin;
    int cash;
    Node* left;
    Node* right;
};

class ATM {
    Node* root;
    string x;          // Temporary input for name
    long long int y;   // Temporary input for card number
    int z;             // Temporary input for pin
    int c;             // Temporary input for cash amount

public:
    ATM() : root(nullptr), x(""), y(0), z(0), c(0) {
        loadAccountsFromFile();
    }

    // Function to validate card number
    bool validateCardNumber(long long int card_no) {
        string cardStr = to_string(card_no);
        if (cardStr.length() >= 13 && cardStr.length() <= 19) {
            for (char ch : cardStr) {
                if (!isdigit(ch)) {
                    return false; // Non-digit character found
                }
            }
            return true;
        }
        return false;
    }

    // Create a new account
    void CreateACC() {
        Node* newNode = new Node;
        cout << "  Enter Name: ";
        cin >> x;
        newNode->name = x;

        // Card number input validation
        while (true) {
            cout << "  Enter Card No (13 to 19 digits): ";
            cin >> y;
            if (validateCardNumber(y)) {
                newNode->card_no = y;
                break;
            } else {
                cout << "  Invalid Card Number! Please enter a valid 13 to 19 digit card number." << endl;
            }
        }

        cout << "  Enter Pin: ";
        z = getPinInput();
        newNode->pin = z;

        newNode->cash = 0;
        newNode->left = newNode->right = nullptr;

        root = insertNode(root, newNode);

        cout << "Account Created Successfully" << endl;
        saveAccountsToFile();
    }

    // Insert node into the BST
    Node* insertNode(Node* root, Node* newNode) {
        if (root == nullptr)
            return newNode;

        if (newNode->card_no < root->card_no)
            root->left = insertNode(root->left, newNode);
        else if (newNode->card_no > root->card_no)
            root->right = insertNode(root->right, newNode);

        return root;
    }

    // Login to an account
    bool LOGIN_TO_ACCOUNT() {
        cout << "  Enter Card No: ";
        cin >> y;

        // Card number validation
        if (!validateCardNumber(y)) {
            cout << "  Invalid Card Number! It should be between 13 to 19 digits." << endl;
            return false;
        }

        cout << "  Enter Your PIN: ";
        z = getPinInput();

        Node* account = searchAccount(root, y, z);
        if (account) {
            cout << "***********" << endl;
            cout << "  Name: " << account->name << endl;
            return true;
        } else {
            cout << "  Invalid Card No or Pin" << endl;
            return false;
        }
    }

    // Search for an account in the BST
    Node* searchAccount(Node* root, long long int card_no, int pin) {
        if (root == nullptr || (root->card_no == card_no && root->pin == pin))
            return root;

        if (card_no < root->card_no)
            return searchAccount(root->left, card_no, pin);
        else
            return searchAccount(root->right, card_no, pin);
    }

// Function to hide PIN input and return the value
int getPinInput() {
    int pin = 0;
    char ch;
    int i = 0;
    
    while (i < 4) {  // Ensure that exactly 4 digits are entered
        ch = _getch();  // Get a character without displaying it
        
        if (ch == 13) {  // Enter key
            if (i == 4) {  // If 4 digits are entered, allow the user to press Enter
                break;
            } else {
                cout << "\n  PIN must be 4 digits! Please enter again: ";
                pin = 0;  // Reset the PIN
                i = 0;    // Reset the counter
                continue;
            }
        } else if (ch == 8) {  // Backspace
            if (i > 0) {
                cout << "\b \b";  // Erase the last '*' character
                pin /= 10;        // Remove the last digit from the PIN
                i--;
            }
        } else if (isdigit(ch)) {  // Only accept digits
            pin = pin * 10 + (ch - '0');
            cout << "*";  // Display '*' instead of the number
            i++;
        }
    }
    cout << endl;
    return pin;
}


    // Deposit money
    void Deposit() {
        Node* account = searchAccount(root, y, z);
        if (account) {
            cout << "Enter The Amount to Deposit: ";
            cin >> c;
            account->cash += c;
            cout << "Amount Deposited Successfully" << endl;
            saveAccountsToFile();
        }
    }

    // Withdraw money
    void Withdraw() {
        Node* account = searchAccount(root, y, z);
        if (account) {
            cout << "Enter The Amount to Withdraw: ";
            cin >> c;
            if (account->cash >= c) {
                account->cash -= c;
                cout << "Amount Withdrawn Successfully" << endl;
            } else {
                cout << "Insufficient Balance" << endl;
            }
            saveAccountsToFile();
        }
    }

    // Balance inquiry
    void Balance_Enquiry() {
        Node* account = searchAccount(root, y, z);
        if (account) {
            cout << "Your Balance is: " << account->cash << endl;
        }
    }

    // Display all accounts sorted alphabetically by name
    void DisplayAccountsSortedByName() {
        int count = countNodes(root);
        if (count == 0) {
            cout << "No accounts available to display." << endl;
            return;
        }

        Node** accounts = new Node*[count];
        int index = 0;
        collectNodesToArray(root, accounts, index);

        // Bubble sort to sort accounts by name
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (accounts[j]->name > accounts[j + 1]->name) {
                    Node* temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                }
            }
        }

        cout << "*********** All Accounts Sorted by Name ***********" << endl;
        cout << left << setw(20) << "Name"
             << setw(20) << "Account Number"
             << setw(15) << "Balance" << endl;
        cout << string(55, '-') << endl;

        for (int i = 0; i < count; ++i) {
            cout << left << setw(20) << accounts[i]->name
                 << setw(20) << accounts[i]->card_no
                 << setw(15) << accounts[i]->cash << endl;
        }

        delete[] accounts;
    }

    // Count nodes in the BST
    int countNodes(Node* root) {
        if (root == nullptr)
            return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    // Collect nodes into an array for sorting
    void collectNodesToArray(Node* root, Node** accounts, int& index) {
        if (root == nullptr)
            return;
        collectNodesToArray(root->left, accounts, index);
        accounts[index++] = root;
        collectNodesToArray(root->right, accounts, index);
    }

    // Save accounts to file, sorted alphabetically by name
    void saveAccountsToFile() {
        int count = countNodes(root);
        if (count == 0) {
            cout << "No accounts available to save." << endl;
            return;
        }

        Node** accounts = new Node*[count];
        int index = 0;
        collectNodesToArray(root, accounts, index);

        // Bubble sort to sort accounts by name
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (accounts[j]->name > accounts[j + 1]->name) {
                    Node* temp = accounts[j];
                    accounts[j] = accounts[j + 1];
                    accounts[j + 1] = temp;
                }
            }
        }

        ofstream outFile("accounts.txt", ios::trunc);
        if (!outFile) {
            cout << "Unable to save accounts to file!" << endl;
            delete[] accounts;
            return;
        }

        outFile << left << setw(20) << "Name"
                << setw(20) << "Account Number"
                << setw(15) << "Balance" << endl;
        outFile << string(55, '-') << endl;

        for (int i = 0; i < count; ++i) {
            outFile << left << setw(20) << accounts[i]->name
                    << setw(20) << accounts[i]->card_no
                    << setw(15) << accounts[i]->cash << endl;
        }

        outFile.close();
        delete[] accounts;
    }

    void loadAccountsFromFile() {
        ifstream inFile("accounts.txt");
        if (!inFile) {
            cout << "No accounts file found. Starting fresh." << endl;
            return;
        }

        string line;
        getline(inFile, line); // Skip the heading line
        getline(inFile, line); // Skip the separator line

        string name;
        long long int card_no;
        int cash;

        while (inFile >> name >> card_no >> cash) {
            Node* newNode = new Node{name, card_no, 0, cash, nullptr, nullptr};
            root = insertNode(root, newNode);
        }

        inFile.close();
    }

    // Main menu
    void main_menu() {
        int Scommand;
        bool running = true;

        while (running) {
            cout << "***********" << endl;
            cout << "  MAIN MENU" << endl;
            cout << "  1. Create Account" << endl;
            cout << "  2. Login to Account" << endl;
            cout << "  3. Exit" << endl;
            cout << "***********" << endl;
            cout << "  Enter the Command: ";
            cin >> Scommand;

            switch (Scommand) {
                case 1:
                    CreateACC();
                    break;
                case 2:
                    if (LOGIN_TO_ACCOUNT())
                        user_menu();
                    break;
                case 3:
                    cout << "   Thanks for Using the ATM System" << endl;
                    running = false;
                    break;
                default:
                    cout << "  Invalid Command! Please Try Again." << endl;
            }
        }
    }

    // User menu
    void user_menu() {
        int command;
        bool running = true;

        while (running) {
            cout << "  USER MENU" << endl;
            cout << "  1. Deposit Money" << endl;
            cout << "  2. Withdraw Money" << endl;
            cout << "  3. Check Balance" << endl;
            cout << "  4. Main Menu" << endl;
            cout << "***********" << endl;
            cout << "  Enter the Command: ";
            cin >> command;

            switch (command) {
                case 1:
                    Deposit();
                    break;
                case 2:
                    Withdraw();
                    break;
                case 3:
                    Balance_Enquiry();
                    break;
                case 4:
                    running = false;
                    break;
                default:
                    cout << "  Invalid Command! Please Try Again." << endl;
            }
        }
    }
};

int main() {
    ATM atm;
    atm.main_menu();

    // atm.DisplayAccountsSortedByName();

    return 0;
}
