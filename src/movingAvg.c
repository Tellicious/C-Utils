/* BEGIN Header */
/**
 ******************************************************************************
 * \file            movingAvg.c
 * \author          Andrea Vivani
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

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "movingAvg.h"

/* Private  functions ---------------------------------------------------------*/

movingAvgStatus_t movingAvgInit(movingAvg_t* movingAvg, MOVAVG_IND_TYPE size) {
    movingAvg->data = NULL;
    movingAvg->data = calloc(size, sizeof(MOVAVG_TYPE));
    /* movingAvg->data = malloc(size * sizeof(MOVAVG_TYPE)); */
    if (movingAvg->data == NULL) {
        movingAvg->size = 0;
        return MOVINGAVG_ERROR;
    }

    movingAvg->size = size;
    movingAvg->sum = 0;
    movingAvg->inv_size = (MOVAVG_TYPE)1.0 / size;
    movingAvg->_write = 0;
    return MOVINGAVG_SUCCESS;
}

MOVAVG_TYPE
movingAvgCalc(movingAvg_t* movingAvg, MOVAVG_TYPE value) {
    movingAvg->sum -= movingAvg->data[movingAvg->_write];
    movingAvg->sum += value;
    movingAvg->data[movingAvg->_write] = value;
    movingAvg->_write++;
    movingAvg->_write %= movingAvg->size;
    return (movingAvg->sum * movingAvg->inv_size);
}

movingAvgStatus_t movingAvgFlush(movingAvg_t* movingAvg) {
    if (movingAvg->data == NULL) {
        return MOVINGAVG_ERROR;
    }

    memset(movingAvg->data, 0x00, movingAvg->size * sizeof(MOVAVG_TYPE));

    movingAvg->sum = 0;
    movingAvg->_write = 0;

    return MOVINGAVG_SUCCESS;
}

movingAvgStatus_t movingAvgDelete(movingAvg_t* movingAvg) {

    if (movingAvg->data == NULL) {
        return MOVINGAVG_ERROR;
    }

    free(movingAvg->data);

    return MOVINGAVG_SUCCESS;
}