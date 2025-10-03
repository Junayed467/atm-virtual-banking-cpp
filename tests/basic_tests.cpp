#include <iostream>
#include <cstdlib>
#include "../include/SavingAccount.h"
#include "../include/CheckingAccount.h"

// Minimal self-checking tests without a framework.
// Returns non-zero on failure so CI can catch it.
int main() {
    int failures = 0;

    // Withdraw/Deposit semantics
    {
        CheckingAccount c(100.0, 1.0, "t");
        if (!c.deposit(50.0)) { std::cerr << "deposit failed\n"; failures++; }
        if (!c.withdraw(25.0)) { std::cerr << "withdraw failed\n"; failures++; }
        if (std::abs(c.getBalance() - 125.0) > 1e-9) { std::cerr << "balance wrong\n"; failures++; }
    }

    // Insufficient funds
    {
        CheckingAccount c(10.0, 1.0, "t");
        if (c.withdraw(20.0)) { std::cerr << "withdraw should fail\n"; failures++; }
    }

    // Check fee
    {
        CheckingAccount c(100.0, 2.0, "t");
        bool ok = c.writeCheck(50.0);
        if (!ok) { std::cerr << "writeCheck should succeed\n"; failures++; }
        double expected = 100.0 - 52.0;
        if (std::abs(c.getBalance() - expected) > 1e-9) { std::cerr << "fee not applied\n"; failures++; }
    }

    // Savings interest (12 months at 12% on 100 => 12)
    {
        SavingAccount s(100.0, 12.0, "t");
        s.updateMonths(12);
        if (std::abs(s.getBalance() - 112.0) > 1e-9) { std::cerr << "interest wrong\n"; failures++; }
    }

    if (failures) {
        std::cerr << "FAILURES: " << failures << "\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}
