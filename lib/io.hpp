#pragma once
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace endoqa {

// Load a single image in grayscale (8-bit). Throws on failure.
cv::Mat load_gray(const std::string& path);

// Load all images from a list of paths (grayscale).
std::vector<cv::Mat> load_frames(const std::vector<std::string>& paths);

// Expand a directory or glob-like pattern is out of scope here; we accept explicit paths.
}
