#pragma once
#include "fd.hpp"
#include <vector>
#include <unistd.h>
#include <linux/uinput.h>
#include <atomic>

class VirtualInput {
    public:
        VirtualInput();
        ~VirtualInput();
        bool valid() const;
        void tap(int key);
        void hold(int key, float seconds);
        void moveTo(int x, int y);
        void click();
        void clickAt(int x, int y);
        std::atomic<bool> shouldStop{false};
        void scroll(int amount);
        void drag(int dx, int dy);

    private:
        Fd fd_;
        Fd mouseFd_; // second virtual device, sends relative mouse movement
        void emit(int type, int code, int value);
        void emitMouse(int type, int code, int value);
        std::vector<int> keys{KEY_W, KEY_A, KEY_S, KEY_D, KEY_E, BTN_LEFT, BTN_RIGHT, KEY_P, KEY_LEFT, KEY_RIGHT};
};