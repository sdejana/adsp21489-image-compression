/**
 * @file LED.c
 * @author Dejana Smiljanic
 * @date 23.01.2026.
 * @brief Implementation of LED control functions for ADSP-21489 board.
 *
 * This file contains SRU configuration and GPIO-based control logic
 * for onboard LEDs. It supports initialization, enabling, disabling,
 * and basic LED control used for debugging and status indication.
 */

#include "LED.h"

/**
 * Configures SRU connections for all onboard LEDs.
 *
 * DPI LEDs (LED1–LED3) are controlled via FLAG outputs,
 * while DAI LEDs (LED4–LED8) are directly routed through SRU pins.
 */
void SruInit(void)
{
	/* Configuration for LED1, LED2, LED3 (DPI LEDs) */

	SRU(HIGH, DPI_PBEN06_I);   // LED1
	SRU(FLAG4_O, DPI_PB06_I);

	SRU(HIGH, DPI_PBEN13_I);   // LED2
	SRU(FLAG5_O, DPI_PB13_I);

	SRU(HIGH, DPI_PBEN14_I);   // LED3
	SRU(FLAG6_O, DPI_PB14_I);

	/* Configuration for LED4, LED5, LED6, LED7, LED8 (DAI LEDs) */

	SRU(HIGH, DAI_PBEN03_I);   // LED4
	SRU(HIGH, DAI_PB03_I);

	SRU(HIGH, DAI_PBEN04_I);   // LED5
	SRU(HIGH, DAI_PB04_I);

	SRU(HIGH, DAI_PBEN15_I);   // LED6
	SRU(HIGH, DAI_PB15_I);

    SRU(HIGH, DAI_PBEN16_I);   // LED7
	SRU(HIGH, DAI_PB16_I);

	SRU(HIGH, DAI_PBEN17_I);   // LED8
	SRU(HIGH, DAI_PB17_I);

	/* Set flag pins (for LED1-LED3) as outputs */
	sysreg_bit_set(sysreg_FLAGS, (FLG4O | FLG5O | FLG6O));

	/* Set HIGH on flag pins (initially turned on) */
	sysreg_bit_set(sysreg_FLAGS, (FLG4 | FLG5 | FLG6));
}

/**
 * Initializes LED subsystem.
 *
 * Initializes system components, configures SRU routing,
 * and ensures all LEDs are turned off after initialization.
 */
void LedInit(void)
{
	adi_initComponents();
	SruInit();

	/* Turn off LED1, LED2, LED3 via flag pins */
	sysreg_bit_clr(sysreg_FLAGS, FLG4);
	sysreg_bit_clr(sysreg_FLAGS, FLG5);
	sysreg_bit_clr(sysreg_FLAGS, FLG6);

	LedsOff();
}

/**
 * Turns on a single LED.
 *
 * @param led_index Index of the LED (0–7).
 */
void LedOn (uint8_t led_index)
{
    if (led_index <= 7)
    {
    	if(led_index == 0)
    	{
    		SRU(HIGH, DPI_PB06_I);
    	}
    	else if (led_index == 1)
    	{
    		SRU(HIGH, DPI_PB13_I);
    	}
    	else if (led_index == 2)
    	{
    		SRU(HIGH, DPI_PB14_I);
    	}
    	else if (led_index == 3)
    	{
    		SRU(HIGH, DAI_PB03_I);
    	}
    	else if (led_index == 4)
    	{
    		SRU(HIGH, DAI_PB04_I);
    	}
    	else if (led_index == 5)
    	{
    		SRU(HIGH, DAI_PB15_I);
    	}
    	else if (led_index == 6)
    	{
    		SRU(HIGH, DAI_PB16_I);
    	}
    	else
    	{
    		SRU(HIGH, DAI_PB17_I);
    	}
    }
}

/**
 * Turns off all DAI-controlled LEDs (LED4–LED8).
 */
void LedsOff(void)
{
	SRU(LOW, DAI_PB03_I);
	SRU(LOW, DAI_PB04_I);
	SRU(LOW, DAI_PB15_I);
	SRU(LOW, DAI_PB16_I);
	SRU(LOW, DAI_PB17_I);
}
