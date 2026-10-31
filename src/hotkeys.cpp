#include "hotkeys.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <atomic>
#include <linux/input.h>

void listenHotkeys(std::function<bool(int)> onKeyPress) {
    int fd = open("/dev/input/event1", O_RDONLY);
    if (fd == -1) {
        perror("open");
        std::cerr << "Failed to open the fd file" << "\n";
        return;
    }
    input_event ev{};

    while (true) {
        ssize_t n = read(fd, &ev, sizeof ev);
        if (n == -1) {perror("n failed"); return;}
        else if (n == 0) {std::cerr << "N failed"<< "\n"; return;}
        else if (n != sizeof(ev)) {std::cerr << "N failed"<< "\n"; return;}
        
        if (ev.value == 1&&ev.type == EV_KEY) {
            if (!onKeyPress(ev.code)) break;
        }

        
    }
    
    
    close(fd);
}