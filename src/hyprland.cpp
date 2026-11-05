#include "hyprland.hpp"
#include "fd.hpp"
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h> 
#include <cstdlib>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <utility>

// Window class of the Roblox client (Quadra), from its quadrahq-roblox-window.desktop file
static const std::string gameClass = "com.roblox.RobloxPlayer";

std::string hyprcommand(std::string command) {
    const char* runtime = std::getenv("XDG_RUNTIME_DIR");
    const char* sign = std::getenv("HYPRLAND_INSTANCE_SIGNATURE");
    if (sign == nullptr || runtime == nullptr) return "";
    std::string path = std::string(runtime) + "/hypr/" + sign + "/.socket.sock";
    Fd sock(socket(AF_UNIX, SOCK_STREAM, 0));
    if (!sock.valid()) return "";
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    if (path.size() >= sizeof(addr.sun_path)) return "";
    std::memcpy(addr.sun_path, path.c_str(), path.size() + 1);
    int con = connect(sock.get(), reinterpret_cast<sockaddr*>(&addr), sizeof addr);
    if (con == -1) {perror("connect");return "";}

    ssize_t res = write(sock.get(), command.c_str(), command.size());
    if (res == -1) return "";

    char buf[4096];
    std::string result;

    while (true) {
        auto n = read(sock.get(), buf, sizeof buf);
        if (n > 0 ) result.append(buf, n);
        else if (n == 0) break;
        else if (n == -1) return "";
    }
    return result;
}

std::optional<parseresult> parser() {
    parseresult x;
    std::string command = hyprcommand("j/clients");
    if (command == "") return std::nullopt;
    auto j = nlohmann::json::parse(command);
    for (auto& w : j) {
        if (w["class"] == gameClass) {
            x.x = w["at"][0];
            x.y = w["at"][1];
            x.width = w["size"][0];
            x.height = w["size"][1];
            return x;
        }
    }
    return std::nullopt;
}

bool focusSober() {
    std::string reply = hyprcommand(R"(dispatch hl.dsp.focus({ window = "class:)" + gameClass + R"(" }))");
    if (reply == "ok") return true;
    else return false;
}

bool closeGame() {
    std::string reply = hyprcommand(R"(dispatch hl.dsp.window.close({ window = "class:)" + gameClass + R"(" }))");
    return reply == "ok";
}

std::optional<cursorParse> cursorPos() {
    cursorParse cursor;
    auto com = hyprcommand("j/cursorpos");
    if (com == "") return std::nullopt;
    auto j = nlohmann::json::parse(com);
    cursor.x = j["x"];
    cursor.y = j["y"];
    return cursor;
}