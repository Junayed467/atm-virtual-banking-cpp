#pragma once
#include <mutex>

struct LockManager {
    static void lockPair(int id1, std::mutex& m1,
                         int id2, std::mutex& m2,
                         std::unique_lock<std::mutex>& outFirst,
                         std::unique_lock<std::mutex>& outSecond) {
        if (id1 < id2) {
            outFirst  = std::unique_lock<std::mutex>(m1);
            outSecond = std::unique_lock<std::mutex>(m2);
        } else if (id2 < id1) {
            outFirst  = std::unique_lock<std::mutex>(m2);
            outSecond = std::unique_lock<std::mutex>(m1);
        } else {
            outFirst  = std::unique_lock<std::mutex>(m1);
            // outSecond stays default-constructed (unlocked)
        }
    }
};
