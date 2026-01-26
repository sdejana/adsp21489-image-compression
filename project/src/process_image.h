/**
 * @file process_image.h
 * @author Dejana Smiljanic
 * @date 17.01.2026.
 * @brief Image processing pipeline interface.
 *
 * Declares functions used for block-based image processing,
 * including DCT coefficient quantization and run-length encoding.
 */

#ifndef PROCESS_IMAGE_H_
#define PROCESS_IMAGE_H_

#include "dct.h"
#include "rl_encoder.h"
#include "image_rgb.h"
#include "common_defs.h"

/**
 * @brief Quantizes DCT coefficients of one block.
 *
 * Converts floating-point DCT coefficients into
 * fixed-point integer values using the quantization table.
 *
 * @param dct_coeffs_p    Pointer to input DCT coefficients (64 values)
 * @param quant_output_p  Pointer to output quantized coefficients
 */
void QuantizeBlock(const float* dct_coeffs_p, int16_t* quant_output_p);

/**
 * @brief Executes the full image processing flow.
 *
 * Runs grayscale image processing including block splitting,
 * DCT, quantization, zig-zag scan and RLE encoding.
 *
 * @param gray_buffer_p  Pointer to input grayscale image buffer
 * @param rle_results_p  Pointer to output RLE blocks
 */
void RunImageProcessing(const int8_t* gray_buffer_p, RLE_block_t* rle_results_p);

#endif /* PROCESS_IMAGE_H_ */
