#include <thread>
#include <vector>
#include <atomic>
#include <random>
#include <cstdio>
#include <chrono>
#include <cmath>
#include "../engine/Wal.h"
#include "../engine/Bank.h"

static inline long long now_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

static void worker_fn(int tid, Bank* bank, std::atomic<long>* failed,
                      int USERS, long OPS_PER_THREAD) {
    std::mt19937_64 rng(1234u + static_cast<unsigned>(tid));
    std::uniform_int_distribution<int>    uid(0, USERS-1);
    std::uniform_real_distribution<double> money(0.01, 100.0);

    for (long i = 0; i < OPS_PER_THREAD; ++i) {
        int a = uid(rng), b = uid(rng);
        if (a == b) continue;
        // Cap amount by current balance to avoid guaranteed failures
        double balA = bank->balanceOf(a);
        if (balA <= 0.0) continue;
        double amt = money(rng);
        if (amt > balA) amt = balA;

        long long txId = ( (static_cast<long long>(tid) << 32) ^ i ) ^ 0x9E3779B97F4A7C15ULL;
        if (!bank->transfer(txId, a, b, amt))
            failed->fetch_add(1, std::memory_order_relaxed);

        if ((i % 100000) == 0 && i != 0 && bank->anyNegative()) {
            puts("[concurrency] NEGATIVE mid-run");
            return;
        }
    }
}

int main() {
    Wal wal("wal_concurrency.log");

    const int USERS = 1000;
    const int THREADS = std::max(8u, std::thread::hardware_concurrency()*2);
    const long OPS_PER_THREAD = 200000;
    const double INIT = 1000.0;

    Bank bank(USERS, INIT, wal);
    const double initial = bank.totalBalance();

    std::atomic<long> failed(0);

    long long t0 = now_us();
    std::vector<std::thread> threads;
    threads.reserve(static_cast<size_t>(THREADS));
    for (int t = 0; t < THREADS; ++t) {
        threads.push_back(std::thread(worker_fn, t, &bank, &failed, USERS, OPS_PER_THREAD));
    }
    for (size_t i = 0; i < threads.size(); ++i) threads[i].join();
    long long t1 = now_us();

    const bool neg = bank.anyNegative();
    const double final = bank.totalBalance();
    const double secs = (t1 - t0) / 1e6;
    const long total_ops = static_cast<long>(THREADS) * OPS_PER_THREAD;
    const double ops_sec = total_ops / secs;

    printf("[concurrency] USERS=%d THREADS=%d OPS=%ld time=%.2fs ops/sec=%.0f failed=%ld neg=%d\n",
        USERS, THREADS, total_ops, secs, ops_sec, (long)failed.load(), (int)neg);
    printf("[concurrency] total_initial=%.2f total_final=%.2f delta=%.6f\n",
        initial, final, final - initial);

    const double eps = std::max(1e-7 * initial, 1e-6);
    if (std::fabs(static_cast<double>(final - initial)) > eps) {
        puts("[concurrency] FUND CONSERVATION VIOLATION");
        return 3;
    }
    if (neg) return 2;
    return 0;
}
