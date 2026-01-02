# EndoQA  
**Objective Image Quality Analysis for Medical & Endoscopic Imaging**

**Author:** Rose Allegrette  
**Language:** C++17  
**Build System:** CMake  
**Primary Dependency:** OpenCV  
**Testing:** Doctest  
**Platform:** macOS / Linux / Windows  

![Look-SeeEndoscopeCamera](https://github.com/user-attachments/assets/f44d6a54-fc48-4d1b-86ce-a921f56590bc)

---

## Overview

**EndoQA** is a C++ image-quality analysis tool designed to evaluate the reliability and consistency of medical-imaging frames, such as those produced by endoscopic camera systems.

The application computes deterministic, objective image-quality metrics and generates a structured **JSON PASS/FAIL report** suitable for automated verification pipelines.  
The project mirrors real-world medical-device software workflows where **image quality, traceability, and reproducibility** are critical.

This project is particularly relevant to companies like **Karl Storz**, where software plays a key role in validating optical performance, sensor health, and imaging stability.

---

## Motivation

Medical imaging systems must meet strict quality requirements:

- Sharp and well-defined visuals  
- Stable illumination and exposure  
- Low sensor noise  
- Reliable detection of sensor defects  
- Documented, repeatable validation  

EndoQA demonstrates how software can **quantitatively validate imaging performance**, rather than relying on subjective visual inspection.

---

## Features

### Image Quality Metrics
- **Sharpness**
  - Variance of Laplacian to quantify edge clarity
- **Noise Estimation**
  - Local pixel variance to detect sensor noise
- **Exposure Uniformity**
  - Tile-based brightness analysis across the frame
- **Brightness Stability**
  - Temporal brightness trend across frames
- **Dead Pixel Detection**
  - Multi-frame consistency analysis to identify sensor defects

### Engineering Practices
- Deterministic algorithms (reproducible results)
- Modular C++ design
- Automated unit tests for each metric
- Static analysis via `.clang-tidy`
- JSON-based machine-readable output
- CMake-based cross-platform build

---

## Example Output

```json
{
  "per_frame": {
    "brightness_mean": 49.57,
    "sharpness": 2238.67,
    "noise": 13.88,
    "exposure_uniformity": 0.801
  },
  "brightness_series": [49.57, 49.57],
  "brightness_slope": 0.0,
  "dead_pixels_multi_frame": 76502,
  "overall": "FAIL",
  "thresholds": {
    "minBrightness": 10,
    "maxBrightness": 245,
    "minSharpness": 50,
    "maxNoise": 15,
    "minExposureUniformity": 0.85,
    "maxDeadPixels": 0
  }
}
```


Each metric is evaluated against predefined thresholds, resulting in an objective PASS/FAIL decision.


## Project Structure

```
endoqa_project/
├── app/
│   └── main.cpp          # CLI entry point
├── lib/
│   ├── metrics.cpp       # Image-quality algorithms
│   ├── io.cpp            # Image loading and output
│   └── report.cpp        # JSON report generation
├── tests/
│   └── test_metrics.cpp  # Unit tests
├── docs/
│   └── requirements.md   # Design and validation notes
├── .clang-tidy            # Static analysis configuration
└── CMakeLists.txt         # Build configuration
```

## Testing & Validation

Each metric is validated using synthetic and controlled test cases:

- Blurred vs. sharp images validate the sharpness metric
- Noisy vs. clean images validate noise detection
- Uneven lighting validates exposure-uniformity analysis
- Repeated frames validate dead-pixel detection

All tests are executable via the `endoqa_tests` binary and must pass before deployment.

---

## Medical Device Relevance

EndoQA reflects software practices used in regulated medical environments:

- Clear separation of computation and reporting
- Deterministic outputs for auditability
- Automated verification via unit tests
- Metrics aligned with optical and sensor performance
- Design consistent with IEC 62304 principles

This makes the project directly applicable to imaging-system validation workflows at companies like **Karl Storz**.

## Build Instructions

### macOS

```bash
brew install cmake opencv

cd endoqa_project
mkdir build && cd build
cmake .. -DOpenCV_DIR="$(brew --prefix opencv)/lib/cmake/opencv4"
cmake --build . -j
```

### Linux
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libopencv-dev

cd endoqa_project
mkdir build && cd build
cmake ..
cmake --build . -j
```

### Windows (PowerShell)
```bash 
# Install Visual Studio Build Tools and CMake
# Set OpenCV_DIR to your OpenCV installation path

cd endoqa_project
mkdir build
cd build
cmake .. -DOpenCV_DIR="C:\path\to\opencv\build"
cmake --build . --config Release
```

### Usage
```bash
./endoqa frame1.jpg frame2.jpg --report report.json
```
Run unit tests:
```bash
./endoqa_tests
```
### Future Improvements
- Color balance and white-balance drift detection
- Lens vignetting analysis
- Real-time video stream support
- GPU acceleration for embedded systems
- Qt-based visualization dashboard
- CI integration for automated regression testing


## Quality Hooks
- `.clang-tidy` with modernize/readability/cppcoreguidelines
- Clear **requirements** and **traceability** in `docs/`
