#pragma once
#include <linux/input.h>

#include <functional>



void listenHotkeys(std::function<bool(int)> onKeyPress);