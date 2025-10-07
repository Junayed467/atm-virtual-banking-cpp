#pragma once
#include <string>
#include <stdexcept>

class BankAccount {
protected:
    std::string owner_;
    double      balance_;   // no brace-init

public:
    explicit BankAccount(double initialBalance = 0.0,
                         const std::string& owner = std::string())
        : owner_(owner), balance_(initialBalance) {
        if (initialBalance < 0.0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
    }

    virtual ~BankAccount() {}

    double              getBalance() const { return balance_; }
    const std::string&  getOwner()   const { return owner_;   }

    virtual bool deposit(double amount) {
        if (amount <= 0.0) return false;
        balance_ += amount;
        return true;
    }
    virtual bool withdraw(double amount) {
        if (amount <= 0.0) return false;
        if (amount > balance_) return false;
        balance_ -= amount;
        return true;
    }
};
