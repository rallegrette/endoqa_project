#include "lib/io.hpp"
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>

namespace endoqa {

cv::Mat load_gray(const std::string& path) {
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        throw std::runtime_error("Failed to load image: " + path);
    }
    return img;
}

std::vector<cv::Mat> load_frames(const std::vector<std::string>& paths) {
    std::vector<cv::Mat> frames;
    frames.reserve(paths.size());
    for (const auto& p : paths) {
        frames.push_back(load_gray(p));
    }
    return frames;
}

} // namespace endoqa
