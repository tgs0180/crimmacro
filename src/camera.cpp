#include "camera.hpp"
#include "input.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void Camera::moveCamera(int amount, VirtualInput &input) {
    if (amount > 0) {
        int x = amount;
        while (x != 0) {
            input.tap(KEY_RIGHT);
            x -= 1;
            std::this_thread::sleep_for(30ms);
        }
    }
    else if (amount < 0) {
        int x = amount * -1;
        while (x != 0) {
            input.tap(KEY_LEFT);
            x -= 1;
            std::this_thread::sleep_for(30ms);
        }
    }
    cameraPos += amount;
}



void Camera::moveCameraTo(int toWhere, VirtualInput &input){
    int amount = toWhere - cameraPos;
    moveCamera(amount, input);
}

void Camera::resetCamera(VirtualInput &input) {
    moveCamera(0, input);
}