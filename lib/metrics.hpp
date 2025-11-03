#pragma once
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace endoqa {

struct Metrics {
    double sharpness;           // Higher = sharper
    double noise;               // Estimated sigma (0..~30 typical 8-bit)
    double exposureUniformity;  // 0..1 (1 is perfectly uniform)
    int deadPixels;             // Count of likely stuck pixels across frames
    double brightnessMean;      // Mean brightness of current image (0..255)
};

// Compute metrics on a single frame (grayscale 8-bit).
Metrics compute_single_frame_metrics(const cv::Mat& gray);

// Compute dead/stuck pixels across frames.
// A pixel is "stuck" if its stddev across frames < var_epsilon and mean is very low or very high.
int count_stuck_pixels(const std::vector<cv::Mat>& grayFrames, double var_epsilon = 1.0, int extreme_threshold = 10);

// Compute brightness trend (per-frame means) and return them; also compute slope of linear fit (pixels per frame).
std::pair<std::vector<double>, double> brightness_trend(const std::vector<cv::Mat>& grayFrames);

} // namespace endoqa
