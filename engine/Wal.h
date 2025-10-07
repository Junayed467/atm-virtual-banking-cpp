#pragma once
#include <fstream>
#include <mutex>
#include <string>

class Wal {
    std::mutex    m_;
    std::ofstream out_;
public:
    explicit Wal(const std::string& path)
      : out_(path.c_str(), std::ios::out | std::ios::app) {}

    void appendPrepare(long long id, int from, int to, double amt) {
        std::lock_guard<std::mutex> g(m_);
        out_ << "PREPARE " << id << " " << from << " " << to << " " << amt << "\n";
        out_.flush();
    }
    void appendCommit(long long id) {
        std::lock_guard<std::mutex> g(m_);
        out_ << "COMMIT " << id << "\n";
        out_.flush();
    }
    void appendAbort(long long id) {
        std::lock_guard<std::mutex> g(m_);
        out_ << "ABORT " << id << "\n";
        out_.flush();
    }
};
