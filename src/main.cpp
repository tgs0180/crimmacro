#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <optional>
#include <unistd.h>
#include <linux/input.h>
#include "hotkeys.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "hyprland.hpp"
#include "input.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "eyes.hpp"

using namespace std::chrono_literals; // Enables the 'ms' suffix

enum class State{Paused, Working, Quitting};

struct fract {
    float x{};
    float y{};
};

int main() {
    VirtualInput input;

    if (!input.valid()) std::cout << "input isnt vaid" << "\n";

    std::atomic<State> state{State::Paused};
    std::atomic<bool> pickRequested{false};
    std::atomic<bool> screenshotRequested{false};
    std::thread thread(listenHotkeys, [&state, &input, &pickRequested, &screenshotRequested](int code) {
        if (code == KEY_F1) {state = State::Working; input.shouldStop = false; return true;}
        else if (code == KEY_F2) {state = State::Paused; input.shouldStop = true; return true;}
        else if (code == KEY_F3) {state = State::Quitting; input.shouldStop = true; return false;}
        else if (code == KEY_F5) {pickRequested = true; return true;}
        else if (code == KEY_F6) {screenshotRequested = true; return true;}
        else return true;});


    if (auto win = parser()) {
        std::cout << win->x << "\n" << win->y << "\n";
    }

    else std::cout << "Sober is not opened" << "\n";
    if (!focusSober()) std::cout << "failed to focus sober" << "\n";

    std::optional<fract> autohatchPos;
    bool autohatchClicked{false};
    std::chrono::steady_clock::time_point lastAfk = std::chrono::steady_clock::now() - 6min;
    while (state != State::Quitting) {
        if (state == State::Working) {
            focusSober();
            std::cout << "working..." << "\n";
            if (!autohatchPos.has_value()) {
                std::cout << "Press F5 to save button position" << "\n";
            }
            else {
                auto win = parser();
                if (win) {
                    auto clickposx = autohatchPos->x * win->width + win->x;
                    auto clickposy = autohatchPos->y * win->height + win->y;
                    if (!autohatchClicked) {input.clickAt(clickposx, clickposy); autohatchClicked = true;}
                }
            }

            if (std::chrono::steady_clock::now() - lastAfk >= 5min) {
                input.tap(KEY_P);
                lastAfk = std::chrono::steady_clock::now();
            }
        }

        if (pickRequested.exchange(false)) {
            auto cursor = cursorPos();
            auto win = parser();
            if (win && cursor) {
                float relX = (cursor->x - win->x) / float(win->width);
                float relY = (cursor->y - win->y) / float(win->height);
                std::cout << relX << "\n" << relY << "\n";
                autohatchPos = fract{relX, relY};
            }
        }

        if (screenshotRequested.exchange(false)) {
            cv::Mat img = look();
            if (!img.empty()) {
                cv::imwrite("debug.png", img);
                std::cout << "TOOK SCREENSHOT" << "\n";
            }
        }

        std::this_thread::sleep_for(500ms);
    }

    thread.join();

    return 0;
}