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
#include "moving.hpp"
#include <vector>
#include "opencv2/core/types.hpp"
#include <opencv2/imgproc.hpp>

using namespace std::chrono_literals; // Enables the 'ms' suffix

enum class State{Paused, Working, Quitting};

constexpr double failedTextThreshold = 0.5;

// * only used by the F7/F8 teleport test
struct TeleportTest {
    const char* name;
    cv::Mat tmpl;
};

int main() {
    cv::Mat failedText = cv::imread("templates/failed_text.png", cv::IMREAD_COLOR);

    std::vector<TeleportTest> islands = {
        {"Haunted Island", cv::imread("templates/tp_haunted_island.png", cv::IMREAD_COLOR)},
        {"Hell Island", cv::imread("templates/tp_hell_island.png", cv::IMREAD_COLOR)},
        {"Heaven Island", cv::imread("templates/tp_heaven_island.png", cv::IMREAD_COLOR)},
        {"Heaven's Gate", cv::imread("templates/tp_heavens_gate.png", cv::IMREAD_COLOR)},
    };
    size_t nextIsland = 0;

    VirtualInput input;

    if (!input.valid()) std::cout << "input isnt vaid" << "\n";

    std::atomic<State> state{State::Paused};
    std::atomic<bool> screenshotRequested{false};
    std::atomic<bool> teleportRequested{false};
    std::atomic<bool> wrongTeleportRequested{false};
    std::thread thread(listenHotkeys, [&state, &input, &screenshotRequested, &teleportRequested, &wrongTeleportRequested](int code) {
        if (code == KEY_F1) {state = State::Working; input.shouldStop = false; return true;}
        else if (code == KEY_F2) {state = State::Paused; input.shouldStop = true; return true;}
        else if (code == KEY_F3) {state = State::Quitting; input.shouldStop = true; return false;}
        else if (code == KEY_F6) {
            screenshotRequested = true;
            return true;
        }
        else if (code == KEY_F7) {teleportRequested = true; return true;}
        else if (code == KEY_F8) {wrongTeleportRequested = true; return true;}
        else return true;});


    if (auto win = parser()) {
        std::cout << win->x << "\n" << win->y << "\n";
    }

    else std::cout << "Sober is not opened" << "\n";
    if (!focusSober()) std::cout << "failed to focus sober" << "\n";

    std::chrono::steady_clock::time_point lastAfk = std::chrono::steady_clock::now() - 6min;
    std::chrono::steady_clock::time_point lastClick;
    while (state != State::Quitting) {
        if (state == State::Working) {
            focusSober();
            std::cout << "working..." << "\n";

            if (std::chrono::steady_clock::now() - lastAfk >= 5min) {
                input.tap(KEY_P);
                lastAfk = std::chrono::steady_clock::now();
            }

            // ! the actual working logic
            cv::Mat img = look();
            if (!img.empty()) {
                auto turnOn_ = findButton(img, cv::Scalar(30, 200, 30), cv::Scalar(90, 255, 90));
                auto turnOff_ = findButton(img, cv::Scalar(0,0,200), cv::Scalar(90,90,255));
                auto failedText_ = bestMatch(img, failedText);
                if (failedText_.score > failedTextThreshold) {
                    state = State::Paused;
                    std::cout << "Autohatch failed!" << "\n";
                }
                else if (turnOn_) {
                    auto win = parser();
                    if (win && std::chrono::steady_clock::now() - lastClick >= 2s) {
                        int cx = turnOn_->x + turnOn_->width / 2;
                        int cy = turnOn_->y + turnOn_->height / 2;
                        input.clickAt(cx + win->x, cy + win->y);
                        std::cout << "Turned autohatch on" << "\n";
                        lastClick = std::chrono::steady_clock::now();
                    }
                }
                else if (turnOff_) {
                    // * do nothing
                }
                else std::cout << "Couldnt find anything" << "\n";
            }
        }


        if (screenshotRequested.exchange(false)) {
            cv::Mat img = look();
            if (!img.empty()) {
                cv::imwrite("debug.png", img); // stays clean, so templates can be cropped from it
                std::cout << "TOOK SCREENSHOT" << "\n";

                auto r = findButton(img, cv::Scalar(0,0,200), cv::Scalar(90,90,255));
                auto rr = findButton(img, cv::Scalar(30, 200, 30), cv::Scalar(90, 255, 90));
                auto m = bestMatch(img, failedText);
                std::cout << "failed text score: " << m.score << "\n";
                if (rr) {
                    std::cout << "turn on detected" << "\n";
                    cv::rectangle(img, *rr, cv::Scalar(0, 0, 255), 2);
                }
                else if (r) {
                    std::cout << "turn off detected" << "\n";
                    cv::rectangle(img, *r, cv::Scalar(0,255,0), 2);
                }
                if (m.score > failedTextThreshold) {
                    std::cout << "failed text detected" << "\n";
                    cv::rectangle(img, m.box, cv::Scalar(255, 0, 0), 2);
                }
                cv::imwrite("debug_boxes.png", img);
            }
        }

        // * teleport test: F7 goes to the next island, F8 searches for something that isn't in the popup
        bool realTest = teleportRequested.exchange(false);
        bool wrongTest = wrongTeleportRequested.exchange(false);
        if (realTest || wrongTest) {
            auto win = parser();
            if (state != State::Paused) std::cout << "teleport test only runs while paused" << "\n";
            else if (!win) std::cout << "Sober is not opened" << "\n";
            else {
                const char* name = wrongTest ? "wrong template" : islands[nextIsland].name;
                const cv::Mat& tmpl = wrongTest ? failedText : islands[nextIsland].tmpl;
                if (!wrongTest) nextIsland = (nextIsland + 1) % islands.size();

                std::cout << "teleport test: " << name << "\n";
                focusSober();
                input.shouldStop = false; // F2 leaves this on, and it would make teleportTo quit right away
                bool result = teleportTo(tmpl, input, *win);
                std::cout << "teleportTo returned " << (result ? "true" : "false") << "\n";
            }
        }

        std::this_thread::sleep_for(500ms);
    }

    thread.join();

    return 0;
}
