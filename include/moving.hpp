#pragma once
#include <opencv2/core.hpp>
#include "hyprland.hpp"
#include "input.hpp"
#include "camera.hpp"

bool teleportTo(const cv::Mat& tmpl, VirtualInput& input, parseresult win);

bool goToEmpyreanEgg(VirtualInput& input, Camera& camera);

bool joinGame(VirtualInput& input, Camera& camera);
