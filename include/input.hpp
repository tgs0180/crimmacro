#pragma once
#include "fd.hpp"
#include <vector>
#include <unistd.h>
#include <linux/uinput.h>

class VirtualInput {
    public:
        VirtualInput();
        ~VirtualInput();
        bool valid() const;
        void tap(int key);
        void hold(int key, float seconds);

    private:
        Fd fd_;
        void emit(int type, int code, int value);
        std::vector<int> keys{KEY_W, KEY_A, KEY_S, KEY_D, KEY_E};
};