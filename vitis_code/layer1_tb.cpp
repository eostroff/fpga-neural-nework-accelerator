#include <iostream>
#include "layer1.h"

int main() {
    int x[INPUT_SIZE];
    int W[OUTPUT_SIZE * INPUT_SIZE];
    int b[OUTPUT_SIZE];
    int y_hw[OUTPUT_SIZE];
    int y_sw[OUTPUT_SIZE];

    // Fill with simple known values
    for (int j = 0; j < INPUT_SIZE; j++)
        x[j] = 1;

    for (int i = 0; i < OUTPUT_SIZE; i++) {
        b[i] = 1;
        for (int j = 0; j < INPUT_SIZE; j++)
            W[i * INPUT_SIZE + j] = 1;
    }

    // Software reference: same logic as the kernel
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        int acc = b[i];
        for (int j = 0; j < INPUT_SIZE; j++)
            acc += W[i * INPUT_SIZE + j] * x[j];
        y_sw[i] = (acc > 0) ? acc : 0;
    }

    // Hardware function under test
    layer1_hw(x, W, b, y_hw, INPUT_SIZE, OUTPUT_SIZE);

    // Compare
    int errors = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        if (y_hw[i] != y_sw[i]) {
            std::cout << "MISMATCH at y[" << i << "]: hw=" << y_hw[i]
                      << " sw=" << y_sw[i] << std::endl;
            errors++;
        }
    }

    if (errors == 0) {
        std::cout << "PASS: all " << OUTPUT_SIZE << " outputs match" << std::endl;
        std::cout << "y[0] = " << y_hw[0] << " (expected " << y_sw[0] << ")" << std::endl;
    } else {
        std::cout << "FAIL: " << errors << " mismatches" << std::endl;
    }

    return errors;
}