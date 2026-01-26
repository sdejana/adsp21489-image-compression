/**
 * @file common_defs.h
 * @author Dejana Smiljanic
 * @date 02.01.2026.
 * @brief Common definitions and constants for image block processing.
 *
 * This file contains geometric constants and buffer size limits used for
 * dividing the input image into 8x8 blocks and managing RLE symbols.
 */

#ifndef COMMON_DEFS_H_
#define COMMON_DEFS_H_

#include "image_rgb.h"

/** @brief Dimension of a single square DCT block (8x8). */
#define BLOCK_SIZE  (8U)

/** @brief Number of blocks along the horizontal axis. */
#define BLOCKS_X    (IMG_WIDTH / BLOCK_SIZE)

/** @brief Number of blocks along the vertical axis. */
#define BLOCKS_Y    (IMG_HEIGHT / BLOCK_SIZE)

/** @brief Total number of blocks in the current image. */
#define TOTAL_BLOCKS (BLOCKS_X * BLOCKS_Y)

/** @brief Maximum possible blocks along X axis based on image constraints. */
#define MAX_BLOCKS_X   (IMG_WIDTH  / 8)

/** @brief Maximum possible blocks along Y axis based on image constraints. */
#define MAX_BLOCKS_Y   (IMG_HEIGHT / 8)

/** @brief Global maximum number of blocks the system can handle. */
#define MAX_BLOCKS     (MAX_BLOCKS_X * MAX_BLOCKS_Y)

/** * @brief Maximum number of Run-Length Encoding symbols in a single 8x8 block.
 * Since an 8x8 block has 64 coefficients, the RLE sequence cannot exceed 64 pairs.
 */
#define MAX_RLE_SYMBOLS_PER_BLOCK (64U)

#endif /* COMMON_DEFS_H_ */
