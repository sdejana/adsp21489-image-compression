/*
 * common_defs.h
 *
 *  Created on: 02.01.2026.
 *      Author: 23dej
 */

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

//#define IMG_WIDTH (64U)
//#define IMG_HEIGHT (64U)

#include "image_rgb.h"

#define BLOCK_SIZE  (8U)
#define BLOCKS_X    (IMG_WIDTH / BLOCK_SIZE)
#define BLOCKS_Y    (IMG_HEIGHT / BLOCK_SIZE)
#define TOTAL_BLOCKS (BLOCKS_X * BLOCKS_Y)

#define MAX_BLOCKS_X   (IMG_WIDTH  / 8)
#define MAX_BLOCKS_Y   (IMG_HEIGHT / 8)
#define MAX_BLOCKS     (MAX_BLOCKS_X * MAX_BLOCKS_Y)
#define MAX_RLE_SYMBOLS_PER_BLOCK (64U)

#endif /* COMMON_DEFS_H_ */
