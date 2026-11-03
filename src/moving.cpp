#include "moving.hpp"
#include "eyes.hpp"
#include <chrono>
#include <thread>
#include <opencv2/imgcodecs.hpp>

using namespace std::chrono_literals;

bool teleportTo(const cv::Mat& tmpl, VirtualInput& input, parseresult win) {
    Match X;
    auto scr = look();
    cv::Mat X_button = cv::imread("templates/tp_close.png", cv::IMREAD_COLOR);
    Match tp = bestMatch(scr, cv::imread("templates/tp_remote.png", cv::IMREAD_COLOR));
    if (tp.score > 0.7) {
        input.clickAt(tp.box.x + tp.box.width / 2 + win.x, tp.box.y + tp.box.height / 2 + win.y);
    }
    else return false;
    
    std::chrono::steady_clock::time_point timeout = std::chrono::steady_clock::now();

    while (true) {
        if (input.shouldStop || std::chrono::steady_clock::now() - timeout >= 2s) return false;
        scr = look();
        X = bestMatch(scr, X_button);
        if (X.score >= 0.70) {
            break;
        }
    }

    input.moveTo(X.box.x + X.box.width / 2 - 16 + win.x, win.height / 2 + win.y);
    input.scroll(16);
    std::this_thread::sleep_for(100ms);
    while (true) {
        if (input.shouldStop) return false;
        scr = look();
        Match m = bestMatch(scr, tmpl);
        if (m.score > 0.94) {
            input.clickAt(m.box.x + m.box.width / 2 + win.x, m.box.y + m.box.height / 2 + win.y);
            std::this_thread::sleep_for(500ms);
            input.clickAt(X.box.x + X.box.width / 2 + win.x, X.box.y + X.box.height / 2 + win.y);
            return true;
        }
        else {
            input.scroll(-3);
            std::this_thread::sleep_for(100ms);
        }
        if (std::chrono::steady_clock::now() - timeout >= 4s) break;
    }
    return false;
}
