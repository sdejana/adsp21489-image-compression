/**
 * @file process_image.c
 * @author Dejana Smiljanic
 * @date 17.01.2026.
 * @brief Image processing pipeline implementation.
 *
 * Implements block-based image processing including
 * DCT, quantization, zig-zag scan and RLE encoding.
 */


#include "process_image.h"
#include "LED.h"


/**
 * @note Cycle counting
 *
 * To enable cycle counting, uncomment the lines starting with `//`
 * that use START_CYCLE_COUNT, STOP_CYCLE_COUNT and PRINT_CYCLES macros.
 */

// #include <cycle_count.h>
// cycle_t start_count;
// cycle_t final_count;

/**
 * @brief Temporary buffer for DCT coefficients.
 *
 * Stores DCT results for a single 8x8 block.
 */
#pragma align 2
static section("seg_dmda") float dct_temp[64];

/**
 * @brief Temporary buffer for quantized coefficients.
 *
 * Holds quantized DCT coefficients before zig-zag scanning.
 */
#pragma align 2
static section("seg_dmda") int16_t quant_temp[64];

/**
 * @brief Temporary buffer for zig-zag ordered coefficients.
 *
 * Used as input for run-length encoding.
 */
#pragma align 2
static section("seg_dmda") int16_t zigzag_temp[64];

/**
 * @brief Quantizes DCT coefficients of a single block.
 *
 * Applies inverse quantization values and rounds the result
 * to the nearest integer.
 *
 * @param dct_coeffs_p   Pointer to input DCT coefficients
 * @param quant_output_p Pointer to output quantized coefficients
 */
void QuantizeBlock(const float* dct_coeffs_p, int16_t* quant_output_p)
{
    uint8_t i;

	#pragma loop_count (64,64,8)
    for (i = 0; i < 64; i++)
    {

    	float val = dct_coeffs_p[i] * Q_INV[i];
        if (val >= 0.0f)
        {
        	quant_output_p[i] = (int16_t)(val + 0.5f);
        } else
        {
        	quant_output_p[i] = (int16_t)(val - 0.5f);
        }

    }
}

/**
 * @brief Runs the complete image processing pipeline.
 *
 * Processes the grayscale image block by block, performing
 * DCT, quantization, zig-zag transform and RLE encoding.
 *
 * @param gray_buffer_p Pointer to input grayscale image buffer
 * @param rle_results_p Pointer to output RLE blocks
 */
void RunImageProcessing(const int8_t* gray_buffer_p, RLE_block_t* rle_results_p)
{
    uint16_t bx, by;

    ResetRLE();

    for (by = 0; by < BLOCKS_Y; by++)
    {
		#pragma loop_count(32,32,32)
        for (bx = 0; bx < BLOCKS_X; bx++)
        {
            RLE_block_t* rle_out = &rle_results_p[by * BLOCKS_X + bx];

            // START_CYCLE_COUNT(start_count);
            DctBlockProcess(gray_buffer_p, dct_temp,
                            bx * BLOCK_SIZE,by * BLOCK_SIZE);
            // STOP_CYCLE_COUNT(final_count, start_count);
            // PRINT_CYCLES("Cycle num for DCT processing: ", final_count);

            // START_CYCLE_COUNT(start_count);
            QuantizeBlock(dct_temp, quant_temp);
            // STOP_CYCLE_COUNT(final_count, start_count);
            // PRINT_CYCLES("Cycle num for quantization: ", final_count);

            // START_CYCLE_COUNT(start_count);
            ZigZagTransform(quant_temp, zigzag_temp);
            // STOP_CYCLE_COUNT(final_count, start_count);
            // PRINT_CYCLES("Cycle num for zig-zag transform: ", final_count);

            // START_CYCLE_COUNT(start_count);
            RLE(zigzag_temp, rle_out);
            // STOP_CYCLE_COUNT(final_count, start_count);
            // PRINT_CYCLES("Cycle num for RLE: ", final_count);
        }
    }
}



