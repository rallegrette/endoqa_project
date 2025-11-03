#pragma once
#include <string>
#include <vector>
#include "third_party/json.hpp"
#include "lib/metrics.hpp"

namespace endoqa {

struct Thresholds {
    double minSharpness = 50.0;         // example thresholds - tune as needed
    double maxNoise = 15.0;
    double minExposureUniformity = 0.85; // 0..1
    int maxDeadPixels = 0;
    double minBrightness = 10.0;
    double maxBrightness = 245.0;
};

struct Report {
    Metrics perFrame;
    int deadPixels;
    std::vector<double> brightnessSeries;
    double brightnessSlope;
    bool pass;
    Thresholds used;
};

// Build a report given metrics and thresholds.
Report build_report(const Metrics& m, int deadPixels, const std::vector<double>& brightnessSeries, double slope, const Thresholds& th);

// Convert report to JSON string.
std::string report_to_json(const Report& r);

} // namespace endoqa
