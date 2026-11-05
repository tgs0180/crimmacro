#include "moving.hpp"
#include "camera.hpp"
#include "eyes.hpp"
#include "input.hpp"
#include <chrono>
#include <thread>
#include <opencv2/imgcodecs.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>

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
        if (X.score >= 0.7) {
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
            std::this_thread::sleep_for(250ms);
        }
        if (std::chrono::steady_clock::now() - timeout >= 4s) break;
    }
    return false;
}

// Checks done() every 500ms. Returns false if F2/F3 was pressed or the time ran out.
static bool waitUntil(VirtualInput& input, std::function<bool()> done, std::chrono::seconds limit) {
    auto deadline = std::chrono::steady_clock::now() + limit;
    while (std::chrono::steady_clock::now() < deadline) {
        if (input.shouldStop) return false;
        if (done()) return true;
        std::this_thread::sleep_for(500ms);
    }
    return false;
}

bool joinGame(VirtualInput& input, Camera& camera) {
    if (parser()) {
        std::system("flatpak kill org.vinegarhq.Sober >/dev/null 2>&1");
        if (!waitUntil(input, [] { return !parser(); }, 30s)) return false;
    }

    int open = std::system(R"(xdg-open "roblox://placeId=85920622393590" >/dev/null 2>&1 &)");
    if (open != 0) return false;
    if (!waitUntil(input, [] { return parser().has_value(); }, 60s)) return false;

    auto tpTmpl = cv::imread("templates/tp_remote.png", cv::IMREAD_COLOR);
    if (tpTmpl.empty()) return false;
    bool loaded = waitUntil(input, [&tpTmpl] { return bestMatch(look(), tpTmpl).score > 0.8; }, 120s);
    if (!loaded) return false;

    camera.cameraPos = 0; // a fresh join puts the real camera back at its starting view
    return true;
}



bool goToEmpyreanEgg(VirtualInput& input, Camera& camera) {
    auto win = parser();
    if (!win) {
        std::cout << "No window found" << "\n";
        return false;
    }
    cv::Mat shoptmpl = cv::imread("templates/tp_fantasy_shop.png", cv::IMREAD_COLOR);
    if (!teleportTo(shoptmpl, input, *win)) {
        std::cout << "teleport failed" << "\n";
        return false;
    }
    camera.moveCameraTo(360, input);
    std::this_thread::sleep_for(1s);
    input.hold(KEY_W, 1.75);
    input.hold(KEY_A, 1);
    auto scr = look();
    auto hatch = findButton(scr, cv::Scalar(30, 200, 30), cv::Scalar(90, 255, 90));
    input.moveTo(200, 500);
    if (!hatch) std::cout << "couldnt find the autohatch button" << "\n";
    input.clickAt(hatch->x + hatch->width / 2 + win->x, hatch->y + hatch->height / 2 + win->y);
    return true;
}