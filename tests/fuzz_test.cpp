#include <random>
#include <cstdio>
#include <cmath>
#include "../engine/Bank.h"
#include "../engine/Wal.h"

static double cap_by_balance(const Bank& bank, int id, double want) {
    double bal = bank.balanceOf(id);
    if (bal <= 0.0) return 0.0;
    if (want > bal) want = bal;
    return want > 0.0 ? want : 0.0;
}

int main() {
    Wal wal("wal_fuzz.log");
    const int    USERS  = 500;
    const double INIT   = 500.0;
    const int    ROUNDS = 300000;

    Bank bank(USERS, INIT, wal);
    const double before = bank.totalBalance();

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int>    uid(0, USERS-1);
    std::uniform_real_distribution<double> money(0.01, 50.0);

    long attempted = 0, succeeded = 0, failed = 0;

    for (int i = 0; i < ROUNDS; ++i) {
        int a = uid(rng), b = uid(rng);
        if (a == b) continue;
        double amt = cap_by_balance(bank, a, money(rng));
        if (amt <= 0.0) { ++failed; continue; }

        long long txId = ( (static_cast<long long>(i) << 21) ^ a ^ (static_cast<long long>(b) << 33) );
        ++attempted;
        if (bank.transfer(txId, a, b, amt)) ++succeeded; else ++failed;

        if ((i % 50000) == 0 && i != 0 && bank.anyNegative()) {
            fprintf(stderr, "[fuzz] negative balance mid-run\n");
            return 2;
        }
    }

    const double after = bank.totalBalance();
    const bool neg = bank.anyNegative();

    printf("[fuzz] users=%d rounds=%d attempted=%ld ok=%ld fail=%ld total_before=%.6f total_after=%.6f delta=%.9f\n",
        USERS, ROUNDS, attempted, succeeded, failed, before, after, after - before);

    // Transfer-only workload -> totals should match (allow tiny FP noise).
    const double eps = std::max(1e-7 * before, 1e-6);
    if (std::fabs(static_cast<double>(after - before)) > eps) {
        fprintf(stderr, "[fuzz] FUND CONSERVATION VIOLATION: |after-before|=%.9f > eps=%.9f\n",
                std::fabs(static_cast<double>(after - before)), eps);
        return 3;
    }
    if (neg) {
        fprintf(stderr, "[fuzz] negative balance at end\n");
        return 4;
    }
    puts("[fuzz] OK");
    return 0;
}
