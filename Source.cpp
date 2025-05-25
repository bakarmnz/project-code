#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct Account
{
    int accountNumber;
    string firstName;
    string lastName;
    char accountType;
    int balance;
    string password;
};

bool checkAccount(int acn, string password)
{
    Account acc;
    bool found = false;
    ifstream record("Record_sys.txt");

    while (record >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.accountType >> acc.balance >> acc.password)
    {
        if (acn == acc.accountNumber && password == acc.password)
        {
            found = true;
            break;
        }
    }
    record.close();
    return found;
}

void viewAc()
{
    Account acc;
    int account;
    string password;
    bool found = false;
    cout << "Enter Account no: ";
    cin >> account;
    cout << "Enter password: ";
    cin >> password;
    ifstream record("Record_sys.txt");

    while (record >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.accountType >> acc.balance >> acc.password)
    {
        if (account == acc.accountNumber && password == acc.password)
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        cout << "Name of the depositor: " << acc.firstName << " " << acc.lastName << endl;
        cout << "Account Number: " << acc.accountNumber << endl;
        cout << "Account Type: " << acc.accountType << endl;
        cout << "Balance: " << acc.balance << endl;
    }
    else
    {
        cout << "Account not found or incorrect password!\n";
    }

    record.close();
}

void createAc()
{
    ofstream record("Record_sys.txt", ios::app);
    Account acc;

    cin.ignore();
    cout << " _________________________________" << endl;
    cout << "|      Create New Account         |" << endl;
    cout << "|_________________________________|" << endl;
    cout << "Please enter your first name: ";
    getline(cin, acc.firstName);
    cout << "Please enter your last name: ";
    getline(cin, acc.lastName);
    cout << "Please select account type (s/c): ";
    cin >> acc.accountType;
    cout << "Please enter initial amount: ";
    cin >> acc.balance;
    cout << "Please enter Account number: ";
    cin >> acc.accountNumber;
    cin.ignore();
    cout << "Please enter your password: ";
    getline(cin, acc.password);

    record << acc.accountNumber << " " << acc.firstName << " " << acc.lastName << " " << acc.accountType << " " << acc.balance << " " << acc.password << '\n';

    cout << "Account created successfully!" << endl;

    record.close();
}

void deposit(int acn)
{
    Account acc;
    string password;
    cout << "Enter password: ";
    cin >> password;
    if (!checkAccount(acn, password))
    {
        cout << "Account not found or incorrect password!\n";
        return;
    }
    int depositAmount;
    cout << "Enter deposit amount: ";
    cin >> depositAmount;
    ifstream record("Record_sys.txt");
    ofstream temp("Temp.txt", ios::out);

    while (record >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.accountType >> acc.balance >> acc.password)
    {
        if (acc.accountNumber == acn)
        {
            acc.balance += depositAmount;
        }
        temp << acc.accountNumber << " " << acc.firstName << " " << acc.lastName << " " << acc.accountType << " " << acc.balance << " " << acc.password << '\n';
    }

    record.close();
    temp.close();
    remove("Record_sys.txt");
    rename("Temp.txt", "Record_sys.txt");

    cout << "Deposit successful!" << endl;
}

void withdraw(int acn)
{
    Account acc;
    string password;
    cout << "Enter password: ";
    cin >> password;
    if (!checkAccount(acn, password))
    {
        cout << "Account not found or incorrect password!\n";
        return;
    }
    int withdrawAmount;
    cout << "Enter withdraw amount: ";
    cin >> withdrawAmount;

    ifstream record("Record_sys.txt");
    ofstream temp("Temp.txt", ios::out);
    bool success = false;

    while (record >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.accountType >> acc.balance >> acc.password)
    {
        if (acc.accountNumber == acn)
        {
            if (withdrawAmount > acc.balance)
            {
                cout << "Insufficient balance!\n";
                temp << acc.accountNumber << " " << acc.firstName << " " << acc.lastName << " " << acc.accountType << " " << acc.balance << " " << acc.password << '\n';
                continue;
            }
            acc.balance -= withdrawAmount;
            success = true;
        }
        temp << acc.accountNumber << " " << acc.firstName << " " << acc.lastName << " " << acc.accountType << " " << acc.balance << " " << acc.password << '\n';
    }

    record.close();
    temp.close();
    remove("Record_sys.txt");
    rename("Temp.txt", "Record_sys.txt");

    if (success)
        cout << "Withdrawal successful!" << endl;
}

void transfer(int senderAcn, int receiverAcn)
{
    Account sender, receiver;
    string senderPassword, receiverPassword;
    cout << "Enter sender account password: ";
    cin >> senderPassword;
    if (!checkAccount(senderAcn, senderPassword))
    {
        cout << "Sender account not found or incorrect password!\n";
        return;
    }

    int amount;
    cout << "Enter transfer amount: ";
    cin >> amount;

    ifstream record("Record_sys.txt");
    ofstream temp("Temp.txt", ios::out);

    while (record >> sender.accountNumber >> sender.firstName >> sender.lastName >> sender.accountType >> sender.balance >> sender.password)
    {
        if (sender.accountNumber == senderAcn)
        {
            sender.balance -= amount;
        }
        temp << sender.accountNumber << " " << sender.firstName << " " << sender.lastName << " " << sender.accountType << " " << sender.balance << " " << sender.password << '\n';
    }

    record.close();
    temp.close();
    remove("Record_sys.txt");
    rename("Temp.txt", "Record_sys.txt");

    ifstream record2("Record_sys.txt");
    ofstream temp2("Temp2.txt", ios::out);

    while (record2 >> receiver.accountNumber >> receiver.firstName >> receiver.lastName >> receiver.accountType >> receiver.balance >> receiver.password)
    {
        if (receiver.accountNumber == receiverAcn)
        {
            receiver.balance += amount;
        }
        temp2 << receiver.accountNumber << " " << receiver.firstName << " " << receiver.lastName << " " << receiver.accountType << " " << receiver.balance << " " << receiver.password << '\n';
    }

    record2.close();
    temp2.close();
    remove("Record_sys.txt");
    rename("Temp2.txt", "Record_sys.txt");

    cout << "Transfer successful!" << endl;
}

void SetColor(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + (backgroundColor * 16);
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

int main()
{
    SetColor(14, 13);
    cout << "\t\t\t\t\tWelcome to the Online Banking System" << endl;
    SetColor(7, 0);

    int choice;
    cout << "Menu:" << endl;
    while (true)
    {
        cout << "      ____________________________________________________________" << endl;
        cout << "     |                                                           |" << endl;
        cout << "     |               1 – Create a new account.                   |" << endl;
        cout << "     |               2 – View account details.                   |" << endl;
        cout << "     |               3 – Deposit.                                |" << endl;
        cout << "     |               4 – Withdraw.                               |" << endl;
        cout << "     |               5 – Transfer.                               |" << endl;
        cout << "     |               6 – Exit.                                   |" << endl;
        cout << "     |___________________________________________________________|" << endl;
        cout << endl;

        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            createAc();
            break;
        case 2:
            viewAc();
            break;
        case 3:
        {
            int acn;
            cout << "Enter your Account No: ";
            cin >> acn;
            deposit(acn);
            break;
        }
        case 4:
        {
            int acn;
            cout << "Enter your Account No: ";
            cin >> acn;
            withdraw(acn);
            break;
        }
        case 5:
        {
            int senderAcn, receiverAcn;
            cout << "Enter sender Account No: ";
            cin >> senderAcn;
            cout << "Enter receiver Account No: ";
            cin >> receiverAcn;
            transfer(senderAcn, receiverAcn);
            break;
        }
        case 6:
            return 0;
        default:
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
