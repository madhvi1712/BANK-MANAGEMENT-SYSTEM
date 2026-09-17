#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>

using namespace std;

class BankAccount
{
private:
    int accNumber;
    string holderName;
    double amount;

public:

    BankAccount()
    {
        accNumber = 0;
        holderName = "";
        amount = 0;
    }

    BankAccount(int no, string n, double bal)
    {
        accNumber = no;
        holderName = n;
        amount = bal;
    }

    int getAccountNumber()
    {
        return accNumber;
    }

    string getName()
    {
        return holderName;
    }

    double getBalance()
    {
        return amount;
    }

    void deposit(double money)
    {
        if (money > 0)
        {
            amount += money;
            cout << "\nAmount deposited successfully.\n";
        }
        else
        {
            cout << "\nInvalid amount.\n";
        }
    }

    bool withdraw(double money)
    {
        if (money <= 0)
        {
            cout << "\nInvalid amount.\n";
            return false;
        }

        if (money > amount)
        {
            cout << "\nInsufficient balance.\n";
            return false;
        }

        amount -= money;
        cout << "\nAmount withdrawn successfully.\n";
        return true;
    }

    void display()
    {
        cout << "\nAccount Number : " << accNumber << endl;
        cout << "Account Holder : " << holderName << endl;
        cout << "Balance        : Rs. "
             << fixed << setprecision(2)
             << amount << endl;
    }
};

class BankSystem
{
private:
    const string dataFile = "accounts.txt";

    void saveAccount(BankAccount userAccount)
    {
        ofstream file(dataFile, ios::app);

        if (file.is_open())
        {
            file << userAccount.getAccountNumber() << "|"
                 << userAccount.getName() << "|"
                 << userAccount.getBalance() << endl;

            file.close();
        }
        else
        {
            cout << "Error opening file.\n";
        }
    }

    BankAccount findAccount(int searchNumber)
    {
        ifstream file(dataFile);

        int number;
        string personName;
        double money;
        string separator;

        while (file >> number)
        {
            getline(file, separator, '|');
            getline(file, personName, '|');
            file >> money;

            if (number == searchNumber)
            {
                file.close();

                return BankAccount(
                    number,
                    personName,
                    money
                );
            }
        }

        file.close();

        return BankAccount();
    }

    bool accountExists(int searchNumber)
    {
        ifstream file(dataFile);

        int number;
        string personName;
        double money;
        string separator;

        while (file >> number)
        {
            getline(file, separator, '|');
            getline(file, personName, '|');
            file >> money;

            if (number == searchNumber)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void updateAccount(BankAccount newAccount)
    {
        ifstream file(dataFile);
        ofstream tempFile("temp.txt");

        int number;
        string personName;
        double money;
        string separator;

        while (file >> number)
        {
            getline(file, separator, '|');
            getline(file, personName, '|');
            file >> money;

            if (number == newAccount.getAccountNumber())
            {
                tempFile << newAccount.getAccountNumber()
                         << "|"
                         << newAccount.getName()
                         << "|"
                         << newAccount.getBalance()
                         << endl;
            }
            else
            {
                tempFile << number << "|"
                         << personName << "|"
                         << money << endl;
            }
        }

        file.close();
        tempFile.close();

        remove(dataFile.c_str());
        rename("temp.txt", dataFile.c_str());
    }

public:

    void createAccount()
    {
        int newNumber;
        string newName;
        double openingAmount;

        cout << "\nEnter Account Number: ";
        cin >> newNumber;

        if (accountExists(newNumber))
        {
            cout << "Account already exists.\n";
            return;
        }

        cin.ignore();

        cout << "Enter Account Holder Name: ";
        getline(cin, newName);

        cout << "Enter Initial Deposit: Rs. ";
        cin >> openingAmount;

        if (openingAmount < 0)
        {
            cout << "Invalid initial deposit.\n";
            return;
        }

        BankAccount newUser(
            newNumber,
            newName,
            openingAmount
        );

        saveAccount(newUser);

        cout << "Account created successfully.\n";
    }

    void depositMoney()
    {
        int searchNumber;
        double depositAmount;

        cout << "\nEnter Account Number: ";
        cin >> searchNumber;

        BankAccount user = findAccount(searchNumber);

        if (user.getAccountNumber() == 0)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter Deposit Amount: Rs. ";
        cin >> depositAmount;

        user.deposit(depositAmount);

        updateAccount(user);
    }

    void withdrawMoney()
    {
        int searchNumber;
        double withdrawAmount;

        cout << "\nEnter Account Number: ";
        cin >> searchNumber;

        BankAccount user = findAccount(searchNumber);

        if (user.getAccountNumber() == 0)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter Withdrawal Amount: Rs. ";
        cin >> withdrawAmount;

        if (user.withdraw(withdrawAmount))
        {
            updateAccount(user);
        }
    }

    void checkBalance()
    {
        int searchNumber;

        cout << "\nEnter Account Number: ";
        cin >> searchNumber;

        BankAccount user = findAccount(searchNumber);

        if (user.getAccountNumber() == 0)
        {
            cout << "Account not found.\n";
            return;
        }

        cout << "\nCurrent Balance: Rs. "
             << fixed << setprecision(2)
             << user.getBalance()
             << endl;
    }

    void displayAccount()
    {
        int searchNumber;

        cout << "\nEnter Account Number: ";
        cin >> searchNumber;

        BankAccount user = findAccount(searchNumber);

        if (user.getAccountNumber() == 0)
        {
            cout << "Account not found.\n";
            return;
        }

        user.display();
    }

    void displayAllAccounts()
    {
        ifstream file(dataFile);

        int number;
        string personName;
        double money;
        string separator;

        bool foundAccount = false;

        cout << "\n========== ALL ACCOUNTS ==========\n";

        while (file >> number)
        {
            getline(file, separator, '|');
            getline(file, personName, '|');
            file >> money;

            cout << "\nAccount Number : " << number;
            cout << "\nAccount Holder : " << personName;
            cout << "\nBalance        : Rs. "
                 << fixed << setprecision(2)
                 << money << endl;

            foundAccount = true;
        }

        file.close();

        if (!foundAccount)
        {
            cout << "No accounts found.\n";
        }
    }

    void run()
    {
        int option;

        do
        {
            cout << "\n====================================\n";
            cout << "       BANK MANAGEMENT SYSTEM\n";
            cout << "====================================\n";

            cout << "1. Create Account\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Check Balance\n";
            cout << "5. Display Account\n";
            cout << "6. Display All Accounts\n";
            cout << "7. Exit\n";

            cout << "====================================\n";
            cout << "Enter your choice: ";
            cin >> option;

            switch (option)
            {
                case 1:
                    createAccount();
                    break;

                case 2:
                    depositMoney();
                    break;

                case 3:
                    withdrawMoney();
                    break;

                case 4:
                    checkBalance();
                    break;

                case 5:
                    displayAccount();
                    break;

                case 6:
                    displayAllAccounts();
                    break;

                case 7:
                    cout << "\nThank you for using Bank Management System.\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (option != 7);
    }
};

int main()
{
    BankSystem system;
    system.run();

    return 0;
}