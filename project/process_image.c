/*
 * process_image.c
 *
 *  Created on: 17.01.2026.
 *      Author: 23dej
 */

#include "process_image.h"

void RunImagePipeline(const int8_t* p_gray_buffer, float* p_dct_results, int16_t* p_quant_results,
		                int16_t* p_zigzag_results) {
    uint16_t bx, by;
    uint16_t num_blocks_x = IMG_WIDTH / BLOCK_SIZE;
    uint16_t num_blocks_y = IMG_HEIGHT / BLOCK_SIZE;

    for (by = 0; by < num_blocks_y; by++) {
        for (bx = 0; bx < num_blocks_x; bx++) {

            uint32_t block_index = (by * num_blocks_x + bx) * 64;

            float* p_current_dct = &p_dct_results[block_index];

            int16_t* p_current_quant = &p_quant_results[block_index];

            int16_t* p_current_zz    = &p_zigzag_results[block_index];

            process_block_2d_dct_ultra(
                p_gray_buffer,
                p_current_dct,
                (uint16_t)(bx * BLOCK_SIZE),
                (uint16_t)(by * BLOCK_SIZE)
            );

            QuantizeBlockUltra(p_current_dct, p_current_quant);
            ZigZagScan(p_current_quant, p_current_zz);
        }
    }
}

void QuantizeBlockUltra(const float* p_dct_coeffs, int16_t* p_quant_output) {
    int i;

    //#pragma SIMD_for
    for (i = 0; i < 64; i++) {
        float val = p_dct_coeffs[i] * Q_INV[i];
        if (val >= 0.0f) {
            p_quant_output[i] = (int16_t)(val + 0.5f);
        } else {
            p_quant_output[i] = (int16_t)(val - 0.5f);
        }
    }
}

void ispisi_quant(int16_t* g_dct_coeffs, uint16_t block_y, uint16_t block_x)
{
	uint16_t r, c;
	    uint16_t num_blocks_x = 64 / 8;

	    uint32_t block_offset = (block_y * num_blocks_x + block_x) * 64;
	    int16_t* p_block = &g_dct_coeffs[block_offset];

	    printf("\n--- DCT Block [%d, %d] ---\n", block_x, block_y);

	    for (r = 0; r < 8; r++)
	    {
	        for (c = 0; c < 8; c++)
	        {
	            printf("%d ", p_block[r * 8 + c]);
	        }
	        printf("\n");
	    }
}

#include <cycle_count.h>
cycle_t start_count;
cycle_t final_count;

// Dodaj ove sekcije iznad funkcija u process_image.c
#pragma align 2
static section("seg_dmda") float dct_temp[64];

#pragma align 2
static section("seg_dmda") int16_t quant_temp[64];

#pragma align 2
static section("seg_dmda") int16_t zigzag_temp[64];

void RunImagePipelineRLE(const int8_t* p_gray_buffer, RLE_block_t* p_rle_results) {
    uint16_t bx, by;
    /*float dct_temp[64];
    int16_t quant_temp[64];
    int16_t zigzag_temp[64];*/

    ResetRLE();

    for (by = 0; by < BLOCKS_Y; by++) {
        for (bx = 0; bx < BLOCKS_X; bx++) {
            RLE_block_t* rle_out = &p_rle_results[by * BLOCKS_X + bx];

            // 1. DCT
            //START_CYCLE_COUNT(start_count);
            process_block_2d_dct_ultra(
                p_gray_buffer,
                dct_temp,
                bx * BLOCK_SIZE,
                by * BLOCK_SIZE
            );
           // STOP_CYCLE_COUNT(final_count, start_count);
            //PRINT_CYCLES("Cycle num for DCT conversion: ", final_count);

            // 2. Kvantizacija
            //START_CYCLE_COUNT(start_count);
            QuantizeBlockUltra(dct_temp, quant_temp);
            //STOP_CYCLE_COUNT(final_count, start_count);
            //PRINT_CYCLES("Cycle num for quantization: ", final_count);

            // 3. ZigZag
           // START_CYCLE_COUNT(start_count);
            ZigZagScan(quant_temp, zigzag_temp);
           // STOP_CYCLE_COUNT(final_count, start_count);
           // PRINT_CYCLES("Cycle num for zigzag: ", final_count);

            // 4. RLE direktno u izlaz
            //START_CYCLE_COUNT(start_count);
            RLE(zigzag_temp, rle_out);
           // STOP_CYCLE_COUNT(final_count, start_count);
           //PRINT_CYCLES("Cycle num for RLE: ", final_count);
        }
    }
}
