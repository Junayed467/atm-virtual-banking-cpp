#include <cassert>
#include <cstdio>
#include "../include/BankAccount.h"
#include "../include/SavingAccount.h"

static bool transfer(BankAccount& a, BankAccount& b, double amt) {
    if (amt <= 0.0) return false;
    if (!a.withdraw(amt)) return false;
    if (!b.deposit(amt)) { a.deposit(amt); return false; }
    return true;
}

int main() {
    // Construction
    {
        BankAccount a(100.0, "A");
        assert(a.getBalance() == 100.0);
        try { BankAccount x(-1.0, "X"); assert(false); }
        catch (const std::invalid_argument&) {}
    }

    // Deposits / Withdrawals
    {
        BankAccount a(100.0, "A");
        assert(!a.deposit(-1));
        assert(!a.deposit(0));
        assert(a.deposit(50) && a.getBalance() == 150.0);
        assert(!a.withdraw(-1));
        assert(!a.withdraw(0));
        assert(!a.withdraw(1000));
        assert(a.withdraw(20) && a.getBalance() == 130.0);
    }

    // Transfer conservation (no fees)
    {
        BankAccount a(200, "A"), b(50, "B");
        double before = a.getBalance() + b.getBalance();
        assert(transfer(a, b, 80));
        assert(a.getBalance() == 120);
        assert(b.getBalance() == 130);
        assert(a.getBalance() + b.getBalance() == before);
    }

    // Saving interest math
    {
        SavingAccount s(1200.0, 12.0, "S");
        s.updateMonths(6); // 6 months at 12% APR = +72
        double v = s.getBalance();
        assert(v > 1271.9 && v < 1272.1);
    }

    std::puts("[unit_test] OK");
    return 0;
}
