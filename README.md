# ENDOQA — Endoscopic Image Quality Checker (C++17 / OpenCV)

A compact, production-quality sample that analyzes image frames for metrics relevant to endoscopy and emits a JSON report with a PASS/FAIL decision.

## Metrics
- **Sharpness** — Variance of Laplacian (focus measure)
- **Noise** — Stddev of (image - GaussianBlur(image))
- **Exposure Uniformity** — Tiled mean CV mapped to 0..1 (1 is best)
- **Dead/Stuck Pixels** (multi-frame) — Low variance & extreme mean
- **Brightness Trend** — Per-frame means + linear slope

## Build

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
