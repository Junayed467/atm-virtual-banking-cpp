#include <cstdio>
#include <vector>
#include <random>
#include <chrono>
#include "../engine/Wal.h"
#include "../engine/Bank.h"

static inline long long now_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

int main() {
    Wal wal("wal_scale.log");
    const int USERS = 10000;
    const long OPS = 1000000;
    const double INIT = 1000.0;

    Bank bank(USERS, INIT, wal);

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> uid(0, USERS-1);
    std::uniform_real_distribution<double> money(0.01, 100.0);

    long long t0 = now_us();
    long failed = 0;
    for (long i=0;i<OPS;++i) {
        int from = uid(rng), to = uid(rng);
        if (from == to) continue;
        double amt = money(rng);
        long long txId = (static_cast<long long>(i) << 1) ^ 0xBADC0FFEE;
        if (!bank.transfer(txId, from, to, amt)) ++failed;
    }
    long long t1 = now_us();

    bool neg = bank.anyNegative();
    double secs = (t1 - t0)/1e6;
    std::printf("[scale] USERS=%d OPS=%ld time=%.2fs ops/sec=%.0f failed=%ld neg=%d total=%.2f\n",
        USERS, OPS, secs, OPS/secs, failed, (int)neg, bank.totalBalance());
    return neg ? 2 : 0;
}
