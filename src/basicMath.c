/* BEGIN Header */
/**
 ******************************************************************************
 * \file    basicMath.c
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

/* Includes ------------------------------------------------------------------*/

#include "basicMath.h"

/* Private  functions ---------------------------------------------------------*/

// Fast square-root
float fastSqrt(float value)
{
    union {
        int i;
        float x;
    } u;
    u.x = value;
    u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
    
    // Two Babylonian Steps (simplified from:)
    // u.x = 0.5f * (u.x + x/u.x);
    // u.x = 0.5f * (u.x + x/u.x);
    u.x = u.x + value / u.x;
    u.x = 0.25f * u.x + value / u.x;
    
    return u.x;
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float fastInvSqrt(float value)
{
    union {
        float    f;
        uint32_t i;
    } conv = { .f = value };
    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - (value * 0.5f * conv.f * conv.f);
    return conv.f;
}
