#pragma once
#include "BankAccount.h"

// Checking account that charges a flat fee per check/transfer-like operation.
class CheckingAccount : public BankAccount {
    double feePerCheck_;

public:
    CheckingAccount(double initialBalance = 0.0, double feePerCheck = 0.0, std::string owner = {})
        : BankAccount(initialBalance, std::move(owner)), feePerCheck_(feePerCheck) {}

    double getFeePerCheck() const noexcept { return feePerCheck_; }
    void setFeePerCheck(double f) noexcept { feePerCheck_ = f; }

    // A "check" is modeled as withdrawing amount + fee atomically.
    bool writeCheck(double amount) {
        const double total = amount + feePerCheck_;
        return withdraw(total);
    }
};
