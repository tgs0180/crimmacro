#include "camera.hpp"
#include "input.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void Camera::moveCamera(int amount, VirtualInput &input) {
    input.drag(amount, 0);
    cameraPos += amount;
}



void Camera::moveCameraTo(int toWhere, VirtualInput &input){
    int amount = toWhere - cameraPos;
    moveCamera(amount, input);
}

void Camera::resetCamera(VirtualInput &input) {
    moveCameraTo(0, input);
}
