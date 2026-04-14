#include <iostream>
#include "layer1_stream.h"

int main() {
    hls::stream<stream_t> in_stream;
    hls::stream<stream_t> out_stream;
    stream_t tmp;

    int x[INPUT_SIZE];
    int W[OUTPUT_SIZE * INPUT_SIZE];
    int b[OUTPUT_SIZE];
    int y_sw[OUTPUT_SIZE];

    // Fill with known values
    for (int j = 0; j < INPUT_SIZE; j++) x[j] = 1;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        b[i] = 1;
        for (int j = 0; j < INPUT_SIZE; j++)
            W[i * INPUT_SIZE + j] = 1;
    }

    // Software reference
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        int acc = b[i];
        for (int j = 0; j < INPUT_SIZE; j++)
            acc += W[i * INPUT_SIZE + j] * x[j];
        y_sw[i] = (acc > 0) ? acc : 0;
    }

    // Stream in x (no last)
    for (int j = 0; j < INPUT_SIZE; j++) {
        tmp.data = x[j];
        tmp.keep = 0xF;
        tmp.strb = 0xF;
        tmp.last = 0;
        in_stream.write(tmp);
    }

    // Stream in W (no last)
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            tmp.data = W[i * INPUT_SIZE + j];
            tmp.keep = 0xF;
            tmp.strb = 0xF;
            tmp.last = 0;
            in_stream.write(tmp);
        }
    }

    // Stream in b — set last ONLY on the final value
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        tmp.data = b[i];
        tmp.keep = 0xF;
        tmp.strb = 0xF;
        tmp.last = (i == OUTPUT_SIZE - 1) ? 1 : 0;
        in_stream.write(tmp);
    }

    // Run kernel
    layer1_stream(in_stream, out_stream);

    // Check output
    int errors = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        out_stream.read(tmp);
        int y_hw = tmp.data.to_int();
        if (y_hw != y_sw[i]) {
            std::cout << "MISMATCH at y[" << i << "]: hw=" << y_hw
                      << " sw=" << y_sw[i] << std::endl;
            errors++;
        }
    }

    if (errors == 0) {
        std::cout << "PASS: all " << OUTPUT_SIZE << " outputs match" << std::endl;
        std::cout << "y[0] expected: " << y_sw[0] << std::endl;
    } else {
        std::cout << "FAIL: " << errors << " mismatches" << std::endl;
    }

    return errors;
}