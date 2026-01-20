#ifndef DCT_H_
#define DCT_H_

#include <stdio.h>
#include "custom_types.h"
#include "grayscale_converter.h"
#include "common_defs.h"

//void run_image_dct(const int8_t* p_gray_buffer, float* p_dct_results);


void process_block_2d_dct_ultra(const int8_t* p_input,
                                float* p_output,
                                uint16_t start_x,
                                uint16_t start_y);
//void print_dct_block(double* p_block);
//void ispisi(float* g_dct_coeffs, uint16_t block_y, uint16_t block_x);
//void ispisi_quant(int16_t* g_dct_coeffs, uint16_t block_y, uint16_t block_x);
void ZigZagScan(const int16_t *input, int16_t *output);
void ispisi_zigzag(int16_t* g_zz, uint16_t block_y, uint16_t block_x);
#endif /* DCT_H_ */
