#pragma once
#include "BankAccount.h"

// Savings account with a simple nominal interest rate in percent per YEAR.
class SavingAccount : public BankAccount {
    double ratePercent_; // e.g., 2.5 means 2.5% annual rate

public:
    SavingAccount(double initialBalance = 0.0, double ratePercent = 0.0, std::string owner = {})
        : BankAccount(initialBalance, std::move(owner)), ratePercent_(ratePercent) {}

    double getRatePercent() const noexcept { return ratePercent_; }
    void setRatePercent(double p) noexcept { ratePercent_ = p; }

    // Apply interest for 'months' months using simple interest approximation.
    void updateMonths(int months) {
        if (months <= 0) return;
        const double years = months / 12.0;
        const double interest = balance_ * (ratePercent_ / 100.0) * years;
        balance_ += interest;
    }
};
