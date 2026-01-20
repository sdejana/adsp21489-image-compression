/*
 * rl_encoder.h
 *
 *  Created on: 17.01.2026.
 *      Author: 23dej
 */

#ifndef RL_ENCODER_H_
#define RL_ENCODER_H_

#include "custom_types.h"
#include "common_defs.h"
#include "lookup_tables.h"

typedef struct
{
	int16_t zero_num; // padding would be added i guess
	int16_t coeff_value;
} RLE_pair_t;

typedef struct
{
	RLE_pair_t AC_coeffs[MAX_RLE_SYMBOLS_PER_BLOCK];
	int16_t DC_value_diff;
	uint8_t end_of_block; // represents AC coeffs number  (so i know where end of AC_coeffs actually is)
} RLE_block_t;

void DumpRleToFile(const char* filename, RLE_block_t* rle_blocks);
void RunImageRLE(const int16_t* p_zigzag_buffer, RLE_block_t*   p_rle_blocks);
void ResetRLE(void);
void RLE (int16_t* g_zz_p, RLE_block_t* RLE_output_p);

#pragma section("seg_sdram1")
extern RLE_block_t g_rle_blocks[MAX_BLOCKS];

#endif /* RL_ENCODER_H_ */
