#pragma once
#include <string>
#include <stdexcept>

class BankAccount {
protected:
    std::string owner_;
    double balance_;

public:
    explicit BankAccount(double initialBalance = 0.0, std::string owner = {})
        : owner_(std::move(owner)), balance_(initialBalance) {
        if (initialBalance < 0.0) {
            throw std::invalid_argument("Initial balance cannot be negative");
        }
    }

    virtual ~BankAccount() = default;

    // Inspectors
    double getBalance() const noexcept { return balance_; }
    const std::string& getOwner() const noexcept { return owner_; }

    // Mutators return success; they do NOT print (UI-free domain layer)
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
