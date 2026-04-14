#ifndef LAYER1_STREAM_H
#define LAYER1_STREAM_H

#include "ap_axi_sdata.h"
#include "hls_stream.h"

#define INPUT_SIZE  784
#define OUTPUT_SIZE 128
#define W_SIZE      (INPUT_SIZE * OUTPUT_SIZE)  // 100352

typedef ap_axis<32,2,5,6> stream_t;

void layer1_stream(
    hls::stream<stream_t> &in_stream,
    hls::stream<stream_t> &out_stream
);

#endif