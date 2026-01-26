/**
 * @file dct.c
 * @author Dejana Smiljanic
 * @date 02.01.2026.
 * @brief Implementation of Discrete Cosine Transform (DCT) functions.
 *
 * This source file contains the implementation of an 8x8 Discrete
 * Cosine Transform (DCT) used for image block processing, as well as
 * the Zig-Zag transform used to reorder DCT coefficients for further
 * compression stages.
 */

#include "dct.h"
#include "custom_types.h"
#include "lookup_tables.h"
#include "common_defs.h"

/**
 * @brief Intermediate buffer for DCT computation.
 *
 * Stores the intermediate result of the first matrix multiplication
 * step (T * block). The buffer is aligned to improve SIMD performance
 * on the target DSP architecture.
 */
#pragma align 8
static float s_inter[64];

/**
 * @brief DCT transform matrix.
 *
 * Constant 8x8 transform matrix stored in program memory.
 * Defined in lookup_tables.h.
 */
extern const __pm float T[8][8];

/**
 * @brief Performs DCT on a single 8x8 image block.
 *
 * This function extracts an 8x8 block from the input image, converts
 * it to floating-point format, and applies a 2D DCT using separable
 * matrix multiplication:
 *
 *   output = T * block * T^T
 *
 * SIMD and loop count pragmas are used to optimize execution on
 * DSP hardware.
 *
 * @param input_p   Pointer to the input grayscale image buffer.
 * @param output_p  Pointer to the output DCT coefficient buffer.
 * @param start_x   X-coordinate of the top-left corner of the block.
 * @param start_y   Y-coordinate of the top-left corner of the block.
 */
void DctBlockProcess(const int8_t* input_p, float* output_p,
		             uint16_t start_x, uint16_t start_y)
{
    int r, c;
    float block[64];

    /* Load 8x8 image block and convert to float */
    for (r = 0; r < 8; r++)
    {
        const int8_t* src = &input_p[(start_y + r) * IMG_WIDTH + start_x];
        float* dst = &block[r * 8];
        dst[0]=(float)src[0]; dst[1]=(float)src[1];
        dst[2]=(float)src[2]; dst[3]=(float)src[3];
        dst[4]=(float)src[4]; dst[5]=(float)src[5];
        dst[6]=(float)src[6]; dst[7]=(float)src[7];
    }

    /* First multiplication: T * block */
    for (r = 0; r < 8; r++)
    {
		#pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (c = 0; c < 8; c++)
        {
            float sum;
            const float* row_T = &T[r][0];
            const float* col_B = &block[c];

            sum  = row_T[0] * col_B[0];
            sum += row_T[1] * col_B[8];
            sum += row_T[2] * col_B[16];
            sum += row_T[3] * col_B[24];
            sum += row_T[4] * col_B[32];
            sum += row_T[5] * col_B[40];
            sum += row_T[6] * col_B[48];
            sum += row_T[7] * col_B[56];

            s_inter[r * 8 + c] = sum;
        }
    }

    /* Second multiplication: intermediate * T^T */
    for (r = 0; r < 8; r++)
    {
		#pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (c = 0; c < 8; c++)
        {
            float sum;
            const float* row_I = &s_inter[r * 8];
            const float* row_T = &T[c][0];

            sum  = row_I[0] * row_T[0];
            sum += row_I[1] * row_T[1];
            sum += row_I[2] * row_T[2];
            sum += row_I[3] * row_T[3];
            sum += row_I[4] * row_T[4];
            sum += row_I[5] * row_T[5];
            sum += row_I[6] * row_T[6];
            sum += row_I[7] * row_T[7];

            output_p[r * 8 + c] = sum;
        }
    }
}

/**
 * @brief Performs Zig-Zag reordering of DCT coefficients.
 *
 * Reorders a 1D array of 64 DCT coefficients according to the Zig-Zag
 * scan pattern, grouping low-frequency coefficients first.
 *
 * @param input_p   Pointer to input DCT coefficients.
 * @param output_p  Pointer to output array in Zig-Zag order.
 */
void ZigZagTransform(const int16_t* input_p, int16_t* output_p)
{
	#pragma loop_count(64, 64, 64)
    for (int i = 0; i < 64; i++)
    {
        output_p[i] = input_p[zigzag_index[i]];
    }
}


