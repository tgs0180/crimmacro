#pragma once
#include <string>
#include <optional>

std::string hyprcommand(std::string command);

struct parseresult {
    int x{};
    int y{};
    int width{};
    int height{};
};

std::optional<parseresult> parser();

bool focusSober();

bool closeGame();

struct cursorParse {
    int x{};
    int y{};
};

std::optional<cursorParse> cursorPos();