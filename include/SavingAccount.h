#pragma once
#include "BankAccount.h"

class SavingAccount : public BankAccount {
    double ratePercent_;

public:
    SavingAccount(double initialBalance = 0.0,
                  double ratePercent    = 0.0,
                  const std::string& owner = std::string())
        : BankAccount(initialBalance, owner),
          ratePercent_(ratePercent) {}

    double getRatePercent() const { return ratePercent_; }
    void   setRatePercent(double p) { ratePercent_ = p; }

    void updateMonths(int months) {
        if (months <= 0) return;
        const double years    = months / 12.0;
        const double interest = balance_ * (ratePercent_ / 100.0) * years;
        balance_ += interest;
    }
};
