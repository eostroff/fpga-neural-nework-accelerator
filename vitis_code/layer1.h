#ifndef LAYER1_H
#define LAYER1_H

#define INPUT_SIZE  784
#define OUTPUT_SIZE 128

void layer1_hw(
    int x[INPUT_SIZE],
    int W[OUTPUT_SIZE * INPUT_SIZE],
    int b[OUTPUT_SIZE],
    int y[OUTPUT_SIZE],
    int input_size,
    int output_size
);

#endif