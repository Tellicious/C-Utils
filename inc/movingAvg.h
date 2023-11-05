/* BEGIN Header */
/**
 ******************************************************************************
 * \file    movingAvg.h
 * \author  Andrea Vivani
 * \brief   Implementation of moving average
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
#ifndef __MOVINGAVG_H__
#define __MOVINGAVG_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Macros --------------------------------------------------------------------*/

#define MOVAVG_TYPE float
#define MOVAVG_IND_TYPE uint16_t

/* Typedefs ------------------------------------------------------------------*/

/*!
 * Moving average struct
 */
typedef struct
{
    MOVAVG_TYPE *data;
    MOVAVG_TYPE sum, inv_size;
    MOVAVG_IND_TYPE size, _write;
} movingAvg_t;

/*!
 * Moving Average return status
 */
typedef enum
{
    MOVINGAVG_SUCCESS = 00,
    MOVINGAVG_ERROR = 01
} movingAvgStatus_t;

/* Function prototypes --------------------------------------------------------*/

/*!
 * \brief Init moving average structure
 *
 * \param [in] movingAvg    pointer to moving average object
 * \param [in] size         required queue size
 */
movingAvgStatus_t movingAvgInit (movingAvg_t *movingAvg, MOVAVG_IND_TYPE size);

/*!
 * \brief Calculate moving average 
 *
 * \param [in] movingAvg    pointer to moving average object
 * \param [in] value        input value
 * 
 * \return updated moving average
 */
MOVAVG_TYPE movingAvgCalc(movingAvg_t *movingAvg, MOVAVG_TYPE value);

/*!
 * \brief Get latest moving average 
 *
 * \param [in] movingAvg    pointer to moving average object
 * 
 * \return latest moving average
 */
static inline MOVAVG_TYPE movingAvgGetLatest(movingAvg_t *movingAvg) {return (movingAvg->sum * movingAvg->inv_size);}

/*!
 * \brief Flush queue setting all values to 0
 *
 * \param [in] movingAvg    pointer to moving average object
 */
movingAvgStatus_t movingAvgFlush(movingAvg_t *movingAvg);

/*!
 * \brief Delete moving average
 *
 * \param [in] movingAvg    pointer to moving average object
 */
movingAvgStatus_t movingAvgDelete(movingAvg_t *movingAvg);

#ifdef __cplusplus
}
#endif

#endif // __QUEUE_H__