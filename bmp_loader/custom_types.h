/**
 * @file    custom_types.h
 * @author  Dejana Smiljanic
 * @date    02.01.2026
 *
 * @brief   Custom fixed-width integer type definitions.
 *
 * @details
 * This header defines fixed-width integer types.
 *
 * IMPORTANT:
 * If <stdint.h> is available, it is recommended to use the
 * standard types instead of these custom definitions.
 * CrossCore Embedded Studio 2.12.1 seems not to have support
 * for <stdint.h>.
 */

#ifndef CUSTOM_TYPES_H_
#define CUSTOM_TYPES_H_

/* ============================================================================
 * Type Definitions
 * ========================================================================== */

/** Signed 8-bit integer */
typedef signed char int8_t;

/** Unsigned 8-bit integer */
typedef unsigned char uint8_t;

/** Signed 16-bit integer */
typedef signed short int16_t;

/** Unsigned 16-bit integer */
typedef unsigned short uint16_t;

/** Signed 32-bit integer */
typedef signed int int32_t;

/** Unsigned 32-bit integer */
typedef unsigned int uint32_t;

#endif /* CUSTOM_TYPES_H_ */
