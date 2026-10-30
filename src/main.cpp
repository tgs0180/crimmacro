#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <iostream>

int main() {
    int fd = open("/dev/input/event1", O_RDONLY);
    if (fd == -1) {
        perror("open");
        std::cerr << "Failed to open the fd file" << "\n";
        return 1;
    }
    input_event ev{};

    while (true) {
        ssize_t n = read(fd, &ev, sizeof ev);
        if (n == -1) {perror("n failed"); return 1;}
        else if (n == 0) {std::cerr << "N failed"<< "\n"; return 1;}
        else if (n != sizeof(ev)) {std::cerr << "N failed"<< "\n"; return 1;}
        
        if (ev.code == KEY_F1) {
            if (ev.type == EV_KEY) {
                if (ev.value == 1) { 
                std::cout << "F1!" << "\n";
                }
            }
        }
    }
    
    
    close(fd);
    return 0;
}