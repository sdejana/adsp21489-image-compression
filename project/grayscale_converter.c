/*
 * grayscale_converter.c
 *
 *  Created on: 04.01.2026.
 *      Author: 23dej
 */

#include <stdio.h>
#include "custom_types.h"
#include "grayscale_converter.h"


#pragma section("seg_sdram1")
int8_t grayscale_image_buffer[IMG_WIDTH*IMG_HEIGHT];

#define FIXED_R (77)    // 0.299 * 256
#define FIXED_G (150)   // 0.587 * 256
#define FIXED_B (29)    // 0.114 * 256


void ConvertRgbToGrayscale(void)
{
    uint32_t total_pixels = IMG_WIDTH * IMG_HEIGHT;

	for (uint32_t i = 0; i < total_pixels; i++)
	{
	        uint8_t r = rgb_image_buffer[i * 3 + 0];
	        uint8_t g = rgb_image_buffer[i * 3 + 1];
	        uint8_t b = rgb_image_buffer[i * 3 + 2];
	        int32_t gray = (FIXED_R * r + FIXED_G * g + FIXED_B * b) >> 8;

	        // Centered around zero
	        grayscale_image_buffer[i] = (int8_t)(gray - 128);
	}
}

/*
#pragma optimize_for_speed
void ConvertRgbToGrayscale(void)
{
    uint32_t total_pixels = IMG_WIDTH * IMG_HEIGHT;

    const uint8_t *p_rgb = rgb_image_buffer;
    int8_t *p_gray = grayscale_image_buffer;

    #pragma loop_count(91136, 91136, 4)
    #pragma SIMD_for
    for (uint32_t i = 0; i < total_pixels; i++) {

        uint8_t r = *p_rgb++;
        uint8_t g = *p_rgb++;
        uint8_t b = *p_rgb++;

        int32_t gray = (77 * r + 150 * g + 29 * b) >> 8; // MAC

        *p_gray++ = (int8_t)(gray - 128);
    }
}

*/
void PrintGrayscaleInfo(void) {
    printf("=== Grayscale Image ===\n");
    printf("Dimensions: %dx%d\n", IMG_WIDTH, IMG_HEIGHT);
    printf("Buffer: gray_image_buffer[%d]\n", IMG_WIDTH * IMG_HEIGHT);

    if (IMG_WIDTH * IMG_HEIGHT > 0) {
        printf("First pixel value: %d\n", grayscale_image_buffer[0]);
        printf("Range check: %d to %d\n",
        		grayscale_image_buffer[0],
			   grayscale_image_buffer[IMG_WIDTH * IMG_HEIGHT - 1]);
    }
}

void PrintFirstGrayPixels(uint16_t count) {
   /* if (!image_loaded) {
        printf("No image loaded!\n");
        return;
    }*/

    uint16_t limit = count;
    if (limit > IMG_WIDTH * IMG_HEIGHT) {
        limit = IMG_WIDTH * IMG_HEIGHT;
    }

    printf("First %d grayscale pixels (centered):\n", limit);
    for (uint16_t i = 0; i < limit; i++) {
        printf("  Pixel %d: %d\n", i, grayscale_image_buffer[i]);
    }
}
