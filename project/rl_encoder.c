/**
 * @file rl_encoder.c
 * @author Dejana Smiljanic
 * @date 17.01.2026.
 * @brief Run-Length Encoding implementation for JPEG-like compression.
 *
 * This file implements run-length encoding of zig-zag ordered
 * DCT coefficients, including DC prediction and AC zero run encoding.
 */

#include "rl_encoder.h"

/**
 * Global buffer holding RLE-encoded blocks.
 *
 */
#pragma section("seg_sram1")
RLE_block_t rle_blocks_g[MAX_BLOCKS];

/** Previous DC coefficient used for differential coding */
static int16_t DC_prev_value_g = 0;

/** Flag indicating the first encoded block */
static int8_t first_block_g = 1;


/**
 * Resets RLE internal state.
 *
 * Clears DC prediction state and prepares the encoder
 * for a new image or frame.
 */
void ResetRLE(void)
{
    DC_prev_value_g = 0;
    first_block_g = 1;
}

/**
 * Performs run-length encoding on a single 8x8 DCT block.
 *
 * The input block must be in zig-zag order. DC coefficient
 * is differentially encoded, while AC coefficients are
 * encoded using (zero-run, value) pairs.
 *
 * An explicit End-Of-Block (EOB) symbol is appended.
 *
 * @param zigzag_p      Pointer to zig-zag ordered DCT coefficients.
 * @param RLE_output_p  Pointer to the output RLE block structure.
 */
void RLE (int16_t* zigzag_p, RLE_block_t* RLE_output_p)
{
	int32_t i = 0; /**< Zig-zag coefficient index */
	int32_t zero_num_i = 0; /**< Count of consecutive zero AC coefficients */
	int32_t ac_i = 0;  /**< Number of encoded AC symbols */

	/* DC differential encoding */
	if(first_block_g == 1)
	{
		RLE_output_p -> DC_value_diff = zigzag_p[0];
		first_block_g = 0;
	}
	else
	{
		RLE_output_p -> DC_value_diff = zigzag_p[0] - DC_prev_value_g;
	}

	DC_prev_value_g = zigzag_p[0];

    /* AC run-length encoding */
	for(i = 1; i < MAX_RLE_SYMBOLS_PER_BLOCK; i++)
	{
		if(EXPRA(zigzag_p[i] == 0))
		{
			zero_num_i++;
		}
		else
		{
			RLE_output_p -> AC_coeffs[ac_i].zero_num = zero_num_i;
			RLE_output_p -> AC_coeffs[ac_i].coeff_value = zigzag_p[i];

			ac_i++;
			zero_num_i = 0;
		}
	}

    /* Explicit End-Of-Block marker */
	RLE_output_p -> AC_coeffs[ac_i].zero_num = 0;
	RLE_output_p -> AC_coeffs[ac_i].coeff_value = 0;
	RLE_output_p -> end_of_block = ac_i + 1;
}

/**
 * Dumps all RLE-encoded blocks to a text file.
 *
 * @param filename_p Pointer to the output file name.
 */
void DumpRleToFile(const char* filename_p)
{
    FILE* f = fopen(filename_p, "w");
    if (f == NULL) return;

    uint32_t num_blocks = (IMG_WIDTH / 8) * (IMG_HEIGHT / 8);
    char line[128];

    for (uint32_t b = 0; b < num_blocks; b++)
    {

        int len = snprintf(line, sizeof(line), "BLOCK %u\n", b);
        fwrite(line, 1, len, f);

        len = snprintf(line, sizeof(line), "DC %d\n",
                       rle_blocks_g[b].DC_value_diff);
        fwrite(line, 1, len, f);

        for (uint32_t i = 0; i < rle_blocks_g[b].end_of_block; i++)
        {
            len = snprintf(
                line,
                sizeof(line),
                "%d %d\n",
                rle_blocks_g[b].AC_coeffs[i].zero_num,
                rle_blocks_g[b].AC_coeffs[i].coeff_value
            );
            fwrite(line, 1, len, f);
        }

        fwrite("EOB\n", 1, 4, f);
    }

    fclose(f);
}

