/*
 * rl_encoder.c
 *
 *  Created on: 17.01.2026.
 *      Author: 23dej
 */

#include <stdio.h>
#include <builtins.h>
#include "common_defs.h"
#include "rl_encoder.h"
#include "choices.h"

#pragma section("seg_sdram1")
RLE_block_t g_rle_blocks[MAX_BLOCKS];


static int16_t g_DC_prev_value = 0;
static int8_t g_first_block = 1;

void DumpRleToFile(const char* filename, RLE_block_t* rle_blocks)
{
    FILE* f = fopen(filename, "w");
    if(f == NULL) return;
    uint32_t num_blocks = (IMG_WIDTH / 8) * (IMG_HEIGHT / 8);

    for (uint32_t b = 0; b < num_blocks; b++)
    {
        fprintf(f, "BLOCK %u\n", b);
        fprintf(f, "DC %d\n", rle_blocks[b].DC_value_diff);

        for (uint32_t i = 0; i < rle_blocks[b].end_of_block; i++)
        {
            fprintf(
                f,
                "%d %d\n",
                rle_blocks[b].AC_coeffs[i].zero_num,
                rle_blocks[b].AC_coeffs[i].coeff_value
            );
        }
        fprintf(f, "EOB\n");
    }

    fclose(f);
}


void RunImageRLE(const int16_t* p_zigzag_buffer, RLE_block_t*   p_rle_blocks)
{
    uint16_t bx, by;
    uint16_t num_blocks_x = IMG_WIDTH / 8;
    uint16_t num_blocks_y = IMG_HEIGHT / 8;

    uint32_t block_idx = 0;

    ResetRLE();

    for (by = 0; by < num_blocks_y; by++)
    {
        for (bx = 0; bx < num_blocks_x; bx++)
        {
            const int16_t* zz_block =
                &p_zigzag_buffer[block_idx * 64];

            RLE(
                (int16_t*)zz_block,
                &p_rle_blocks[block_idx]
            );

            block_idx++;
        }
    }
}

void ResetRLE(void)
{
    g_DC_prev_value = 0;
    g_first_block = 1;
}

void RLE (int16_t* g_zz_p, RLE_block_t* RLE_output_p)
{
	int32_t i = 0; // iterator for zigzag array
	int32_t zero_num_i = 0; // num of repeating zeros
	int32_t ac_i = 0; // num of ac coeffs

	if(g_first_block == 1)
	{
		RLE_output_p -> DC_value_diff = g_zz_p[0];
		g_first_block = 0;
	}
	else
	{
		RLE_output_p -> DC_value_diff = g_zz_p[0] - g_DC_prev_value;
	}

	g_DC_prev_value = g_zz_p[0];

	for(i = 1; i < MAX_RLE_SYMBOLS_PER_BLOCK; i++)
	{
		//if(EXPRA(g_zz_p[i] == 0))
		if(g_zz_p[i]==0)
		{
			zero_num_i++;
		}
		else
		{
			RLE_output_p -> AC_coeffs[ac_i].zero_num = zero_num_i;
			RLE_output_p -> AC_coeffs[ac_i].coeff_value = g_zz_p[i];
			ac_i++;
			zero_num_i = 0;
		}
	}

	RLE_output_p -> AC_coeffs[ac_i].zero_num = 0;
	RLE_output_p -> AC_coeffs[ac_i].coeff_value = 0;
	RLE_output_p -> end_of_block = ac_i + 1;
}
