#include "third_party/doctest.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "lib/metrics.hpp"


using namespace endoqa;

static cv::Mat make_constant(int w, int h, uint8_t val) {
    return cv::Mat(h, w, CV_8UC1, cv::Scalar(val)).clone();
}

static cv::Mat make_edge_image(int w, int h) {
    cv::Mat img(h, w, CV_8UC1, cv::Scalar(0));
    cv::rectangle(img, cv::Rect(w/4, h/4, w/2, h/2), cv::Scalar(255), cv::FILLED);
    return img;
}

DOCTEST_TEST_CASE(test_sharp_vs_blurry) {
    auto sharp = make_edge_image(256, 256);
    cv::Mat blurry;
    cv::GaussianBlur(sharp, blurry, cv::Size(31,31), 0);
    auto ms = compute_single_frame_metrics(sharp);
    auto mb = compute_single_frame_metrics(blurry);
    DOCTEST_CHECK(ms.sharpness > mb.sharpness);
}

DOCTEST_TEST_CASE(test_noise_nonzero) {
    auto base = make_constant(128, 128, 128);
    cv::Mat noisy = base.clone();
    cv::randn(noisy, 128, 10);
    auto m = compute_single_frame_metrics(noisy);
    DOCTEST_CHECK(m.noise > 0.0);
}

DOCTEST_TEST_CASE(test_uniformity_high_on_constant) {
    auto constimg = make_constant(128, 128, 200);
    auto m = compute_single_frame_metrics(constimg);
    DOCTEST_CHECK(m.exposureUniformity > 0.95);
}

DOCTEST_TEST_CASE(test_stuck_pixel_detection) {
    // Create three frames where one pixel is stuck at 255 and others vary
    int w=32, h=32;
    std::vector<cv::Mat> frames;
    for (int i=0;i<3;++i) {
        cv::Mat f(h, w, CV_8UC1);
        cv::randu(f, 0, 255);
        f.at<uint8_t>(10,10) = 255;
        frames.push_back(f);
    }
    int stuck = count_stuck_pixels(frames, /*var_epsilon=*/0.1, /*extreme_threshold=*/10);
    DOCTEST_CHECK(stuck >= 1);
}

DOCTEST_TEST_CASE(test_brightness_trend_slope_positive) {
    int w=64, h=64;
    std::vector<cv::Mat> frames;
    for (int i=0;i<5;++i) {
        frames.push_back(make_constant(w, h, 50 + i*10));
    }
    auto res = brightness_trend(frames);
    // slope should be positive
    DOCTEST_CHECK(res.second > 0.0);
}

DOCTEST_MAIN
