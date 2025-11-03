# Requirements (ENDOQA)

**REQ-001 (Sharpness):** The system shall compute a sharpness score per frame using a focus measure based on the variance of the Laplacian.

**REQ-002 (Noise):** The system shall estimate per-frame noise level by comparing the image to a Gaussian-blurred version and computing the standard deviation of the high-frequency residual.

**REQ-003 (Exposure Uniformity):** The system shall estimate uniformity by tiling the image and computing the coefficient of variation (CV) of tile means, mapping to a 0..1 score.

**REQ-004 (Dead/ Stuck Pixels):** The system shall detect stuck pixels across a sequence by finding pixels with very low variance across frames and extreme mean intensity.

**REQ-005 (Brightness Trend):** The system shall compute mean brightness per frame and the slope of a linear fit across frames.

**REQ-006 (JSON Report):** The system shall emit a JSON report with metric values, thresholds used, and an overall PASS/FAIL decision.

**REQ-007 (Thresholding):** The system shall decide PASS/FAIL using configurable thresholds with reasonable defaults.

**REQ-008 (CLI):** The system shall provide a CLI that accepts one or more image paths and an optional `--report` output file path.

**REQ-009 (Testability):** The project shall include unit tests for key metrics using synthetic images.
