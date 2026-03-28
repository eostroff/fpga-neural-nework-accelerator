# FPGA Neural Network Accelerator (MNIST on AUP-ZU3)

Project for accelerating neural network MNIST inference on AMD AUP-ZU3.

This project uses a 3-layer neural network (input layer, one hidden layer, and output layer).

Neural network structure (3 layers):

```
Input (784) -> Dense(128) + ReLU -> Dense(10) + Softmax
```

Layer 1 (784 -> 128) is offloaded to programmable logic (PL) through a custom HLS IP, while Layer 2 and softmax run on the processor side (PS).

Team: Ethan Ostroff and Neirit Mahabub

## Repository Contents

```
fpga-neural-nework-accelerator/
├── README.md
├── training/
│   ├── neural_network.ipynb
│   ├── mnist_model.keras
│   ├── w1.npy  b1.npy  w2.npy  b2.npy
│
├── board_files/
│   ├── board_inference.ipynb
│   ├── hw_inference.ipynb
│   ├── t10k-images.idx3-ubyte
│   ├── t10k-labels.idx1-ubyte
│   ├── w1.npy  b1.npy  w2.npy  b2.npy
│
├── vitis_code/
│   ├── layer1.cpp
│   ├── layer1.h
│   ├── layer1_tb.cpp
│   ├── layer1 simulation.png
│   └── layer1.zip
│
└── vivado_project/
        ├── block design layer1.png
        └── nn_accelerator.zip
```

## What Each Folder Does

### training/

- `neural_network.ipynb` trains the Keras model on MNIST.
- Exports:
  - `w1.npy`, `b1.npy` for Layer 1
  - `w2.npy`, `b2.npy` for Layer 2
- Saves the full model as `mnist_model.keras`.

### board_files/

- `board_inference.ipynb` runs full-software inference (NumPy only) on board or host.
- `hw_inference.ipynb` runs mixed HW/SW inference:
  - Layer 1 on FPGA IP (`layer1_hw_0`)
  - Layer 2 + softmax in NumPy on PS
- Includes MNIST IDX test files (`t10k-*`) and copied weight files.

### vitis_code/

HLS kernel and C simulation testbench for Layer 1.

- `layer1.h`: interface constants and top function declaration
- `layer1.cpp`: HLS top `layer1_hw(...)` with:
  - 4 AXI master ports (`x`, `W`, `b`, `y`)
  - AXI-Lite control (`input_size`, `output_size`, `return`)
  - ReLU applied at output
- `layer1_tb.cpp`: C testbench compares HLS function against software reference.
- `layer1.zip`: archived Vitis HLS project/results (includes `csim` logs, `csynth` reports, packaged IP under `impl/ip`).

### vivado_project/

- `nn_accelerator.zip`: full Vivado project archive with block design, synth/impl runs, and generated bitstream.
  - Contains `design_1_wrapper.bit` (bitstream)
  - Contains `design_1.hwh` (hardware handoff metadata)
  - Contains `nn_accelerator.xpr` and associated run reports
- `block design layer1.png`: screenshot of the hardware block design.

## End-to-End Workflow

1. Train/export model weights
   - Open `training/neural_network.ipynb`
   - Train model and export `w1.npy`, `b1.npy`, `w2.npy`, `b2.npy`

2. Validate software baseline
   - Open `board_files/board_inference.ipynb`
   - Run NumPy forward pass, sample timing, and full-test accuracy

3. Build/verify Layer 1 HLS kernel
   - Source files: `vitis_code/layer1.cpp`, `vitis_code/layer1.h`, `vitis_code/layer1_tb.cpp`
   - Use Vitis HLS for C simulation and synthesis
   - Optional: inspect archived outputs in `vitis_code/layer1.zip`

4. Integrate IP in Vivado and generate overlay
   - Open/extract `vivado_project/nn_accelerator.zip`
   - Use the included block design/project to generate/update `.bit` and `.hwh`

5. Run hardware-assisted inference on board
   - Ensure `layer1.bit` and matching `layer1.hwh` are present for PYNQ overlay loading
   - Open `board_files/hw_inference.ipynb`
   - Run HW Layer 1 + SW Layer 2/softmax inference and compare timing/accuracy

## Hardware Inference Notes

- `hw_inference.ipynb` uses `SCALE = 1000` fixed-point style quantization:
  - Inputs scaled to int32
  - `W1` transposed to `(128, 784)` before loading
  - Bias scaled by `SCALE^2` for consistent accumulation units
- Kernel polling is done through AXI-Lite control register (`ap_start`/`ap_done`).
- The notebook prints:
  - 30-sample prediction/timing summary
  - Full test-set accuracy
  - Layer 1-only timing breakdown

## Dependencies

### Training environment (host/laptop)

- Python 3.x
- `tensorflow` / `keras`
- `numpy`
- `matplotlib`

### Board software baseline

- Python 3.x
- `numpy`
- `matplotlib`

### Board hardware inference

- Python 3.x
- `numpy`
- `matplotlib`
- `pynq` (for `Overlay` and `allocate`)
