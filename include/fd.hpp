#pragma once
#include <unistd.h>

// Owns a file descriptor and closes it automatically when it goes out of scope (RAII).
class Fd {
public:
    explicit Fd(int fd) : fd_(fd) {}
    ~Fd() { if (fd_ != -1) close(fd_); }

    // No copying: two Fd objects closing the same number would be a bug
    Fd(const Fd&) = delete;
    Fd& operator=(const Fd&) = delete;

    int get() const { return fd_; }
    bool valid() const { return fd_ != -1; }

private:
    int fd_;
};
