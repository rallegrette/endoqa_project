# ENDOQA — Endoscopic Image Quality Checker (C++17 / OpenCV)

## Overview

**EndoQA** is a C++ application that evaluates the objective quality of medical-imaging and endoscopic-style frames.  
It computes reproducible metrics such as **sharpness**, **noise**, **exposure uniformity**, **brightness stability**, and **sensor reliability**, then produces a structured **JSON PASS/FAIL report**.

This project was inspired by the verification and validation processes used in regulated medical-device software, such as those employed by **Karl Storz**, where image consistency, reliability, and documentation are critical.

## Key Features

- **C++17 / OpenCV implementation** for speed and portability  
- **Quantitative metrics:**
  - **Sharpness:** Variance-of-Laplacian measure of edge clarity  
  - **Noise:** Local variance of pixel intensity  
  - **Exposure Uniformity:** Brightness variance across image tiles  
  - **Brightness Trend:** Temporal brightness slope across frames  
  - **Dead-Pixel Check:** Identifies sensor pixels that remain constant across frames  
- **JSON report** summarizing metric results and threshold comparisons  
- **Unit tests** verifying each metric using synthetic images  
- **Static analysis** using `.clang-tidy` for code safety and consistency  
- **CMake build system** producing two executables:
  - `endoqa` – main command-line tool  
  - `endoqa_tests` – unit-test suite  


## Metrics
- **Sharpness** — Variance of Laplacian (focus measure)
- **Noise** — Stddev of (image - GaussianBlur(image))
- **Exposure Uniformity** — Tiled mean CV mapped to 0..1 (1 is best)
- **Dead/Stuck Pixels** (multi-frame) — Low variance & extreme mean
- **Brightness Trend** — Per-frame means + linear slope

## Example Output
{
  "per_frame": {
    "brightness_mean": 49.57,
    "sharpness": 2238.67,
    "noise": 13.88,
    "exposure_uniformity": 0.801
  },
  "dead_pixels_multi_frame": 76502,
  "overall": "FAIL",
  "thresholds": {
    "minSharpness": 50,
    "maxNoise": 15,
    "minExposureUniformity": 0.85
  }
}


### Prereqs
- CMake >= 3.15
- C++17 compiler
- OpenCV (core, imgproc, imgcodecs)

macOS (Homebrew):
```bash
brew install opencv cmake
```

Ubuntu:
```bash
sudo apt-get install -y build-essential cmake libopencv-dev
```

### Configure & Build
```bash
mkdir build && cd build
cmake .. -DENDOQA_BUILD_TESTS=ON
cmake --build . -j
```

### Run
Single frame:
```bash
./endoqa ../samples/frame1.jpg
```

Multiple frames (enables dead pixel check & brightness trend):
```bash
./endoqa ../samples/frame1.jpg ../samples/frame2.jpg ../samples/frame3.jpg --report out.json
cat out.json
```

Exit codes:
- `0` — PASS
- `2` — FAIL (threshold violation)

## Tests
```bash
./endoqa_tests
```

## Quality Hooks
- `.clang-tidy` with modernize/readability/cppcoreguidelines
- Clear **requirements** and **traceability** in `docs/`
