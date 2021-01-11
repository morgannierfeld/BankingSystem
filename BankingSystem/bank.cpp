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
#define ACCOUNT_SEP_LINE "-----------------"
#define BAD_FILE "Unable to open file"
#define BAD_FORMAT "File is incorrectly formatted"

class Account {
    private:
        int accNum;
        string holder;
        string type;
        float balance;
    public:
        Account(int accNum, string holder, string type, float balance) {
            this->accNum = accNum;
            this->holder = holder;
            this->type = type;
            this->balance = balance;
        }

        int get_acc_num(void) {
            return accNum;
        }

        string get_holder(void) {
            return holder;
        }

        string get_type(void) {
            return type;
        }

        float get_balance(void) {
            return balance;
        }

        void set_acc_number(int num) {
            accNum = num;
        }

        void set_name(string name) {
            holder = name;
        }

        void set_acc_type(string newType) {
            type = newType;
        }

        void set_balance(float newBalance) {
            balance = newBalance;
        }

        void increase_balance(float increase) {
            printf("(1)\n");
            balance = balance + increase;
            cout << "The new balance is: " << balance << '\n';
        }

        void decrease_balance(float decrease) {
            if (balance - decrease < 0) {
                //throw exception
            } else {
                balance = balance - decrease;
            }
        }

        void display_account(void) {
            cout << "---Account Status---\n";
            cout << account_string();
        }

        string account_string(void) {
            string returnString = "Account Number: " + to_string(accNum) + '\n';
            returnString = returnString + "Account Holder Name: " + holder + '\n';
            returnString = returnString + "Type of Account: " + type + '\n';
            returnString = returnString + "Balance Amount: " + to_string(balance) + '\n';
            return returnString;
        }

};

class Bank {
    private:
        vector<Account> accounts;
        int numberOfAccounts;

    public:
        string name;

        Bank(string name) {
            this->name = name;
            numberOfAccounts = 0;
        }

        void add_account(int number, string holder, string type, float amount) {
            Account* newAcc = new Account(number, holder, type, amount);
            accounts.push_back(*newAcc);
            numberOfAccounts++;
        }

        Account* get_account(int number) {
            for (int i = 0; i < accounts.size(); i++) {
                if (number == accounts.at(i).get_acc_num()) {
                    return &accounts.at(i);
                }
            }
            //Throw new exception if not found!
        }

        void display_accounts(void) {
            for (int i = 0; i < accounts.size(); i++) {
                accounts.at(i).display_account();;
            }
        }

        vector<Account> get_accounts(void) {
            return accounts;
        }

        int get_num_of_accounts(void) {
            return numberOfAccounts;
        }

        void delete_account(int accNum) {
            for (int i = 0; i < numberOfAccounts; i++) {
                if (accNum == accounts.at(i).get_acc_num()) {
                    accounts.erase(accounts.begin() + i);
                    break;
                }
            }
            numberOfAccounts--;
            //handle account does not exist.
        }

        void set_num_of_acc(int num) {
            numberOfAccounts = num;
        }
};

void check_args(int argc) {
    if (argc != 1 && argc != 2) {
        cerr << "Incorrect number of arguments\n"
             << "Usage: ./bank savefile or ./bank\n";
        exit(1);
    }
}

/*
Returns -1 if string is invalid. No negative funds can be in accounts, and no
negative acc numbers.
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

string get_user_input(void) {
    string userInput;
    getline(cin, userInput);
    return userInput;
}

string getLine(ifstream* file) {
    string line;
    if (!getline(*file, line)) {
        cerr << BAD_FORMAT << endl;
        exit(2);
    }
    cout << "The line we just got was: " << line << endl;
    return line;
}

bool invalid_string(string input) {
    for (int i = 0; i < input.length(); i++) {
        if ((input[i] < 'A' || input[i] > 'Z')  && 
            (input[i] < 'a' || input[i] > 'z') && input[i] != ' ') {
                return true;
            }
    }
    return false;
}

Bank* load_bank(Bank* bank, string fileName) {
    ifstream loadFile;
    loadFile.open(fileName);
    if (!loadFile) {
        cerr << BAD_FILE << endl;
        exit(1);
    }
    string line = getLine(&loadFile);
    bank = new Bank(line);
    line = getLine(&loadFile);
    printf("(1)\n");
    int numOfAcc = convert_string_to_int(line);
    printf("(2)\n");
    bank->set_num_of_acc(numOfAcc);
    printf("(3)\n");
    for (int i = 0; i < numOfAcc; i++) {
        printf("(4)\n");
        line = getLine(&loadFile);
        if (line.compare(ACCOUNT_SEP_LINE)) {
            printf("(5)\n");
            line = getLine(&loadFile);
            cout << "line: " << line << endl;
            while (!line.compare(ACCOUNT_SEP_LINE)) {
                printf("(6)\n");
                int accNum = convert_string_to_int(getLine(&loadFile));
                if (accNum == -1 || accNum == -2 || accNum == 0) {
                    cerr << BAD_FORMAT << endl;
                    exit(2);
                }
                string holder = getLine(&loadFile);
                if (invalid_string(holder)) {
                    cerr << BAD_FORMAT << endl;
                    exit(2);
                }
                string type = getLine(&loadFile);
                if (!type.compare("S") && !type.compare("C")) {
                    cerr << BAD_FORMAT << endl;
                    exit(2);
                }
                float balance = convert_string_to_float(getLine(&loadFile));
                if (balance == -1 || balance == -2 || balance == 0) {
                    cerr << BAD_FILE << endl;
                    exit(2);
                }
                cout << "Acc Num: " << accNum << endl;
                cout << "Holder: " << holder << endl;
                cout << "Type: " << type << endl;
                cout << "Balance: " << balance << endl;
                bank->add_account(accNum, holder, type, balance);
            }
        } else if (line.compare("END")) {
            if (i != numOfAcc - 1) {
                cerr << BAD_FORMAT << endl;
                exit(2);
            }
        }
        
    }
    loadFile.close();
    printf("(5)\n");
    printf("number of accounts: %d\n", numOfAcc);
    vector<Account> accounts = bank->get_accounts();
    for (int i = 0; i < numOfAcc; i++) accounts.at(i).display_account();
    return bank;
}


Bank* create_bank(int argc, char** argv, Bank* bank) {
    string bankName;
    if (argc == 1) {
        cout << "Enter the name of the bank: ";
        getline(cin, bankName);
        bank = new Bank(bankName);
        return bank;
    } else {
        bank = load_bank(bank, argv[1]);
        return bank;
    }
}



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

int get_account_number(Bank* bank) {
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    return accNum;

}

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

float get_balance(string message) {
    float amount = run_question_sequence(message, 
            convert_string_to_float);
    return amount;
}

void end_action(string message) {
    cout << message;
    cout << "Press enter to continue.\n";
    fgetc(stdin);
    cout << "-------------------------------\n";
}

void account_transaction_form(Bank* bank, bool withdraw) {
    cout << "----Account Transaction Form----\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    bank->get_account(accNum)->display_account();;
    float amount;
    if (!withdraw) {
        amount = run_question_sequence("Enter the amount to deposit: ", 
                convert_string_to_float);
        bank->get_account(accNum)->increase_balance(amount);
    } else {
        amount = run_question_sequence("Enter the amount to withdraw: ", 
                convert_string_to_float);
        bank->get_account(accNum)->decrease_balance(amount);
    }
}

string add_account_num(string buffer, Account* account) {
    int accNum = account->get_acc_num();
    string accNumStr = to_string(accNum);
    buffer = buffer + accNumStr;
    buffer = buffer + string(NAME_POS - accNumStr.size(), ' ');
    return buffer;
}

string add_holder(string buffer, Account* account) {
    string holder = account->get_holder();
    buffer = buffer + holder;
    buffer = buffer + string(TYPE_POS - buffer.size(), ' ');
    return buffer;
}

string add_type(string buffer, Account* account) {
    string type = account->get_type();
    buffer = buffer + type;
    buffer = buffer + string(BALANCE_POS - buffer.size(), ' ');
    return buffer;
}

string add_balance(string buffer, Account* account) {
    float balance = account->get_balance();
    string balanceStr = to_string(balance);
    buffer = buffer + balanceStr;
    return buffer;
}

string add_details_to_string(string buffer, Account* account) {
    buffer = add_account_num(buffer, account);
    buffer = add_holder(buffer, account);
    buffer = add_type(buffer, account);
    buffer = add_balance(buffer , account);
    return buffer;
}

void new_account(Bank* bank, string message) {
    cout << message;
    int accountNumber;
    string accountHolder, accType;
    float amount;
    accountNumber = get_account_number(bank);
    accountHolder = get_account_holder("Enter the name of the account holder: ");
    accType = get_type_of_account("Enter the type of the account (type 'S' for savings or 'C' for current): ");
    amount = get_balance("Enter initial amount: ");
    bank->add_account(accountNumber, accountHolder, accType, amount);
    end_action("Account was set up succesfully\n");
}

void deposit(Bank* bank) {
    account_transaction_form(bank, false);
    end_action("Record Updated.\n");

}

void withdraw(Bank* bank) {
    account_transaction_form(bank, true);
    end_action("Record updated.\n");
}

void balance_enquiry(Bank* bank) {
    cout << "Balance Details.\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    bank->get_account(accNum)->display_account();;
    end_action("");
}

void account_holders(Bank* bank) {
    cout << "----All Account Holders List----\n";
    string banner = string(101, '=') + '\n';
    cout << banner.size() << '\n';
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

void close_account(Bank* bank) {
    cout << "----Delete Record----\n";
    int accNum = run_question_sequence("Enter the account number: ", 
            convert_string_to_int);
    bank->delete_account(accNum);
    end_action("Record Deleted\n");
}

void modify_account(Bank* bank) {
    cout << "----Modify Record----\n";
    int accNum = run_question_sequence("Enter the Account Number: ", 
            convert_string_to_int);
    Account* account = bank->get_account(accNum);
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

void quit_program(Bank* bank) {
    cout << "Exiting...\n";
    exit(0);
}

void save(Bank* bank) {
    cout << "----Save Bank Status-----\n";
    cout << "Enter the name of the file: ";
    string fileName = get_user_input();
    ofstream saveFile;
    saveFile.open(fileName);
    if (!saveFile) {
        cerr << BAD_FILE << endl;
        exit(1);
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
    bank = create_bank(argc, argv, bank);
    run_bank(bank);
    return 0;
}

/*
To do:
- Finish off the loading bank from save file
- Create exceptions for account not found
- Document code properly

*/