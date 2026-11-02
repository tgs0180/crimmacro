#pragma once
#include <opencv2/core.hpp>
#include <optional>

struct Match {
    cv::Rect box;
    double score{};
};

cv::Mat look();

Match bestMatch(const cv::Mat& screen, const cv::Mat& tmpl);
std::optional<cv::Rect> findButton(const cv::Mat& screen, cv::Scalar lower, cv::Scalar upper);
std::optional<cv::Point> find(const cv::Mat& screen, const cv::Mat& tmpl, double threshold);