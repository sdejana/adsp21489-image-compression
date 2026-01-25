/**
 * @file rl_encoder.h
 * @author Dejana Smiljanic
 * @date 17.01.2026.
 * @brief Run-Length Encoding (RLE) interface for DCT coefficients.
 *
 * This module defines data structures and functions used for
 * run-length encoding of zig-zag ordered DCT coefficients.
 */

#ifndef RL_ENCODER_H_
#define RL_ENCODER_H_

#include <stdio.h>
#include <builtins.h>
#include "custom_types.h"
#include "common_defs.h"
#include "lookup_tables.h"
#include "choices.h"

/**
 * Represents a single run-length encoded AC coefficient.
 *
 * zero_num specifies the number of preceding zero coefficients,
 * while coeff_value stores the non-zero DCT coefficient value.
 */
typedef struct
{
	int16_t zero_num;     /**< Number of preceding zero coefficients */
	int16_t coeff_value;  /**< Non-zero AC coefficient value */
} RLE_pair_t;


/**
 * Represents an RLE-encoded 8x8 DCT block.
 *
 * Stores the DC difference value and a list of run-length encoded
 * AC coefficients. The end_of_block field indicates how many
 * AC symbols are valid in the AC_coeffs array.
 */
typedef struct
{
	RLE_pair_t AC_coeffs[MAX_RLE_SYMBOLS_PER_BLOCK];  /**< AC RLE symbols */
	int16_t DC_value_diff; /**< Differential DC value */
	uint8_t end_of_block;
} RLE_block_t;


/**
 * Resets internal RLE state and buffers.
 *
 * Should be called before starting a new encoding sequence.
 */
void ResetRLE(void);

/**
 * Performs run-length encoding on a zig-zag ordered DCT block.
 *
 * @param zigzag_p       Pointer to 64 zig-zag ordered DCT coefficients.
 * @param RLE_output_p  Pointer to the output RLE block structure.
 */
void RLE(int16_t* zigzag_p, RLE_block_t* RLE_output_p);

/**
 * Dumps RLE-encoded blocks to a text file.
 *
 * @param filename_p Pointer to the output file name.
 */
void DumpRleToFile(const char* filename_p);

/**
 * Global buffer for RLE-encoded blocks.
 *
 * The buffer is placed in internal SRAM for fast access during
 * the encoding process.
 */
#pragma section("seg_sram1")
extern RLE_block_t rle_blocks_g[MAX_BLOCKS];

#endif /* RL_ENCODER_H_ */
