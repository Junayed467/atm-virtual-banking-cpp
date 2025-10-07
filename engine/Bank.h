#pragma once
#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include <stdexcept>
#include "../include/BankAccount.h"
#include "../include/SavingAccount.h"
#include "LockManager.h"
#include "Wal.h"

class Bank {
public:
    struct Rec {
        std::unique_ptr<BankAccount> acc;
        mutable std::mutex m;

        Rec() : acc(nullptr) {}
        explicit Rec(std::unique_ptr<BankAccount> p) : acc(std::move(p)) {}
        Rec(const Rec&)            = delete;
        Rec& operator=(const Rec&) = delete;
    };

private:
    std::vector<std::unique_ptr<Rec>> accounts_;
    Wal& wal_;

public:
    explicit Bank(int nAccounts, double initial, Wal& wal,
                  double savingAprPercent = 2.0)
      : accounts_(), wal_(wal) {
        if (nAccounts <= 0) throw std::invalid_argument("nAccounts must be > 0");
        accounts_.reserve(static_cast<size_t>(nAccounts));

        for (int i = 0; i < nAccounts; ++i) {
            auto ba = (i % 3 == 0)
                ? std::unique_ptr<BankAccount>(std::make_unique<SavingAccount>(
                      initial, savingAprPercent, "S" + std::to_string(i)))
                : std::unique_ptr<BankAccount>(std::make_unique<BankAccount>(
                      initial, "B" + std::to_string(i)));

            accounts_.emplace_back(std::make_unique<Rec>(std::move(ba)));
        }
    }

    int size() const { return static_cast<int>(accounts_.size()); }

    bool deposit(int id, double amt) {
        Rec& r = *accounts_.at(static_cast<size_t>(id));
        std::lock_guard<std::mutex> g(r.m);
        return r.acc->deposit(amt);
    }

    bool withdraw(int id, double amt) {
        Rec& r = *accounts_.at(static_cast<size_t>(id));
        std::lock_guard<std::mutex> g(r.m);
        return r.acc->withdraw(amt);
    }

    bool transfer(long long txId, int from, int to, double amt) {
        if (from == to || amt <= 0.0) return false;

        wal_.appendPrepare(txId, from, to, amt);

        Rec& A = *accounts_.at(static_cast<size_t>(from));
        Rec& B = *accounts_.at(static_cast<size_t>(to));

        std::unique_lock<std::mutex> lkFirst;
        std::unique_lock<std::mutex> lkSecond;
        LockManager::lockPair(from, A.m, to, B.m, lkFirst, lkSecond);

        if (!A.acc->withdraw(amt)) { wal_.appendAbort(txId); return false; }
        if (!B.acc->deposit(amt))  { A.acc->deposit(amt);   wal_.appendAbort(txId); return false; }

        wal_.appendCommit(txId);
        return true;
    }

    bool anyNegative() const {
        for (size_t i = 0; i < accounts_.size(); ++i) {
            Rec& r = *accounts_.at(i);
            std::lock_guard<std::mutex> g(r.m);
            if (r.acc->getBalance() < 0.0) return true;
        }
        return false;
    }

    double totalBalance() const {
        double s = 0.0;
        for (size_t i = 0; i < accounts_.size(); ++i) {
            Rec& r = *accounts_.at(i);
            std::lock_guard<std::mutex> g(r.m);
            s += r.acc->getBalance();
        }
        return s;
    }

    double balanceOf(int id) const {
        Rec& r = *accounts_.at(static_cast<size_t>(id));
        std::lock_guard<std::mutex> g(r.m);
        return r.acc->getBalance();
    }
};
