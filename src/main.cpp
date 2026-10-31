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

enum class State{Paused, Working, Quitting};

int main() {
    std::atomic<State> state{State::Paused};
    std::thread thread(listenHotkeys, [&state](int code) {
        if (code == KEY_F1) {state = State::Working; return true;}
        else if (code == KEY_F2) {state = State::Paused; return true;}
        else if (code == KEY_F3) {state = State::Quitting; return false;}
        else return true;});
    while (state != State::Quitting) {
        if (state == State::Working) {
            std::cout << "working..." << "\n";
        }
        std::this_thread::sleep_for(500ms);
    }
    thread.join();
    return 0;
}