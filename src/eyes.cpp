#include "eyes.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string>
#include "hyprland.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry.hpp>

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

Match bestMatch(const cv::Mat &screen, const cv::Mat &tmpl) {
    if (screen.empty() || tmpl.empty() || tmpl.cols > screen.cols || tmpl.rows > screen.rows) return Match{};
    cv::Mat result;
    cv::matchTemplate(screen, tmpl, result, cv::TM_CCOEFF_NORMED);

    double maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(result, nullptr, &maxVal, nullptr, &maxLoc);
    cv::Rect rect = cv::Rect(maxLoc.x, maxLoc.y, tmpl.cols, tmpl.rows);
    return Match{rect, maxVal};
}


std::optional<cv::Point> find(const cv::Mat& screen, const cv::Mat& tmpl, double threshold) {
    Match match = bestMatch(screen, tmpl);
    if (match.score < threshold) return std::nullopt;
    return cv::Point(match.box.x + match.box.width / 2, match.box.y + match.box.height / 2);
}

std::optional<cv::Rect> findButton(const cv::Mat& screen, cv::Scalar lower, cv::Scalar upper) {
    cv::Mat mask;
    cv::inRange(screen, lower, upper, mask);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, {9, 9}));
    cv::imwrite("mask.png", mask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (const auto& x : contours) {
        cv::Rect r = cv::boundingRect(x);
        double area = cv::contourArea(x);
        float aspectRatio = float(r.width) / r.height;
        float fill = area / r.area();
        if (aspectRatio < 3.8 && aspectRatio > 2.2 && fill > 0.75 && area >= 300) return r;
    }
    return std::nullopt;
}