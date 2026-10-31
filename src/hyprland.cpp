#include "hyprland.hpp"
#include "fd.hpp"
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h> 
#include <cstdlib>
#include <unistd.h>

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
