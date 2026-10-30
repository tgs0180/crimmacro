#pragma once
#include <linux/input.h>

#include <functional>

enum class State{Paused, Working, Quitting};

void listenHotkeys(std::function<void(int)> onKeyPress);