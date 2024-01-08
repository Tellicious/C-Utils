/* BEGIN Header */
/**
 ******************************************************************************
 * \file            IIRFilters.c
 * \author          Andrea Vivani
 * \brief           Implementation of simple discrete-time IIR filters
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

#include "IIRFilters.h"

/* Private  functions ---------------------------------------------------------*/

void IIRFilterInit(IIRFilterGeneric_t* filter, float n0, float n1, float n2, float n3, float d1, float d2, float d3) {
    /* Store filter coefficients */
    filter->n0 = n0;
    filter->n1 = n1;
    filter->n2 = n2;
    filter->n3 = n3;

    filter->d1 = d1;
    filter->d2 = d2;
    filter->d3 = d3;

    /* Initialize state variables */
    filter->i1 = filter->i2 = filter->i3 = filter->o1 = filter->o2 = filter->o3 = 0.0;
}

float IIRFilterProcess(IIRFilterGeneric_t* filter, float input) {
    /* Apply the IIR filter equation */
    float output;

    output = filter->n0 * input + filter->n1 * filter->i1 + filter->n2 * filter->i2 + filter->n3 * filter->i3
             - filter->d1 * filter->o1 - filter->d2 * filter->o2 - filter->d3 * filter->o3;

    /* Update state variables */
    filter->i3 = filter->i2;
    filter->i2 = filter->i1;
    filter->i1 = input;
    filter->o3 = filter->o2;
    filter->o2 = filter->o1;
    filter->o1 = output;

    return output;
}