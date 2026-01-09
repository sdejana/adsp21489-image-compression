/**
 * @file    bmp_loader.c
 * @author  Dejana Smiljanic
 * @date    04.01.2026
 *
 * @brief   Implementation of BMP image loader.
 *
 * @details
 * Supports loading and exporting uncompressed 24-bit BMP images
 * with maximum resolution of 256x256 pixels.
 */

#include "bmp_loader.h"

/* ============================================================================
 * Global Variables
 * ========================================================================== */

uint8_t  rgb_image_buffer[BMP_MAX_BYTES_RGB];
uint16_t image_width = 0;
uint16_t image_height = 0;
uint8_t  image_loaded_flag = 0;

/* ============================================================================
 * Private Structures
 * ========================================================================== */

#pragma pack(push, 1)

/**
 * @brief BMP file header structure.
 */
typedef struct
{
	int16_t signature;
	int32_t file_size;
	int32_t reserved;
	int32_t data_offset;
} bmp_file_header_t;

/**
 * @brief BMP info header.
 */
typedef struct
{
	int32_t size;
	int32_t width;
	int32_t height;
	int16_t planes;
	int16_t bits_per_pixels;
	int32_t compression;
	int32_t image_size;
	int32_t x_pixels_per_m;
	int32_t y_pixels_per_m;
	int32_t colors_used;
	int32_t important_colors;
} bmp_info_header_t;

#pragma pack(pop)

/* ============================================================================
 * Private Function Prototypes
 * ========================================================================== */
static int8_t ReadBmpHeaders(FILE* fp,
                            bmp_file_header_t* file_header_ptr,
                            bmp_info_header_t* info_header_ptr);

static uint8_t CalculateRowPadding(uint16_t width_pixels);

/* ============================================================================
 * Private Functions
 * ========================================================================== */

/**
 * @brief Reads and validates BMP headers.
 *
 * @param[in]  fp              Open file pointer.
 * @param[out] file_header_ptr Pointer to BMP file header.
 * @param[out] info_header_ptr Pointer to BMP info header.
 *
 * @return int8_t
 * @retval  0  Success
 * @retval -1  Invalid parameters or read error
 * @retval -2  Invalid BMP signature
 * @retval -3  Unsupported pixel format
 */
static int8_t ReadBmpHeaders(FILE* fp,
                            bmp_file_header_t* file_header_ptr,
						    bmp_info_header_t* info_header_ptr)
{
	if ((fp == NULL) || (file_header_ptr == NULL) || (info_header_ptr == NULL))
    {
        return -1;
    }

    if (fread(file_header_ptr, sizeof(bmp_file_header_t), 1U, fp) != 1U)
    {
        return -1;
    }

    if (fread(info_header_ptr, sizeof(bmp_info_header_t), 1U, fp) != 1U)
    {
        return -1;
    }

    if (file_header_ptr->signature != BMP_SIGNATURE)
    {
        return -2;
    }

    if (info_header_ptr->bits_per_pixels != 24U)
    {
        return -3;
    }

    return 0;
}

/**
 * @brief
 * @param width
 */
static uint8_t CalculateRowPadding(uint16_t width_pixels)
{
	uint32_t row_size_bytes = (uint32_t)width_pixels * 3U;
    return (uint8_t)((4U - (row_size_bytes & 3U)) & 3U);
}

/* ============================================================================
 * Public Functions
 * ========================================================================== */

int8_t BmpGetDimensions(const char* filename_ptr,
                        uint16_t* width_ptr,
                        uint16_t* height_ptr)
{
    FILE* fp;
    bmp_file_header_t file_header;
    bmp_info_header_t info_header;
    int8_t result;

    if ((filename_ptr == NULL) || (width_ptr == NULL) || (height_ptr == NULL))
    {
        return -1;
    }

    fp = fopen(filename_ptr, "rb");
    if (fp == NULL)
    {
        return -1;
    }

    result = ReadBmpHeaders(fp, &file_header, &info_header);
    fclose(fp);

    if (result != 0)
    {
        return result;
    }

    *width_ptr  = (uint16_t)info_header.width;
    *height_ptr = (uint16_t)info_header.height;

    return 0;
}

int8_t BmpValidate(const char* filename_ptr)
{
     uint16_t width;
    uint16_t height;
    int8_t result;

    result = BmpGetDimensions(filename_ptr, &width, &height);
    if (result != 0)
    {
        return result;
    }

    if ((width > BMP_MAX_WIDTH) || (height > BMP_MAX_HEIGHT))
    {
        return -4;
    }

    if (((width % 8U) != 0U) || ((height % 8U) != 0U))
    {
        return -5;
    }

    return 0;
}

int8_t BmpLoadRgb(const char* filename_ptr)
{
     FILE* fp;
    bmp_file_header_t file_header;
    bmp_info_header_t info_header;
    uint8_t padding;
    int8_t result;

    if (filename_ptr == NULL)
    {
        return -1;
    }

    fp = fopen(filename_ptr, "rb");
    if (fp == NULL)
    {
        return -1;
    }

    result = ReadBmpHeaders(fp, &file_header, &info_header);
    if (result != 0)
    {
        fclose(fp);
        return result;
    }

    image_width  = (uint16_t)info_header.width;
    image_height = (uint16_t)info_header.height;

    padding = CalculateRowPadding(image_width);

    if (fseek(fp, (long)file_header.data_offset, SEEK_SET) != 0)
    {
        fclose(fp);
        return -1;
    }

    for (int32_t y = (int32_t)image_height - 1; y >= 0; y--)
    {
        for (uint16_t x = 0U; x < image_width; x++)
        {
            uint8_t b = (uint8_t)fgetc(fp);
            uint8_t g = (uint8_t)fgetc(fp);
            uint8_t r = (uint8_t)fgetc(fp);

            uint32_t index = ((uint32_t)y * image_width + x) * 3U;
            rgb_image_buffer[index + 0U] = r;
            rgb_image_buffer[index + 1U] = g;
            rgb_image_buffer[index + 2U] = b;
        }

        (void)fseek(fp, padding, SEEK_CUR);
    }

    fclose(fp);
    image_loaded_flag = 1U;

    return 0;
}

void BmpPrintInfo(void)
{
    printf("=== BMP Loader Info ===\n");
    printf("Image loaded: %s\n", (image_loaded_flag != 0U) ? "YES" : "NO");
    printf("Dimensions: %ux%u\n", image_width, image_height);
    printf("Buffer size: %lu bytes\n",
          (unsigned long)(image_width * image_height * 3U));
}

void BmpPrintFirstPixels(uint16_t count)
{
    uint32_t max_pixels;

    if (image_loaded_flag == 0U)
    {
        printf("No image loaded!\n");
        return;
    }

    max_pixels = (uint32_t)image_width * image_height;

    for (uint32_t i = 0U; (i < count) && (i < max_pixels); i++)
    {
        printf("Pixel %lu: R=%3u G=%3u B=%3u\n",
               (unsigned long)i,
               rgb_image_buffer[i * 3U],
               rgb_image_buffer[i * 3U + 1U],
               rgb_image_buffer[i * 3U + 2U]);
    }
}

uint8_t ExportRgbToHeaderAndSource(const char* base_filename_ptr)
{
    char header_filename[256];
    char source_filename[256];
    const char* filename;
    uint32_t total_bytes;
    FILE* h_file;
    FILE* c_file;
    uint8_t status = 0U;

    if ((base_filename_ptr == NULL) || (image_loaded_flag == 0U))
    {
        return 1U;
    }

    filename = strrchr(base_filename_ptr, '/');
    if (filename != NULL)
    {
        filename++;
    }
    else
    {
        filename = base_filename_ptr;
    }

    if ((snprintf(header_filename,
                  sizeof(header_filename),
                  "%s.h",
                  base_filename_ptr) <= 0) ||
        (snprintf(source_filename,
                  sizeof(source_filename),
                  "%s.c",
                  base_filename_ptr) <= 0))
    {
        return 1U;
    }

    total_bytes = (uint32_t)image_width * image_height * 3U;

    h_file = fopen(header_filename, "w");
    if (h_file == NULL)
    {
        return 1U;
    }

    fprintf(h_file, "#ifndef IMAGE_RGB_H\n#define IMAGE_RGB_H\n\n");
    fprintf(h_file, "#include <stdint.h>\n");
    fprintf(h_file, "#include \"custom_types.h\"\n\n");
    fprintf(h_file, "#define IMG_WIDTH %u\n#define IMG_HEIGHT %u\n\n",
            image_width, image_height);
    fprintf(h_file,
            "extern const uint8_t rgb_image_buffer[%luU];\n\n",
            (unsigned long)total_bytes);
    fprintf(h_file, "#endif\n");
    fclose(h_file);

    c_file = fopen(source_filename, "w");
    if (c_file == NULL)
    {
        return 1U;
    }

    fprintf(c_file, "#include \"%s.h\"\n\n", filename);
    fprintf(c_file,
            "const uint8_t rgb_image_buffer[%luU] = {\n",
            (unsigned long)total_bytes);

    for (uint32_t i = 0U; i < total_bytes; i += 16U)
    {
        fprintf(c_file, "    ");
        for (uint32_t j = 0U; (j < 16U) && ((i + j) < total_bytes); j++)
        {
            fprintf(c_file,
                    "0x%02X%s",
                    rgb_image_buffer[i + j],
                    ((i + j + 1U) < total_bytes) ? ", " : "");
        }
        fprintf(c_file, "\n");
    }

    fprintf(c_file, "};\n");
    fclose(c_file);

    return status;
}
