/**
 * @file dct.h
 * @author Dejana Smiljanic
 * @date 02.01.2026.
 * @brief Discrete Cosine Transform (DCT) function declarations.
 *
 * This header declares functions for processing image blocks using
 * the Discrete Cosine Transform (DCT) and for performing Zig-Zag
 * transform of DCT coefficients.
 */

#ifndef DCT_H_
#define DCT_H_

#include <stdio.h>
#include <builtins.h>
#include "common_defs.h"
#include "custom_types.h"
#include "grayscale_converter.h"

/**
 * @brief Performs DCT processing on a single image block.
 *
 * Applies the Discrete Cosine Transform to an 8x8 block of grayscale
 * image data starting at the specified (x, y) position.
 *
 * @param input_p   Pointer to input image data (spatial domain).
 * @param output_p  Pointer to output DCT coefficients (frequency domain).
 * @param start_x   X-coordinate of the top-left block position.
 * @param start_y   Y-coordinate of the top-left block position.
 */
void DctBlockProcess(const int8_t* input_p, float* output_p,
                     uint16_t start_x, uint16_t start_y);

/**
 * @brief Rearranges DCT coefficients using Zig-Zag order.
 *
 * Converts a 2D block of DCT coefficients into a 1D sequence following
 * the Zig-Zag scan pattern, prioritizing low-frequency components.
 *
 * @param input_p   Pointer to input DCT coefficient array.
 * @param output_p  Pointer to output array in Zig-Zag order.
 */
void ZigZagTransform(const int16_t* input_p, int16_t* output_p);

#endif /* DCT_H_ */
