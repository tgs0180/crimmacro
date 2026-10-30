#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <unistd.h>
#include <linux/input.h>
#include "hotkeys.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals; // Enables the 'ms' suffix


int main() {
    std::atomic<State> state{State::Paused};
    std::thread thread(listenHotkeys, [&state](int code) {
        if (code == KEY_F1) state = State::Working;
        else if (code == KEY_F2) state = State::Paused;
        else if (code == KEY_F3) state = State::Quitting;});
    while (state != State::Quitting) {
        if (state == State::Working) {
            std::cout << "working..." << "\n";
        }
        std::this_thread::sleep_for(2000ms);
    }
    thread.join();
    return 0;
}