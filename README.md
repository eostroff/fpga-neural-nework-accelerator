# Hardware-Accelerated Neural Network — Software (PS)

MNIST digit recognition neural network running on the software side (PS) of the AMD AUP-ZU3 board. The goal is to write and time a software baseline, then slowly move the heavy matrix multiplication layers to the FPGA (PL) and measure the speedup.

**Team:** Ethan Ostroff & Neirit Mahabub

---

## Project Overview

The network classifies handwritten digits (0–9) from the MNIST dataset using a two-layer fully connected architecture:

```
Input (784) → Dense 128 + ReLU → Dense 10 + Softmax → Predicted Digit
```

Layer 1 (`784 → 128`) is the main goal for FPGA hardware acceleration since it has the most multiply/accumulate ops. Layer 2 (`128 → 10`) and softmax could remain on the processor if necessary.

---

## Repo Structure

```
neural-network-software/
├── training/           # Training notebook (run on laptop)
│   └── neural_network.ipynb
│       - Trains the model using Keras/TensorFlow
│       - Exports weights as w1.npy, b1.npy, w2.npy, b2.npy
│
└── board_files/        # Inference notebook (run on AUP-ZU3 board)
    └── board_inference.ipynb
        - NumPy forward pass (no PyTorch/Keras needed)
        - Loads .npy weight files
        - Times Layer 1 and Layer 2
```

---

## Dependencies

| Environment               | Libraries                                    |
| ------------------------- | -------------------------------------------- |
| Laptop (training)         | `tensorflow`, `keras`, `numpy`, `matplotlib` |
| AUP-ZU3 board (inference) | `numpy`, `matplotlib`                        |
