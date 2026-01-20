/*
 * process_image.h
 *
 *  Created on: 17.01.2026.
 *      Author: 23dej
 */

#ifndef PROCESS_IMAGE_H_
#define PROCESS_IMAGE_H_

#include "dct.h"
#include "rl_encoder.h"
#include "image_rgb.h"

void RunImagePipeline(const int8_t* p_gray_buffer, float* p_dct_results, int16_t* p_quant_results,
		                int16_t* p_zigzag_results);
void QuantizeBlockUltra(const float* p_dct_coeffs, int16_t* p_quant_output);
void RunImagePipelineRLE(const int8_t* p_gray_buffer, RLE_block_t* p_rle_results);
void QuantizeAndZigZag_Fused(const float* __restrict dct_in, int16_t* __restrict zz_out);

#endif /* PROCESS_IMAGE_H_ */
