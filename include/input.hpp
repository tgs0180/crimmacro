#pragma once
#include "fd.hpp"


class VirtualInput {
    public:
        VirtualInput();
        ~VirtualInput();
        bool valid() const;

    private:
        Fd fd_;
};