#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <opencv2/core.hpp>
#include "lib/io.hpp"
#include "lib/metrics.hpp"
#include "lib/report.hpp"
using namespace endoqa;

static void usage() {
    std::cout << "Usage: endoqa [--report out.json] img1.jpg [img2.jpg ...]\n";
    std::cout << "Provide one or more image paths (grayscale is fine; color will be converted by OpenCV).\n";
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) { usage(); return 1; }
        std::vector<std::string> paths;
        std::string outReport;
        for (int i=1; i<argc; ++i) {
            std::string a = argv[i];
            if (a == "--report") {
                if (i+1 >= argc) { std::cerr << "--report needs a path\n"; return 1; }
                outReport = argv[++i];
            } else {
                paths.push_back(a);
            }
        }
        if (paths.empty()) { usage(); return 1; }
        auto frames = load_frames(paths);
        // Metrics on first frame
        Metrics m = compute_single_frame_metrics(frames[0]);
        // Multi-frame analysis if >1
        int dead = 0;
        std::vector<double> brightnessSeries;
        double slope = 0.0;
        if (frames.size() > 1) {
            dead = count_stuck_pixels(frames);
            auto trend = brightness_trend(frames);
            brightnessSeries = std::move(trend.first);
            slope = trend.second;
        } else {
            brightnessSeries = { m.brightnessMean };
        }
        Thresholds th; // defaults; could be loaded from a JSON config
        Report r = build_report(m, dead, brightnessSeries, slope, th);
        auto json = report_to_json(r);
        std::cout << json << "\n";
        if (!outReport.empty()) {
            std::ofstream ofs(outReport);
            ofs << json;
            ofs.close();
        }
        return r.pass ? 0 : 2;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

/*
Overview:
- Reads one or more images.
- Computes sharpness (variance of Laplacian), noise (difference vs. blur), exposure uniformity (tile CV), brightness.
- If multiple frames are given, detects stuck pixels and computes brightness trend + slope.
- Emits a JSON report and returns non-zero exit code if thresholds fail.

Time Complexity:
Let N = number of pixels per image, F = number of frames.
- Sharpness: O(N)
- Noise: O(N)
- Exposure uniformity: O(N)
- Dead pixel scan across frames: O(N * F)
- Brightness trend: O(N * F)
Overall: O(N * F). For typical 1080p and small F, runs in milliseconds on a laptop.
*/
