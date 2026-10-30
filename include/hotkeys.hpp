#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <atomic>
#include <functional>
#include <iostream>

void listenHotkeys(std::function<void(int)> onKeyPress);