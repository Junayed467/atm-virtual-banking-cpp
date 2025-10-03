#include <iostream>
#include <limits>
#include <cctype>
#include <unordered_map>
#include <string>
#include <algorithm>

#include "SavingAccount.h"
#include "CheckingAccount.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

struct UserAccounts {
    SavingAccount savings;
    CheckingAccount checking;
};

static inline char upper(char c) {
    return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}

static double askAmount(const string& prompt) {
    cout << prompt;
    double amt{};
    while (true) {
        if (cin >> amt && amt > 0.0) return amt;
        cout << "Invalid amount. Enter a positive number: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static char askAccount(const string& prompt) {
    cout << prompt << " [S]avings or [C]hecking: ";
    char c;
    while (true) {
        if (cin >> c) {
            c = upper(c);
            if (c == 'S' || c == 'C') return c;
        }
        cout << "Please enter S or C: ";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    // Demo in-memory "database" with a single user. Extendable to many users.
    std::unordered_map<string, UserAccounts> db;
    db.emplace("demo", UserAccounts{
        SavingAccount(1000.0, 2.0, "Demo User"),
        CheckingAccount(500.0, 1.0, "Demo User")
    });

    cout << "=== ATM Virtual Banking (C++) ===\n";
    cout << "Login with username (try: demo): ";
    string username;
    if (!(cin >> username)) return 0;

    auto it = db.find(username);
    if (it == db.end()) {
        cout << "User not found.\n";
        return 0;
    }
    auto& accounts = it->second;

    bool running = true;
    while (running) {
        cout << "\nChoose transaction:\n"
             << " [B]alance, [D]eposit, [W]ithdraw, [T]ransfer, [C]heck, [U]pdate interest, [Q]uit\n"
             << "Enter choice: ";
        char t;
        if (!(cin >> t)) break;
        t = upper(t);

        switch (t) {
            case 'B': {
                char acct = askAccount("Which account to view?");
                double bal = (acct == 'S') ? accounts.savings.getBalance()
                                           : accounts.checking.getBalance();
                cout << (acct == 'S' ? "Savings" : "Checking") << " balance: " << bal << "\n";
                break;
            }
            case 'D': {
                char acct = askAccount("Deposit into which account?");
                double amt = askAmount("Amount to deposit: ");
                bool ok = (acct == 'S') ? accounts.savings.deposit(amt)
                                        : accounts.checking.deposit(amt);
                cout << (ok ? "Deposit successful.\n" : "Deposit failed.\n");
                break;
            }
            case 'W': {
                char acct = askAccount("Withdraw from which account?");
                double amt = askAmount("Amount to withdraw: ");
                bool ok = (acct == 'S') ? accounts.savings.withdraw(amt)
                                        : accounts.checking.withdraw(amt);
                cout << (ok ? "Withdrawal successful.\n" : "Insufficient funds or invalid amount.\n");
                break;
            }
            case 'T': {
                char from = askAccount("Transfer FROM which account?");
                char to   = (from == 'S') ? 'C' : 'S';
                double amt = askAmount("Amount to transfer: ");
                bool okFrom = (from == 'S') ? accounts.savings.withdraw(amt)
                                            : accounts.checking.withdraw(amt);
                if (okFrom) {
                    bool okTo = (to == 'S') ? accounts.savings.deposit(amt)
                                            : accounts.checking.deposit(amt);
                    if (okTo) {
                        cout << "Transfer successful ("
                             << (from=='S'?"Savings":"Checking") << " -> "
                             << (to=='S'?"Savings":"Checking") << ").\n";
                    } else {
                        // rollback if deposit fails (shouldn't with validation)
                        if (from == 'S') accounts.savings.deposit(amt);
                        else accounts.checking.deposit(amt);
                        cout << "Transfer failed; rolled back.\n";
                    }
                } else {
                    cout << "Transfer failed: insufficient funds.\n";
                }
                break;
            }
            case 'C': {
                double amt = askAmount("Check amount (fee will apply): ");
                bool ok = accounts.checking.writeCheck(amt);
                cout << (ok ? "Check processed.\n" : "Check failed (insufficient funds).\n");
                break;
            }
            case 'U': {
                int months;
                cout << "Accrue interest on Savings for how many months? ";
                if (cin >> months && months > 0) {
                    accounts.savings.updateMonths(months);
                    cout << "Updated savings balance: " << accounts.savings.getBalance() << "\n";
                } else {
                    cout << "Invalid months.\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                break;
            }
            case 'Q': {
                running = false;
                break;
            }
            default:
                cout << "Unknown option.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
