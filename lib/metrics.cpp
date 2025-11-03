#include "lib/metrics.hpp"
#include <opencv2/imgproc.hpp>
#include <numeric>

namespace endoqa {

static double compute_sharpness(const cv::Mat& gray) {
    cv::Mat lap;
    cv::Laplacian(gray, lap, CV_64F);
    cv::Scalar mean, stddev;
    cv::meanStdDev(lap, mean, stddev);
    // Variance of Laplacian is a common focus measure
    return stddev[0]*stddev[0];
}

static double compute_noise(const cv::Mat& gray) {
    // Estimate noise via high-frequency energy: difference between image and its Gaussian blur
    cv::Mat blur, diff;
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 0);
    cv::absdiff(gray, blur, diff);
    cv::Scalar mean, stddev;
    cv::meanStdDev(diff, mean, stddev);
    // Scale factor to convert to approx sigma in pixel intensities
    return stddev[0];
}

static double compute_exposure_uniformity(const cv::Mat& gray) {
    // Split into tiles and compute mean per tile; uniformity = 1 / (1 + CV of tile means)
    const int tiles_x = 4, tiles_y = 4;
    int w = gray.cols, h = gray.rows;
    double eps = 1e-9;
    std::vector<double> means;
    for (int ty = 0; ty < tiles_y; ++ty) {
        for (int tx = 0; tx < tiles_x; ++tx) {
            int x0 = tx * w / tiles_x;
            int x1 = (tx+1) * w / tiles_x;
            int y0 = ty * h / tiles_y;
            int y1 = (ty+1) * h / tiles_y;
            cv::Rect roi(x0, y0, x1 - x0, y1 - y0);
            cv::Scalar m = cv::mean(gray(roi));
            means.push_back(m[0]);
        }
    }
    double mu = std::accumulate(means.begin(), means.end(), 0.0) / means.size();
    double var = 0.0;
    for (double v : means) var += (v - mu)*(v - mu);
    var /= std::max<size_t>(1, means.size());
    double sd = std::sqrt(var);
    double cv = sd / (mu + eps);
    double uniformity = 1.0 / (1.0 + cv); // 1.0 is best
    if (uniformity < 0.0) uniformity = 0.0;
    if (uniformity > 1.0) uniformity = 1.0;
    return uniformity;
}

static double mean_brightness(const cv::Mat& gray) {
    cv::Scalar m = cv::mean(gray);
    return m[0];
}

Metrics compute_single_frame_metrics(const cv::Mat& gray) {
    Metrics m;
    m.sharpness = compute_sharpness(gray);
    m.noise = compute_noise(gray);
    m.exposureUniformity = compute_exposure_uniformity(gray);
    m.deadPixels = 0; // filled in by multi-frame analysis
    m.brightnessMean = mean_brightness(gray);
    return m;
}

int count_stuck_pixels(const std::vector<cv::Mat>& grayFrames, double var_epsilon, int extreme_threshold) {
    if (grayFrames.empty()) return 0;
    int h = grayFrames[0].rows, w = grayFrames[0].cols;
    for (const auto& f : grayFrames) {
        if (f.rows != h || f.cols != w) return 0; // inconsistent sizes
    }
    int stuck = 0;
    // For each pixel, compute mean and stddev across frames
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            double mu = 0.0;
            for (const auto& f : grayFrames) mu += f.at<uint8_t>(y,x);
            mu /= grayFrames.size();
            double var = 0.0;
            for (const auto& f : grayFrames) {
                double d = f.at<uint8_t>(y,x) - mu;
                var += d*d;
            }
            var /= grayFrames.size();
            double sd = std::sqrt(var);
            if (sd < var_epsilon && (mu < extreme_threshold || mu > 255 - extreme_threshold)) {
                stuck++;
            }
        }
    }
    return stuck;
}

std::pair<std::vector<double>, double> brightness_trend(const std::vector<cv::Mat>& grayFrames) {
    std::vector<double> means;
    means.reserve(grayFrames.size());
    for (const auto& f : grayFrames) {
        means.push_back(cv::mean(f)[0]);
    }
    // Linear regression slope
    double n = static_cast<double>(means.size());
    if (n == 0) return {means, 0.0};
    double sx=0, sy=0, sxx=0, sxy=0;
    for (size_t i=0;i<means.size();++i) {
        double x = static_cast<double>(i);
        double y = means[i];
        sx += x; sy += y; sxx += x*x; sxy += x*y;
    }
    double denom = n*sxx - sx*sx;
    double slope = denom == 0 ? 0.0 : (n*sxy - sx*sy) / denom;
    return {means, slope};
}

} // namespace endoqa
