#include "ap_axi_sdata.h"
#include "hls_stream.h"

#define INPUT_SIZE  784
#define OUTPUT_SIZE 128

typedef ap_axis<32,2,5,6> stream_t;

void layer1_stream(
    hls::stream<stream_t> &in_stream,
    hls::stream<stream_t> &out_stream
)
{
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE s_axilite port=return

    stream_t tmp;
    int x_local[INPUT_SIZE];
    int y_local[OUTPUT_SIZE];

    // Read x
    for (int j = 0; j < INPUT_SIZE; j++) {
        in_stream.read(tmp);
        x_local[j] = tmp.data.to_int();
    }

    // Read W row by row and accumulate
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        y_local[i] = 0;
        for (int j = 0; j < INPUT_SIZE; j++) {
            in_stream.read(tmp);
            y_local[i] += tmp.data.to_int() * x_local[j];
        }
    }

    // Read b, apply bias + ReLU
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        in_stream.read(tmp);
        y_local[i] += tmp.data.to_int();
        y_local[i] = (y_local[i] > 0) ? y_local[i] : 0;
    }

    // Stream out y
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        tmp.data = y_local[i];
        tmp.keep = 0xF;
        tmp.strb = 0xF;
        tmp.last = (i == OUTPUT_SIZE - 1) ? 1 : 0;
        out_stream.write(tmp);
    }
}