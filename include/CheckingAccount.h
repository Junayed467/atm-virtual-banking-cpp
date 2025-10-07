#pragma once
#include "BankAccount.h"

// Simple checking account with a per-check fee (e.g., $2.00).
class CheckingAccount : public BankAccount {
    double perCheckFee_;
public:
    CheckingAccount(double initialBalance = 0.0,
                    double perCheckFee     = 0.0,
                    const std::string& owner = std::string())
        : BankAccount(initialBalance, owner),
          perCheckFee_(perCheckFee) {}

    double fee() const { return perCheckFee_; }
    void   setFee(double f) { perCheckFee_ = f; }

    // Write a check for 'amount' -> withdraw amount + fee atomically.
    // Returns true if it succeeds without overdrawing.
    bool writeCheck(double amount) {
        if (amount <= 0.0) return false;
        const double total = amount + perCheckFee_;
        if (total > balance_) return false;
        balance_ -= total;
        return true;
    }
};
