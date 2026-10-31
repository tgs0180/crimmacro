#pragma once
#include "fd.hpp"


class VirtualInput {
    public:
        VirtualInput();
        ~VirtualInput();
        bool valid() const;
        void tap(int key);

    private:
        Fd fd_;
        void emit(int type, int code, int value);
};