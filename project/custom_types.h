/**
 * @file custom_types.h
 * @author Dejana Smiljanic
 * @date 02.01.2026.
 * @brief Manual definitions of fixed-width integer types.
 *
 * This header defines standard integer types to ensure consistent bit-width
 * across different compiler toolchains and hardware platforms.
 */

#ifndef CUSTOM_TYPES_H_
#define CUSTOM_TYPES_H_

/** @brief 8-bit signed integer type. Range: -128 to 127. */
typedef signed char     int8_t;

/** @brief 8-bit unsigned integer type. Range: 0 to 255. */
typedef unsigned char   uint8_t;

/** @brief 16-bit signed integer type. Range: -32768 to 32767. */
typedef signed short    int16_t;

/** @brief 16-bit unsigned integer type. Range: 0 to 65535. */
typedef unsigned short  uint16_t;

#endif /* CUSTOM_TYPES_H_ */
