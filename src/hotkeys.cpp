#include "hotkeys.hpp"

void listenHotkeys(std::function<void(int)> onKeyPress) {
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
        
        onKeyPress(ev.code);
    }
    
    
    close(fd);
}