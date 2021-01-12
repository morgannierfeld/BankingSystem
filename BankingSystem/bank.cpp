#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <iterator>
#include <fstream>

using namespace std;

#define NAME_POS 33
#define TYPE_POS 62
#define BALANCE_POS 91
#define ACCOUNT_SEP_LINE "---------------"
#define BAD_FILE "Unable to open file"
#define BAD_FORMAT "File is incorrectly formatted"
#define NORMAL_EXIT 0
#define BAD_ARGS 1
#define CANNOT_OPEN_FILE 2
#define BAD_FILE_FORMAT 3

/*
Exception to handle when no account is able to be found.
*/
struct AccountNotFoundException : public std::exception {
    const char* what() const throw() {
        return "Account not found";
    }
};

/*
Exception to handle when there is an attempt to withdraw 
more money than the current balance.
*/
struct NegativeBalanceException : public std::exception {
    const char* what() const throw() {
        return "Withdraw will cause balance to fall below zero";
    }
};

/*
Exception to handle when there is an attempt to add an account
with an account number that already exists. 
*/
struct AccountAlreadyExistsException : public std::exception {
    const char* what() const throw() {
        return "Account number already exists";
    }
};

/*
Object to represent a single bank account. All account numbers
must be unique. Balances cannot be 0 or below. Holder names are
only allowed alphabetical letters and spaces.
*/
class Account {
    private:
        /*Private member variable for the account number.*/
        int accNum;
        /*Private member variable for the account holder.*/
        string holder;
        /*Private member variable for the account type.*/
        string type;
        /*Private member variable for the account balance.*/
        float balance;
    public:
        /*
        Instantiates a new account that stores the account number,
        holder, type and balance.
        Params:
            - accNum: account number
            - holder: name of the holder of the account
            - type: the type this account is. (S or C).
            - balance: the balance of the account.
        */
        Account(int accNum, string holder, string type, float balance) {
            this->accNum = accNum;
            this->holder = holder;
            this->type = type;
            this->balance = balance;
        }

        /*
        Method to return the account number.
        Params:
            - void
        Returns:
            - Account number
        */
        int get_acc_num(void) {
            return accNum;
        }

        /*
        Method to return the account holder
        Params:
            - void
        Returns:
            - Account holder.
        */
        string get_holder(void) {
            return holder;
        }

        /*
        Method to return the account type
        Params:
            - void
        Returns:
            - Account type
        */
        string get_type(void) {
            return type;
        }

        /*
        Method to return the account balance
        Params:
            - void
        Returns:
            - Account balance
        */
        float get_balance(void) {
            return balance;
        }

        /*
        Method to set the new account number for
        this account object after the account has 
        been modified.
        Params:
            - num: new account number
        Returns:
            - void.
        */
        void set_acc_number(int num) {
            accNum = num;
        }

        /*
        Method to set the new account holder for
        this account object after the acconut has
        been modified.
        Params:
            - name: new name of the acc. holder
        Returns:
            - void.
        */
        void set_name(string name) {
            holder = name;
        }

        /*
        Method to set the new account holder for
        this account object after the acconut has
        been modified.
        Params:
            - name: new name of the acc. holder
        Returns:
            - void
        */
        void set_acc_type(string newType) {
            type = newType;
        }

        /*
        Method to set the new account balance for
        this account object after the acconut has
        been modified.
        Params:
            - newBalance: new account balance
        Returns:
            - void
        */
        void set_balance(float newBalance) {
            balance = newBalance;
        }

        /*
        Method to increase the balance after depositing
        money into the account.
        Params:
            - increase: amount deposited into the account
        Returns:
            - void
        */
        void increase_balance(float increase) {
            balance = balance + increase;
        }

        /*
        Method to decrease the balance after withdrawing
        money from the account. 
        Params:
            - decrease: amount withdrawn from the account
        Returns:
            - void.
        Throws:
            - NegativeBalanceException.
        */
        void decrease_balance(float decrease) {
            if (balance - decrease < 0) {
                throw NegativeBalanceException();
            } else {
                balance = balance - decrease;
            }
        }

        /*
        Method to display the account information to the terminal.
        Params:
            - void
        Returns:
            - void.
        */
        void display_account(void) {
            cout << "---Account Status---\n";
            cout << "Account Number: " << to_string(accNum) << endl;
            cout << "Account Holder Name: " << holder << endl;
            cout << "Type of Account: " << type << endl;
            cout << "Balance Amount: " << to_string(balance) << endl;
        }

        /*
        Method to create a string of the account and it's information.
        Params:
            - void
        Returns:
            - string representation of this account.
        */
        string account_string(void) {
            string returnString = to_string(accNum) + '\n';
            returnString = returnString + holder + '\n';
            returnString = returnString + type + '\n';
            returnString = returnString + to_string(balance) + '\n';
            return returnString;
        }

};

/*
Class to represent a single bank object that holds multiple
account objects.
*/
class Bank {
    private:
        /*Private member variable vector to store the bank accounts.*/
        vector<Account> accounts;
        /*Private member variable to track the number of accounts stored for this bank.*/
        int numberOfAccounts;

    public:
        /*Public member variable to store the name of the bank.*/
        string name;

        /*
        Instantiates a new Bank Object with the specified name
        Params:
            - name: name of the bank.
        */
        Bank(string name) {
            this->name = name;
            numberOfAccounts = 0;
        }

        /*
        Method to add an account within the bank.
        Params:
            - number: account number of new account
            - holder: holder of the new account
            - type: type of the new account
            - amount: initial balance of the account
        Returns:
            - void
        */
        void add_account(int number, string holder, string type, float amount) {
            for (int i = 0; i < numberOfAccounts; i++) {
                if (accounts.at(i).get_acc_num() == number) {
                    throw AccountAlreadyExistsException();
                }
            }
            Account* newAcc = new Account(number, holder, type, amount);
            accounts.push_back(*newAcc);
            numberOfAccounts++;
        }

        /*
        Given an account number, this method finds that account stored
        within the bank and returns a pointer to that object. If no
        such account exists with such an account number in the bank, an
        AccountNotFoundException is thrown.
        Params:
            - number: Account number
        Returns:
            - Pointer to the account object
        Throws:
            - AccountNotFoundException
        */
        Account* get_account(int number) {
            for (int i = 0; i < accounts.size(); i++) {
                if (number == accounts.at(i).get_acc_num()) {
                    return &accounts.at(i);
                }
            }
            throw AccountNotFoundException();
        }

        /*
        Method to display all accounts to the terminal.
        Params:
            - void
        Returns:
            - void
        */
        void display_accounts(void) {
            for (int i = 0; i < accounts.size(); i++) {
                accounts.at(i).display_account();;
            }
        }

        /*
        Method to return the vector that stores the accounts
        Params:
            - void
        Returns:
            - The vector that stores the accounts.
        */
        vector<Account> get_accounts(void) {
            return accounts;
        }

        /*
        Method to return the number of accounts within the bank
        Params:
            - void
        Returns:
            - Number of accounts within the bank.
        */
        int get_num_of_accounts(void) {
            return numberOfAccounts;
        }

        /*
        Method to delete an account within the bank. If no such
        account can be matched the requested account number than
        an AccountNotFoundException will be thrown.
        Params:
            - accNum: number of the account to be deleted.
        
        */
        void delete_account(int accNum) {
            bool found = false;
            for (int i = 0; i < numberOfAccounts; i++) {
                if (accNum == accounts.at(i).get_acc_num()) {
                    accounts.erase(accounts.begin() + i);
                    found = true;
                    break;
                }
            }
            if (found) {
                numberOfAccounts--;
            } else {
                throw AccountNotFoundException();
            }
        }
};

/*
Function to check that the number of arguments
put into the command line are correcnt. Function
will exit the program if the number of args are 
incorrect.
Params:
    - argc: number of arguments on the command line
Returns:
    - void
*/
void check_args(int argc) {
    if (argc != 1 && argc != 2) {
        cerr << "Incorrect number of arguments\n"
             << "Usage: ./bank savefile or ./bank\n";
        exit(BAD_ARGS);
    }
}

/*
Function to convert a string representation of a number
to type int.
Params:
    - numStr: string representation of a number
Returns:
    - The number of type int. If the number is invalid,
    this function will return -1, else if the number is 
    less then 0, it will return -2.
*/
int convert_string_to_int(string numStr) {
    int number;
    try {
        number = stoi(numStr);
    } catch (invalid_argument ia) {
        return -1;
    }
    if (number < 0) {
        return -2;
    }
    return number;
}

/*
Function to convert a string representation of a number
to type float.
Params:
    - numStr: string representation of a number
Returns:
    - The number of type float. If the number is invalid,
    this function will return -1, else if the number is 
    less then 0, it will return -2.
*/
float convert_string_to_float(string numStr) {
    float number;
    try {
        number = stof(numStr);
    } catch (invalid_argument ia) {
        return -1;
    }
    if (number < 0) {
        return -2;
    }
    return number;
}

/*
This function gets user input from the terminal
and returns a string of that input.
Params:
    - void
Returns:
    - A string of the user input.
*/
string get_user_input(void) {
    string userInput;
    getline(cin, userInput);
    return userInput;
}

/*
Reads a single line from a file. If that file is badly
formatted or cannot be read, this function will close
the program.
Params:
    - file: pointer to the opened file to read from
Returns:
    - A string of the line read.
*/
string getLine(ifstream* file) {
    string line;
    if (!getline(*file, line)) {
        cerr << BAD_FORMAT << endl;
        exit(BAD_FILE_FORMAT);
    }
    return line;
}

/*
Checks to see if the given string is valid to be 
used as a holder's name
Params:
    - input: string to be checked
Returns:
    - bool true if the string is invalid
    - bool false if the string is valid.
*/
bool invalid_string(string input) {
    for (int i = 0; i < input.length(); i++) {
        if ((input[i] < 'A' || input[i] > 'Z')  && 
            (input[i] < 'a' || input[i] > 'z') && input[i] != ' ') {
                return true;
            }
    }
    return false;
}

/*
Loads a bank off a given filename. If there is an issue with
the format of the file at any point. The function will cause
the program to exit.
Params:
    - filename: name of the save file of the bank data
Returns:
    - A pointer to a bank object that is created within this function
    and has all the data from the savefile loaded onto it.
*/
Bank* load_bank(string fileName) {
    ifstream loadFile;
    loadFile.open(fileName);
    if (!loadFile) {
        cerr << BAD_FILE << endl;
        exit(CANNOT_OPEN_FILE);
    }
    string line = getLine(&loadFile);
    Bank* bank;
    bank = new Bank(line);
    line = getLine(&loadFile);
    int numOfAcc = convert_string_to_int(line);
    if (numOfAcc == -1 || numOfAcc == -2) {
        cerr << BAD_FORMAT << endl;
        exit(BAD_FILE_FORMAT);
    }
    for (int i = 0; i < numOfAcc; i++) {
        line = getLine(&loadFile);
        if (line.compare(ACCOUNT_SEP_LINE) == 0) {
            int accNum = convert_string_to_int(getLine(&loadFile));
            if (accNum == -1 || accNum == -2 || accNum == 0) {
                cerr << BAD_FORMAT << endl;
                exit(BAD_FILE_FORMAT);
            }
            string holder = getLine(&loadFile);
            if (invalid_string(holder)) {
                cerr << BAD_FORMAT << endl;
                exit(BAD_FILE_FORMAT);
            }
            string type = getLine(&loadFile);
            if (type.compare("S") != 0 && type.compare("C") != 0) {
                cerr << BAD_FORMAT << endl;
                exit(BAD_FILE_FORMAT);
            }
            float balance = convert_string_to_float(getLine(&loadFile));
            if (balance == -1 || balance == -2 || balance == 0) {
                cerr << BAD_FILE << endl;
                exit(BAD_FILE_FORMAT);
            }
            bank->add_account(accNum, holder, type, balance);
        } else if (line.compare("END") == 0) {
            cout << "i:" << i << endl;
            if (i != numOfAcc - 1) {
                cerr << BAD_FORMAT << endl;
                exit(BAD_FILE_FORMAT);
            }
        }
    }
    loadFile.close();
    return bank;
}

/*
Creates a bank object based on the whether the bank
is initialised with a save file or no save file.
Params:
    - argc: number of input arguments
    - argv: for the arguments
Returns:
    - A pointer to a bank object created within this funciton.
*/
Bank* create_bank(int argc, char** argv) {
    string bankName;
    Bank* bank;
    if (argc == 1) {
        cout << "Enter the name of the bank: ";
        getline(cin, bankName);
        bank = new Bank(bankName);
        return bank;
    } else {
        bank = load_bank(argv[1]);
        return bank;
    }
}


/*
Functino to querry the user until they have put in the correct
input for a number. The generic T is used for ints and floats.
Params:
    - message: message to display when querrying the user
    - function pointer: represents a function that takes in a
    string and returns a type T. Used for functions
    convert_string_to_int and convert_string_to_float.
Returns:
    - Number type (float or int) of the input given by the user.
*/
template <typename T>
T run_question_sequence(string message, T (*foo)(string)) {
    T num;
    while (true) {
        cout << message;
        string userInputStr = get_user_input();
        num = foo(userInputStr);
        if (num == -1) {
            cout << "Please enter a valid number.\n";
            continue;
        } else if (num == 0) {
            cout << "Please enter a non-zero account number.\n";
            continue;
        } else if (num == -2) {
            cout << "Please enter a non-negative account number.\n";
            continue;
        }
        break;
    }
    return num;
}

/*
Querries the user for the account number of the new account
Params:
    - Pointer to the bank object
Returns:
    - The int representation of the number given by the user
*/
int get_account_number(Bank* bank) {
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    return accNum;

}

/*
Querries the user for the holder of the new account
Params:
    - message: message to querry the user
Returns:
    - String of the account holder.
*/
string get_account_holder(string message) {
    string accHolder;
    while (true) {
        cout << message;
        accHolder = get_user_input();
        if (invalid_string(accHolder)) {
            cout << 
            "Please enter a holder name containing the characters between A-Z and a-z\n";
            continue;
        }
        break;
    }
    return accHolder;
}

/*
Querries the user for the type of the new account
Params:
    - message: message to display to the user when
    querrying them.
Returns:
    - String of the type of the account.
*/
string get_type_of_account(string message) {
    string accType;
    while (true) {
        cout << message;
        accType = get_user_input();
        if (accType.compare("S") != 0 && accType.compare("C") != 0) {
            cout << "Please enter S for savings or C for current account\n";
            continue;
        }
        break;   
    }
    return accType;
}

/*
Querries the user for the balance of the new account
Params:
    - message: message to display to the user when
    querrying them.
Returns:
    - Balance of the account in type float.
*/
float get_balance(string message) {
    float amount = run_question_sequence(message, 
            convert_string_to_float);
    return amount;
}

/*
Function that forces the user to press enter to see
the status of the action just completed before
entering back to the main screen.
Params:
    - message: message to display to the user at
    the end of an action (i.e. account withdrawal,
    deposit, etc.)
Returns:
    - void
*/
void end_action(string message) {
    cout << message;
    cout << "Press enter to continue.\n";
    fgetc(stdin);
    cout << string(31, '-') << endl;
}

/*
Performs an account transaction for either a deposit
or a withdrawl.
Params:
    - bank: pointer to the main bank object
    - withdraw: flag to indicate whehter the account
    transaction is a deposit or withdraw. True if
    the transaction is a withdraw, false otherwise.
Returns:
    - void
*/
void account_transaction_form(Bank* bank, bool withdraw) {
    cout << "----Account Transaction Form----\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    bank->get_account(accNum)->display_account();
    float amount;
    if (!withdraw) {
        amount = run_question_sequence("Enter the amount to deposit: ", 
                convert_string_to_float);
        bank->get_account(accNum)->increase_balance(amount);
    } else {
        while (true) {
            amount = run_question_sequence("Enter the amount to withdraw: ", 
                        convert_string_to_float);
            try {
                bank->get_account(accNum)->decrease_balance(amount);
                break;
            } catch (NegativeBalanceException &nb) {
                cout << "Insufficient funds.\n";
            }
        }
    }
}

/*
Given a string (buffer) adds the account number to this string
Params:
    - buffer: string to add the account number to.
    - account: the account which the account number comes from
Returns:
    - The buffer string with the concatenated account number.
*/
string add_account_num(string buffer, Account* account) {
    int accNum = account->get_acc_num();
    string accNumStr = to_string(accNum);
    buffer = buffer + accNumStr;
    buffer = buffer + string(NAME_POS - accNumStr.size(), ' ');
    return buffer;
}

/*
Given a string (buffer) adds the account holder to this string
Params:
    - buffer: string to add the account holder to.
    - account: the account which the account holder comes from
Returns:
    - The buffer string with the concatenated account holder.
*/
string add_holder(string buffer, Account* account) {
    string holder = account->get_holder();
    buffer = buffer + holder;
    buffer = buffer + string(TYPE_POS - buffer.size(), ' ');
    return buffer;
}

/*
Given a string (buffer) adds the account type to this string
Params:
    - buffer: string to add the account type to.
    - account: the account which the account type comes from
Returns:
    - The buffer string with the concatenated account type.
*/
string add_type(string buffer, Account* account) {
    string type = account->get_type();
    buffer = buffer + type;
    buffer = buffer + string(BALANCE_POS - buffer.size(), ' ');
    return buffer;
}

/*
Given a string (buffer) adds the account balance to this string
Params:
    - buffer: string to add the account balance to.
    - account: the account which the account balance comes from
Returns:
    - The buffer string with the concatenated account balance.
*/
string add_balance(string buffer, Account* account) {
    float balance = account->get_balance();
    string balanceStr = to_string(balance);
    buffer = buffer + balanceStr;
    return buffer;
}

/*
Adds the details of the account to the string buffer.
Params:
    - buffer: string to add the account details to
    - account: account in which the details are to
    be added from
Returns:
    - The buffer string with the account details concatenated
    onto the end of it.
*/
string add_details_to_string(string buffer, Account* account) {
    buffer = add_account_num(buffer, account);
    buffer = add_holder(buffer, account);
    buffer = add_type(buffer, account);
    buffer = add_balance(buffer , account);
    return buffer;
}

/*
Determines whether or not the new account number from the user
is unique and is not already in use.
Params:
    - accNum: number requested for the new account
    - bank: pointer to the main bank object
Returns:
    - True if the requested account number is already in use
    - False otherwise.
*/
bool not_unique(Bank* bank, int accNum) {
    vector<Account> accounts = bank->get_accounts();
    for (int i = 0; i < bank->get_num_of_accounts(); i++) {
        if (accounts.at(i).get_acc_num() == accNum) {
            return true;
        }
    }
    return false;
}

/*
Creates a new account for the banking system. Will request user
input regarding the account details.
Params:
    - bank: pointer to the main bank object
    - message: initial message to display to the terminal.
Returns:
    - void
*/
void new_account(Bank* bank, string message) {
    cout << message;
    int accountNumber;
    string accountHolder, accType;
    float amount;
    accountNumber = get_account_number(bank);
    if (not_unique(bank, accountNumber)) {
        end_action("An account with the account number " + to_string(accountNumber) + " already exists\n");
        return;
    }
    accountHolder = get_account_holder("Enter the name of the account holder: ");
    accType = get_type_of_account("Enter the type of the account (type 'S' for savings or 'C' for current): ");
    amount = get_balance("Enter initial amount: ");
    bank->add_account(accountNumber, accountHolder, accType, amount);
    end_action("Account was set up succesfully\n");
}

/*
Performs a deposit into the requested bank account. Will
querry the user to get the account details, and the amount
to deposit. If no account is found, no operation is performed.
Params:
    - bank: pointer to the main bank object.
Returns:
    - void.
*/
void deposit(Bank* bank) {
    try {
        account_transaction_form(bank, false);
        end_action("Record Updated.\n");
    } catch (AccountNotFoundException &e) {
        end_action("Could not find the requested account.\n");
    }
}

/*
Performs a withdraw into the requested bank account. Will
querry the user to get the account details, and the amount
to deposit. If no account is found, no operation is performed.
Params:
    - bank: pointer to the main bank object.
Returns:
    - void.
*/
void withdraw(Bank* bank) {
    try {
        account_transaction_form(bank, true);
        end_action("Record updated.\n");
    } catch (AccountNotFoundException &e) {
        end_action("Could not find the account requested\n");
    }
}

/*
Performs the balance enquiry operation for the banking system.
Displays all information of the requested account to the terminal.
If no account is found, no operation is performed.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void balance_enquiry(Bank* bank) {
    cout << "Balance Details.\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    try {
        bank->get_account(accNum)->display_account();
    } catch (AccountNotFoundException &e) {
        end_action("The account " + to_string(accNum) + " does not exist\n");
        return;
    }
    end_action("");
}

/*
Displays the account information of all accounts to the terminal.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void account_holders(Bank* bank) {
    cout << "----All Account Holders List----\n";
    string banner = string(101, '=') + '\n';
    cout << banner;
    string header = "Acc. No" + string(25, ' ') + 
            "Name" + string(25, ' ') + "Type" + string(25, ' ') + "Balance\n";
    cout << header;
    cout << banner;
    string buffer = "";
    vector<Account> accounts = bank->get_accounts();
    int numOfAcc = bank->get_num_of_accounts();
    for (int i = 0; i < numOfAcc; i++) {
        Account* account = &accounts.at(i);
        buffer = add_details_to_string(buffer, account);
        cout << buffer << '\n';
        buffer = "";
    }
}

/*
Closes a bank account. Will querry the user and retrieve
the account informaiton. If no account can be found then
no operation is performed.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void close_account(Bank* bank) {
    cout << "----Delete Record----\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    try {
        bank->delete_account(accNum);
        end_action("Record Deleted\n");
    } catch (AccountNotFoundException &e) {
        cerr << "Account " << accNum << " does not exist." << endl;
        end_action("");
    }
}

/*
Modifies an existing bank account. All details (account no., 
holder, type, balance) are requested from the user. If no account 
is found, no operation is performed.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void modify_account(Bank* bank) {
    cout << "----Modify Record----\n";
    int accNum = run_question_sequence("Enter the Account Number: ", 
            convert_string_to_int);
    Account* account;
    try {
        account = bank->get_account(accNum);
    } catch (AccountNotFoundException &e) {
        end_action("The account " + to_string(accNum) + " does not exist\n");
        return;
    }
    account->display_account();;
    int newAccNum = get_account_number(bank);
    account->set_acc_number(newAccNum);
    string newName = get_account_holder("Modify Account Holder Name: ");
    account->set_name(newName);
    string newAccType = get_type_of_account("Modify Type of Account: ");
    account->set_acc_type(newAccType);
    float newBalance = get_balance("Modify Balance Amount: ");
    account->set_balance(newBalance);
    end_action("Record Updated\n");
}

/*
Quits the program when requested from the user in the main menu.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void quit_program(Bank* bank) {
    cout << "Exiting...\n";
    exit(NORMAL_EXIT);
}

/*
When requested by the user in the main menu, this function saves
the status of the bank into a text file requested by the user.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void save(Bank* bank) {
    cout << "----Save Bank Status-----\n";
    cout << "Enter the name of the file: ";
    string fileName = get_user_input();
    ofstream saveFile;
    saveFile.open(fileName);
    if (!saveFile) {
        cerr << BAD_FILE << endl;
        exit(CANNOT_OPEN_FILE);
    }
    saveFile << bank->name << endl;
    saveFile << bank->get_num_of_accounts() << endl;
    saveFile << ACCOUNT_SEP_LINE << endl;
    int numOfAcc = bank->get_num_of_accounts();
    vector<Account> accounts = bank->get_accounts();
    for (int i = 0; i < numOfAcc; i++) {
        saveFile << accounts.at(i).account_string();
        if (i != numOfAcc - 1) {
            saveFile << ACCOUNT_SEP_LINE << endl;
        }
    }
    saveFile << "END";
    saveFile.close();
}

/*
Function to handle the request from the user
Params:
    - inputNum: number which the user has selected
    - bank: pointer to the main bank object
Returns:
    - void
*/
void handle_input(int inputNum, Bank* bank) {
    switch (inputNum) {
        case 1: new_account(bank, "----New Account Entry Form----\n"); break;
        case 2: deposit(bank); break;
        case 3: withdraw(bank); break;
        case 4: balance_enquiry(bank); break;
        case 5: account_holders(bank); break;
        case 6: close_account(bank); break;
        case 7: modify_account(bank); break;
        case 8: quit_program(bank); break;
        case 9: save(bank); break;
    }
}

/*
Performs the main menu operation for the banking system.
Will display the options then querry the user for their
selection. It will then go onto to execute that operation.
Params:
    - bank: pointer to the main bank object
Returns:
    - void
*/
void run_bank(Bank* bank) {
    string mainMenu = "Main Menu:\n1. New Account\n2. Deposit Amount\n3. \
Withdraw Amount\n4. Balance Enquiry\n5. All Account Holders List\n6. Close \
An Account\n7. Modify An Account\n8. Exit\n9. Save Bank Status\nSelect your \
option (1-8)\n";
    string errMessage = "Please enter a number between 1 to 9\n";
    string input;
    int inputNum;
    while (true) {
        cout << bank->name << "\n";
        cout << mainMenu;
        getline(cin, input);
        inputNum = convert_string_to_int(input);
        if (inputNum < 1 || inputNum > 9) {
            cout << errMessage;
        }
        handle_input(inputNum, bank);
    }
}

int main(int argc, char** argv) {
    check_args(argc);
    Bank* bank;
    bank = create_bank(argc, argv);
    run_bank(bank);
    return NORMAL_EXIT;
}