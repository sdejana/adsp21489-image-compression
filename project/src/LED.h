/**
 * @file LED.h
 * @author Dejana Smiljanic
 * @date 25.01.2026.
 * @brief LED control imterface.
 *
 * This header provides function declarations and definitions
 * for controlling onboard LEDs using GPIO and SRU configuration.
 * The module supports basic LED operations such as initialization,
 * turning individual LEDs on or off, blinking, and enabling all LEDs.
 *
 * It is mainly intended for visual feedback during image processing
 * and DSP execution.
 */

#ifndef LED_H_
#define LED_H_

#include <sys/platform.h>
#include <cdef21489.h>
#include <def21489.h>
#include <sru21489.h>
#include <SYSREG.h>
#include <stdio.h>
#include "adi_initialize.h"
#include "custom_types.h"


/**
 * Number of available LEDs on the board.
 */
#define LED_NUM (8U)

/**
 * Initializes SRU (Signal Routing Unit) connections
 * required for LED control.
 */
void SruInit(void);

/**
 * Initializes LED GPIO pins and sets their initial state.
 */
void LedInit(void);

/**
 * Turns on a single LED.
 *
 * @param led_number Index of the LED to turn on (0 to LED_NUM-1).
 */
void LedOn(uint8_t led_number);


/**
 * Turns off all-DAI controlled LEDs.
 */
void LedsOff(void);


#endif /* LED_H_ */
