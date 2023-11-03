/* BEGIN Header */
/**
 ******************************************************************************
 * \file    basicMath.h
 * \author  Andrea Vivani
 * \brief   Implementation of useful math operations and constants
 ******************************************************************************
 * \copyright
 *
 * Copyright 2023 Andrea Vivani
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the “Software”), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 ******************************************************************************
 */
/* END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BASIC_MATH_H__
#define __BASIC_MATH_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Macros --------------------------------------------------------------------*/

// Absolute value
#define ABS(value)  (((value) >= 0) ? (value) : (-value))

// Constrain value between low and high
#define CONSTRAIN(value, low, high) ((value) < (low) ? (low) :((value) > (high) ? (high) : (value)))

// Re-map value according to new scale
#define MAP(x, fromLow, fromHigh, toLow, toHigh) (((x) - (fromLow)) * ((toHigh) - (toLow)) / ((fromHigh) - (fromLow)) + (fromLow))


/* Constants -----------------------------------------------------------------*/

// Pi value
#define PI              3.141592654f

// G value in m/s^2
#define G               9.80665f

// e value
#define E               2.71828182845904523536028747135266249f

// Conversion between milliDegrees-per-second and rad/s
#define MDPS_TO_RADPS   1.745329252e-5f

// Conversion between rad/s and milliDegrees-per-second
#define RADPS_TO_MDPS   57295.779513f

// Conversion between rad and deg
#define RAD_TO_DEG      57.29578f

// Conversion between deg and rad
#define DEG_TO_RAD      0.0174533f

// Conversion between milliG and m/s^2
#define MG_TO_MS2       0.00980665f

// Conversion between m/s^2 and milliG 
#define MS2_TO_MG		101.9716212978f

// Conversion between C and K
#define C_TO_K          273.15f

/* Functions ----------------------------------------------------------------*/

/*!
 * \brief Perform square root
 *
 * \param [in] value         input value
 *
 * \return square root of value
 */
float fastSqrt(float value);

/*!
 * \brief Perform fast inverse square root
 *
 * \param [in] value         input value
 *
 * \return inverse square root of value
 */
float fastInvSqrt(float value);

#ifdef __cplusplus
}
#endif

#endif // __BASIC_MATH_H__
