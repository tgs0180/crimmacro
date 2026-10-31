#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <unistd.h>
#include <linux/input.h>
#include "hotkeys.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "hyprland.hpp"
#include "input.hpp"

using namespace std::chrono_literals; // Enables the 'ms' suffix

enum class State{Paused, Working, Quitting};

int main() {
    VirtualInput input;
    sleep(3);
    input.tap(KEY_A);
    if (!input.valid()) std::cout << "input isnt vaid" << "\n";
    std::atomic<State> state{State::Paused};
    std::thread thread(listenHotkeys, [&state](int code) {
        if (code == KEY_F1) {state = State::Working; return true;}
        else if (code == KEY_F2) {state = State::Paused; return true;}
        else if (code == KEY_F3) {state = State::Quitting; return false;}
        else return true;});
    if (auto win = parser()) {
        std::cout << win->x << "\n" << win->y << "\n";
    }
    else std::cout << "Sober is not opened" << "\n";
    if (!focusSober()) std::cout << "failed to focus sober" << "\n";
    while (state != State::Quitting) {
        if (state == State::Working) {
            std::cout << "working..." << "\n";
        }
        std::this_thread::sleep_for(500ms);
    }
    thread.join();
    return 0;
}