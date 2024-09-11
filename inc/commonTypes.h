/* BEGIN Header */
/**
 ******************************************************************************
 * \file            commonTypes.h
 * \author          Andrea Vivani
 * \brief           Implementation of common type definitions
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
#ifndef __COMMONTYPES_H__
#define __COMMONTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Typedefs ------------------------------------------------------------------*/

/*
* Generic return status
*/
typedef enum {
    UTILS_STATUS_SUCCESS = 0,
    UTILS_STATUS_ERROR,
    UTILS_STATUS_WARNING,
    UTILS_STATUS_ONGOING,
    UTILS_STATUS_TIMEOUT,
    UTILS_STATUS_EMPTY,
    UTILS_STATUS_FULL,
    UTILS_STATUS_BUCKET_EMPTY,
} utilsStatus_t;

/*
* 3-axis float vector
*/
typedef struct {
    float x, y, z;
} axis3f_t;

#ifdef __cplusplus
}
#endif

#endif /* __COMMONTYPES_H__ */
