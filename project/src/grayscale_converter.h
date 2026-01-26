/**
 * @file grayscale_converter.h
 * @author Dejana Smiljanic
 * @date 04.01.2026.
 * @brief RGB to grayscale conversion interface.
 *
 * This header declares the grayscale image buffer and the function
 * used to convert an RGB image into a grayscale representation.
 */

#ifndef GRAYSCALE_CONVERTER_H_
#define GRAYSCALE_CONVERTER_H_

#include "custom_types.h"
#include "image_rgb.h"

/**
 * Grayscale image buffer.
 *
 * Stores the converted grayscale image as signed 8-bit values with
 * a level shift of -128. The buffer is placed in internal SRAM for
 * faster access during subsequent image processing stages.
 */
#pragma section("seg_sram1")
extern int8_t grayscale_image_buffer_g[IMG_WIDTH*IMG_HEIGHT];

/**
 * Converts an RGB image to grayscale.
 *
 * The function reads pixel data from the RGB image buffer and computes
 * grayscale values using a fixed-point luminance approximation.
 */
void ConvertRgbToGrayscale(void);

#endif /* GRAYSCALE_CONVERTER_H_ */
