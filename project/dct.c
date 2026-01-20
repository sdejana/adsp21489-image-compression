#include "dct.h"
#include "custom_types.h"
#include "lookup_tables.h"
#include "common_defs.h"

#pragma align 4
static float s_inter[64];

//#pragma optimize_for_speed
/*void process_block_2d_dct_ultra(const int8_t* p_input, float* p_output, uint16_t start_x, uint16_t start_y) {
    int r, c;
    float block[64];

    for (r = 0; r < 8; r++) {
        const int8_t* src = &p_input[(start_y + r) * IMG_WIDTH + start_x];
        float* dst = &block[r * 8];
        dst[0]=(float)src[0]; dst[1]=(float)src[1]; dst[2]=(float)src[2]; dst[3]=(float)src[3];
        dst[4]=(float)src[4]; dst[5]=(float)src[5]; dst[6]=(float)src[6]; dst[7]=(float)src[7];
    }

    #pragma SIMD_for
    #pragma loop_count(8, 8, 8)
    for (r = 0; r < 8; r++) {
		#pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (c = 0; c < 8; c++) {
            float sum;
            const float* row_T = &T[r][0];
            const float* col_B = &block[c];


            sum  = row_T[0] * col_B[0*8];
            sum += row_T[1] * col_B[1*8];
            sum += row_T[2] * col_B[2*8];
            sum += row_T[3] * col_B[3*8];
            sum += row_T[4] * col_B[4*8];
            sum += row_T[5] * col_B[5*8];
            sum += row_T[6] * col_B[6*8];
            sum += row_T[7] * col_B[7*8];

            s_inter[r * 8 + c] = sum;
        }
    }



    for (r = 0; r < 8; r++) {
		#pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (c = 0; c < 8; c++) {
            float sum;
            const float* row_I = &s_inter[r * 8];
            const float* row_T = &T[c][0]; //

            sum  = row_I[0] * row_T[0];
            sum += row_I[1] * row_T[1];
            sum += row_I[2] * row_T[2];
            sum += row_I[3] * row_T[3];
            sum += row_I[4] * row_T[4];
            sum += row_I[5] * row_T[5];
            sum += row_I[6] * row_T[6];
            sum += row_I[7] * row_T[7];

            p_output[r * 8 + c] = sum;
        }
    }
}
*/
/*

#pragma optimize_for_speed
void process_block_2d_dct_ultra(const int8_t* __restrict p_input, float* __restrict p_output, uint16_t start_x, uint16_t start_y) {
    float block[64] __attribute__((aligned(4)));
    float * __restrict p_inter = s_inter;
    float * __restrict p_out = p_output;

    for (int r = 0; r < 8; r++) {
        const int8_t* src = &p_input[(start_y + r) * IMG_WIDTH + start_x];
        for(int c = 0; c < 8; c++) block[(r << 3) + c] = (float)src[c];
    }

    for (int r = 0; r < 8; r++) {
        const float* row_T = &T[r][0];

        #pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (int c = 0; c < 8; c++) {
            const float* col_B = &block[c];
            *p_inter++ = (row_T[0] * col_B[0])  + (row_T[1] * col_B[8])  +
                         (row_T[2] * col_B[16]) + (row_T[3] * col_B[24]) +
                         (row_T[4] * col_B[32]) + (row_T[5] * col_B[40]) +
                         (row_T[6] * col_B[48]) + (row_T[7] * col_B[56]);
        }
    }

    for (int r = 0; r < 8; r++) {
        const float* row_I = &s_inter[r << 3];

        #pragma SIMD_for
        #pragma loop_count(8, 8, 8)
        for (int c = 0; c < 8; c++) {
            const float* row_T_trans = &T[c][0];

            *p_out++ = (row_I[0] * row_T_trans[0]) + (row_I[1] * row_T_trans[1]) +
                       (row_I[2] * row_T_trans[2]) + (row_I[3] * row_T_trans[3]) +
                       (row_I[4] * row_T_trans[4]) + (row_I[5] * row_T_trans[5]) +
                       (row_I[6] * row_T_trans[6]) + (row_I[7] * row_T_trans[7]);
        }
    }
}

*/

/*
void process_block_2d_dct_ultra(const int8_t* p_input, float* p_output, uint16_t start_x, uint16_t start_y) {
    int r, c;
    float block[64];

    for (r = 0; r < 8; r++) {
        const int8_t* src = &p_input[(start_y + r) * IMG_WIDTH + start_x];
        float* dst = &block[r * 8];
        dst[0]=(float)src[0]; dst[1]=(float)src[1]; dst[2]=(float)src[2]; dst[3]=(float)src[3];
        dst[4]=(float)src[4]; dst[5]=(float)src[5]; dst[6]=(float)src[6]; dst[7]=(float)src[7];
    }

    #pragma SIMD_for
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            float sum;
            const float* row_T = &T[r][0];
            const float* col_B = &block[c];


            sum  = row_T[0] * col_B[0*8];
            sum += row_T[1] * col_B[1*8];
            sum += row_T[2] * col_B[2*8];
            sum += row_T[3] * col_B[3*8];
            sum += row_T[4] * col_B[4*8];
            sum += row_T[5] * col_B[5*8];
            sum += row_T[6] * col_B[6*8];
            sum += row_T[7] * col_B[7*8];

            s_inter[r * 8 + c] = sum;
        }
    }


    #pragma SIMD_for
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            float sum;
            const float* row_I = &s_inter[r * 8];
            const float* row_T = &T[c][0]; //

            sum  = row_I[0] * row_T[0];
            sum += row_I[1] * row_T[1];
            sum += row_I[2] * row_T[2];
            sum += row_I[3] * row_T[3];
            sum += row_I[4] * row_T[4];
            sum += row_I[5] * row_T[5];
            sum += row_I[6] * row_T[6];
            sum += row_I[7] * row_T[7];

            p_output[r * 8 + c] = sum;
        }
    }
}
*/


void process_block_2d_dct_ultra(const int8_t* p_input, float* p_output, uint16_t start_x, uint16_t start_y) {
    int r, c, k;
    #pragma align 4
    float block[64];
    #pragma align 4
    float s_inter[64];

    for (r = 0; r < 8; r++) {
        const int8_t* src = &p_input[(start_y + r) * IMG_WIDTH + start_x];
        float* dst = &block[r * 8];
        dst[0]=(float)src[0]; dst[1]=(float)src[1]; dst[2]=(float)src[2]; dst[3]=(float)src[3];
        dst[4]=(float)src[4]; dst[5]=(float)src[5]; dst[6]=(float)src[6]; dst[7]=(float)src[7];
    }

    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            float sum = 0.0f;
            #pragma loop_count(8, 8, 8)
            for(k=0; k<8; k++) {
                sum += T[r][k] * block[k * 8 + c];
            }
            s_inter[r * 8 + c] = sum;
        }
    }

    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            float sum = 0.0f;
            #pragma loop_count(8, 8, 8)
            for(k=0; k<8; k++) {
                sum += s_inter[r * 8 + k] * T[c][k];
            }
            p_output[r * 8 + c] = sum;
        }
    }
}


void ZigZagScan(const int16_t *input, int16_t *output)
{
	#pragma loop_count(64, 64, 64)
    for (int i = 0; i < 64; i++)
    {
        output[i] = input[zigzag_index[i]];
    }
}


