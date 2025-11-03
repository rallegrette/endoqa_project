#include "lib/report.hpp"
#include <sstream>
#include <algorithm>

namespace endoqa {
using mini_json::value;
using mini_json::object;
using mini_json::array;

Report build_report(const Metrics& m, int deadPixels, const std::vector<double>& brightnessSeries, double slope, const Thresholds& th) {
    Report r;
    r.perFrame = m;
    r.deadPixels = deadPixels;
    r.brightnessSeries = brightnessSeries;
    r.brightnessSlope = slope;
    r.used = th;
    bool pass = true;
    pass = pass && (m.sharpness >= th.minSharpness);
    pass = pass && (m.noise <= th.maxNoise);
    pass = pass && (m.exposureUniformity >= th.minExposureUniformity);
    pass = pass && (deadPixels <= th.maxDeadPixels);
    pass = pass && (m.brightnessMean >= th.minBrightness && m.brightnessMean <= th.maxBrightness);
    r.pass = pass;
    return r;
}

static value thresholds_to_json(const Thresholds& t) {
    object o;
    o["minSharpness"] = value(t.minSharpness);
    o["maxNoise"] = value(t.maxNoise);
    o["minExposureUniformity"] = value(t.minExposureUniformity);
    o["maxDeadPixels"] = value(double(t.maxDeadPixels));
    o["minBrightness"] = value(t.minBrightness);
    o["maxBrightness"] = value(t.maxBrightness);
    return value(o);
}

std::string report_to_json(const Report& r) {
    object o;
    object pf;
    pf["sharpness"] = value(r.perFrame.sharpness);
    pf["noise"] = value(r.perFrame.noise);
    pf["exposure_uniformity"] = value(r.perFrame.exposureUniformity);
    pf["dead_pixels_single_frame"] = value(double(r.perFrame.deadPixels));
    pf["brightness_mean"] = value(r.perFrame.brightnessMean);
    o["per_frame"] = value(pf);
    o["dead_pixels_multi_frame"] = value(double(r.deadPixels));
    array series;
    for (double v : r.brightnessSeries) series.push_back(value(v));
    o["brightness_series"] = value(series);
    o["brightness_slope"] = value(r.brightnessSlope);
    o["thresholds"] = thresholds_to_json(r.used);
    o["overall"] = value(r.pass ? "PASS" : "FAIL");
    value v(o);
    return v.dump(2);
}

} // namespace endoqa
