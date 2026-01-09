/**
 * @file    bmp_loader.h
 * @author  Dejana Smiljanic
 * @date    02.01.2026
 *
 * @brief   BMP image loader and exporter for ADSP-21489.
 *
 * @details
 * This module provides functionality for:
 * - Validating BMP files
 * - Reading image dimensions
 * - Loading 24-bit RGB BMP images
 * - Exporting RGB data to C header/source files
 *
 * Limitations:
 * - Maximum image size: 256 x 256 pixels
 * - Supported format: 24-bit uncompressed BMP
 */

#ifndef BMP_LOADER_H_
#define BMP_LOADER_H_

/* ============================================================================
 * Includes
 * ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <string.h>
#include "custom_types.h"

/* ============================================================================
 * Macro Definitions
 * ========================================================================== */

/** BMP file header size in bytes */
#define BMP_HEADER_SIZE      (14U)

/** BMP info header size in bytes */
#define BMP_INFO_HEADER_SIZE (40U)

/** BMP file signature ('BM') */
#define BMP_SIGNATURE        (0x4D42)

/** Maximum supported image width in pixels */
#define BMP_MAX_WIDTH        (256U)

/** Maximum supported image height in pixels */
#define BMP_MAX_HEIGHT       (256U)

/** Maximum number of pixels */
#define BMP_MAX_PIXELS       (BMP_MAX_WIDTH * BMP_MAX_HEIGHT)

/** Maximum number of bytes for RGB image buffer */
#define BMP_MAX_BYTES_RGB    (BMP_MAX_PIXELS * 3)

/* ============================================================================
 * Global Variables
 * ========================================================================== */

/**
 * @brief RGB image buffer.
 *
 * Stores raw RGB pixel data.
 * Buffer size is limited by BMP_MAX_RGB_BUFFER_SIZE.
 */
extern uint8_t rgb_image_buffer[BMP_MAX_BYTES_RGB];

/**
 * @brief Loaded image width in pixels.
 */
extern uint16_t image_width;

/**
 * @brief Loaded image height in pixels.
 */
extern uint16_t image_height;

/**
 * @brief Image loaded status flag.
 *
 * Value:
 * - 0U : No image loaded
 * - 1U : Image successfully loaded
 */
extern uint8_t image_loaded_flag;

/* ============================================================================
 * Function Prototypes
 * ========================================================================== */

/**
 * @brief Reads BMP image dimensions without loading pixel data.
 *
 * @param[in]  filename_ptr Pointer to BMP file name.
 * @param[out] width_ptr    Pointer to variable that receives image width.
 * @param[out] height_ptr   Pointer to variable that receives image height.
 *
 * @return  int8_t
 * @retval  0  Success
 * @retval -1  File error or invalid BMP format
 */
int8_t  BmpGetDimensions(const char* filename_ptr,
                         uint16_t* width_ptr,
                         uint16_t* height_ptr);

/**
 * @brief Validates BMP file format and constraints.
 *
 * Checks:
 * - BMP signature
 * - Header sizes
 * - Image dimensions
 * - Color depth
 *
 * @param[in] filename_ptr Pointer to BMP file name.
 *
 * @return  int8_t
 * @retval  0  BMP file is valid
 * @retval -1  BMP file is invalid or unsupported
 */
int8_t  BmpValidate(const char* filename_ptr);

/**
 * @brief Loads 24-bit RGB BMP image into memory buffer.
 *
 * @param[in] filename_ptr Pointer to BMP file name.
 *
 * @return  int8_t
 * @retval  0  Image successfully loaded
 * @retval -1  Loading failed
 */
int8_t  BmpLoadRgb(const char* filename_ptr);

/**
 * @brief Prints information about the currently loaded BMP image.
 *
 * Prints:
 * - Width
 * - Height
 * - Pixel format
 */
void    BmpPrintInfo(void);

/**
 * @brief Prints the first N RGB pixels from the image buffer.
 *
 * @param[in] count Number of pixels to print.
 */
void    BmpPrintFirstPixels(uint16_t count);

/**
 * @brief Exports loaded RGB image into C header and source files.
 *
 * Generates two files:
 * - <base_filename>.h : contains image dimensions and extern declaration
 * - <base_filename>.c : contains RGB image data array
 *
 * @param[in] base_filename_ptr Base filename without extension.
 *
 * @return uint8_t
 * @retval 0U  Export successful
 * @retval 1U  Invalid parameters or image not loaded
 * @retval 2U  File creation error
 */
uint8_t ExportRgbToHeaderAndSource(const char* base_filename_ptr);

#endif /* BMP_LOADER_H_ */
