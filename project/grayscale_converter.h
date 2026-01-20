/*
 * grayscale_convertor.h
 *
 *  Created on: 04.01.2026.
 *      Author: 23dej
 */

#ifndef GRAYSCALE_CONVERTER_H_
#define GRAYSCALE_CONVERTER_H_

#include "custom_types.h"
#include "image_rgb.h"

extern int8_t grayscale_image_buffer[IMG_WIDTH*IMG_HEIGHT];

/**
 * Main function for conversion from rgb to grayscale
 */
void ConvertRgbToGrayscale(void);

/**
 * Debug functions
 */
void PrintGrayscaleInfo(void);
void PrintFirstGrayPixels(uint16_t count);

#endif /* GRAYSCALE_CONVERTER_H_ */
