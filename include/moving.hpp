#pragma once
#include <opencv2/core.hpp>
#include "hyprland.hpp"
#include "input.hpp"

bool teleportTo(const cv::Mat& tmpl, VirtualInput& input, parseresult win);

bool runRoute();