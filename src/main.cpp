#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "hotkeys.hpp"

auto listenHotKeys([](int code){
    if (code == KEY_F1) {
        std::cout << "F1!" << "\n";
    }
});


int main() {
    listenHotkeys([](int code) {if (code == KEY_F1) {std::cout << "F1!" << "\n";}});
    return 0;
}