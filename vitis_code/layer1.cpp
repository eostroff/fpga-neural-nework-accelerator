#include "layer1.h"

void layer1_hw(
    int x[INPUT_SIZE],
    int W[OUTPUT_SIZE * INPUT_SIZE],
    int b[OUTPUT_SIZE],
    int y[OUTPUT_SIZE],
    int input_size,
    int output_size
)
{
    #pragma HLS INTERFACE m_axi depth=784   port=x    bundle=gmem0
    #pragma HLS INTERFACE m_axi depth=100352 port=W   bundle=gmem1
    #pragma HLS INTERFACE m_axi depth=128   port=b    bundle=gmem2
    #pragma HLS INTERFACE m_axi depth=128   port=y    bundle=gmem3
    #pragma HLS INTERFACE s_axilite port=input_size   bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=output_size  bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=return       bundle=CTRL

    loop1: for (int i = 0; i < output_size; i++) {
        #pragma HLS loop_tripcount min=128 max=128 avg=128

        int acc = b[i];

        loop2: for (int j = 0; j < input_size; j++) {
            #pragma HLS loop_tripcount min=784 max=784 avg=784
            acc += W[i * input_size + j] * x[j];
        }

        y[i] = (acc > 0) ? acc : 0;
    }
}