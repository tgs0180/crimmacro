#pragma once
#include "input.hpp"


class Camera {
    public:
        int cameraPos{0};
        void moveCamera(int amount, VirtualInput& input);
        void resetCamera(VirtualInput& input);
        void moveCameraTo(int toWhere, VirtualInput& input);
};