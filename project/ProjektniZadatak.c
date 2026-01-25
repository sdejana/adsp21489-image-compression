/**
 * @file ProjektniZadatak.c
 * @author Dejana Smiljanic
 * @date 23.01.2026.
 *
 * @brief Main application entry point for image processing pipeline.
 *
 * This file contains the main function that initializes the system,
 * performs RGB to grayscale conversion, block-based image processing
 * (DCT, quantization, zig-zag, RLE), and serializes the results.
 *
 * Execution stages are indicated using onboard LEDs and cycle counting
 * is used to measure performance of individual processing steps.
 */

#include <sys/platform.h>
#include <stdio.h>
#include "adi_initialize.h"
#include "ProjektniZadatak.h"
#include "grayscale_converter.h"
#include "process_image.h"
#include "LED.h"

#include <cycle_count.h>

/** Cycle counter start value */
cycle_t start_count;

/** Cycle counter end value */
cycle_t final_count;

/**
 * Application entry point.
 *
 * Initializes hardware components and LEDs, then executes the full
 * image processing pipeline:
 *  - RGB to grayscale conversion
 *  - Block-based image processing (DCT, quantization, zig-zag, RLE)
 *  - Serialization of RLE data to file
 *
 * LEDs are used to indicate progress through individual stages,
 * while cycle counters measure execution time of each step.
 *
 * @return Program exit status (always 0).
 */
int main()
{

	adi_initComponents();
	LedInit();
	LedOn(0);

    /* Grayscale conversion */
	LedOn(1);
	START_CYCLE_COUNT(start_count);
	ConvertRgbToGrayscale();
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Cycle num for grayscale conversion: ", final_count);

	/* Image processing: DCT, quantization, zig-zag, RLE */
	LedOn(2);
	START_CYCLE_COUNT(start_count);
	RunImageProcessing(grayscale_image_buffer_g, rle_blocks_g);
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Cycle num for processing: ", final_count);
	LedOn(3);

	/* RLE data serialization */
	LedOn(4);
	START_CYCLE_COUNT(start_count);
	DumpRleToFile("rle_dump.txt");
	STOP_CYCLE_COUNT(final_count, start_count);
	PRINT_CYCLES("Cycle num for serialization: ", final_count);

	/* Process finished */
	LedOn(5);
	LedOn(6);
	LedOn(7);

	return 0;
}
