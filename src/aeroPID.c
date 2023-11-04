/* BEGIN Header */
/**
 ******************************************************************************
 * \file    aeroPID.c
 * \author  Andrea Vivani
 * \brief   Implementation of PID controller with integral clamping specific
 *          for aero applications
 ******************************************************************************
 * \copyright
 *
 * Copyright 2022 Andrea Vivani
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

#include "aeroPID.h"
#include "basicMath.h"

/* Functions -----------------------------------------------------------------*/

void aeroPID_init(aeroPID_t *PID, float kp_val, float ki_val, float kd_val, float nd_val, float dT, float sat_min, float sat_max)
{
  PID->dT = dT;
  PID->kp = kp_val;
  PID->ki = 0.5 * ki_val * dT;
  PID->nd = nd_val;
  PID->kd = (2 * kd_val * nd_val) / (2 + nd_val * dT);
  PID->kf = (2 - nd_val * dT) / (2 + nd_val * dT);
  aeroPID_setIntegralSaturation(PID, sat_min, sat_max);
  PID->eOld = 0;
  PID->DuD = 0;
  PID->DuI = 0;
}

uint8_t aeroPID_calc(aeroPID_t *PID, float set_point, float measure)
{
  float e;
  e = set_point - measure;
  PID->DuI += PID->ki * (e + PID->eOld);
  PID->DuI = CONSTRAIN(PID->DuI, PID->intMin, PID->intMax);
  PID->DuD = PID->kf * PID->DuD + PID->kd * (e - PID->eOld);
  PID->output = PID->kp * e + PID->DuI + PID->DuD;
  PID->eOld = e;
  if ((PID->DuI == PID->intMin) || (PID->DuI == PID->intMax)) {
    return 1;
  }
  return 0;
}
