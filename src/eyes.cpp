#include "eyes.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include "hyprland.hpp"
#include "opencv2/core/mat.hpp"

cv::Mat look() {
    auto win = parser();
    if (!win) return cv::Mat();
    std::string command = std::string("grim -g ") + std::string(R"(")") + std::to_string(win->x) + "," + std::to_string(win->y) + " " + std::to_string(win->width) + "x" + std::to_string(win->height) + R"(")" + " -t ppm -";
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) {return cv::Mat();}
    
    std::vector<unsigned char> bytes;
    unsigned char buf[65536];
    while (true) {
        size_t n = fread(buf, 1, sizeof buf, pipe);
        if (n > 0) bytes.insert(bytes.end(), buf, buf + n);
        else if (n == 0) break;
        else if (n < 0) {pclose(pipe); return cv::Mat();}
    }

    cv::Mat img = cv::imdecode(bytes, cv::IMREAD_COLOR);
    pclose(pipe);
    return img;
}