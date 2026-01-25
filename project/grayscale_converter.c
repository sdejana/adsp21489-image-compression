/**
 * @file grayscale_converter.c
 * @author Dejana Smiljanic
 * @date 04.01.2026.
 * @brief RGB to grayscale image conversion.
 *
 * This module converts an RGB image buffer into an 8-bit grayscale image
 * using a fixed-point approximation of the standard luminance formula.
 */

#include <stdio.h>
#include "custom_types.h"
#include "grayscale_converter.h"

/**
 * @brief Grayscale image buffer stored in SRAM.
 *
 * Each pixel is stored as a signed 8-bit value with a level shift of -128,
 * which is suitable for subsequent DCT processing.
 */
#pragma section("seg_sram1")
int8_t grayscale_image_buffer_g[IMG_WIDTH*IMG_HEIGHT];

/** Fixed-point luminance coefficients. */

/** Fixed-point coefficient for the red channel (0.299 * 256). */
#define FIXED_R (77)

/** Fixed-point coefficient for the green channel (0.587 * 256). */
#define FIXED_G (150)

/** Fixed-point coefficient for the blue channel (0.114 * 256). */
#define FIXED_B (29)


/**
 * @brief Convert RGB image to grayscale.
 *
 * This function reads RGB pixel data from the input buffer and computes
 * the corresponding grayscale value using fixed-point arithmetic.
 * The result is level-shifted by -128 to match JPEG-style DCT input format.
 */
void ConvertRgbToGrayscale(void)
{
    uint32_t total_pixels = IMG_WIDTH * IMG_HEIGHT;

	#pragma loop_count(64, 65535, 8)
	for (uint32_t i = 0; i < total_pixels; i++)
	{
		uint8_t r = rgb_image_buffer[i * 3 + 0];
	    uint8_t g = rgb_image_buffer[i * 3 + 1];
	    uint8_t b = rgb_image_buffer[i * 3 + 2];

	    int32_t gray = ((FIXED_R * r + FIXED_G * g + FIXED_B * b) >> 8);

	    grayscale_image_buffer_g[i] = (int8_t)(gray - 128);
	}

}
