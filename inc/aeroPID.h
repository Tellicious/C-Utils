/* BEGIN Header */
/**
 ******************************************************************************
 * \file    aeroPID.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AEROPID_H__
#define __AEROPID_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include "stdint.h"

/* Typedefs ------------------------------------------------------------------*/

typedef struct {
  float output;	//output value
  float kp;	//proportional gain
  float ki;	//integral gain
  float kd;	//derivative gain
  float nd;	//derivative filter constant N: derivative in Laplace=s/(1+s/N)
  float dT;	//loop time in ms
  float kf;	//derivative filter constant
  float intMin;	//lower integral saturation limit
  float intMax;	//upper integral saturation limit
  float eOld;	//previous step error
  float DuD;	//derivative action contribution
  float DuI;	//integral action contribution
} aeroPID_t;

/* Function prototypes -------------------------------------------------------*/

/*!
 * \brief PID initialization
 *
 * \param [in] PID        pointer to PID object
 * \param [in] kp_val     proportional gain
 * \param [in] ki_val     integral gain
 * \param [in] kd_val     derivative gain
 * \param [in] nd_val     derivative filter constant N: derivative in Laplace=s/(1+s/N)
 * \param [in] dT         loop time in ms
 * \param [in] sat_min    lower integral saturation limit
 * \param [in] sat_max    upper integral saturation limit
 */
void aeroPID_init(aeroPID_t *PID, float kp_val, float ki_val, float kd_val, float nd_val, float dT, float sat_min, float sat_max);

/*!
 * \brief PID calculation, to be called regularly every dT ms
 *
 * \param [in] PID        pointer to PID object
 * \param [in] set_point  set point value
 * \param [in] measure    measure value
 * 
 * \return 0 if success, 1 if integral term is saturated
 */
uint8_t aeroPID_calc(aeroPID_t *PID, float set_point, float measure);

/*!
 * \brief Return PID output value
 *
 * \param [in] PID        pointer to PID object
 * 
 * \return output value
 */
inline float aeroPID_getOutput(aeroPID_t *PID) {return PID->output;};

/*!
 * \brief Return Kp value
 *
 * \param [in] PID        pointer to PID object
 * 
 * \return Kp value
 */
inline float aeroPID_getKp(aeroPID_t *PID) {return PID->kp;};

/*!
 * \brief Return Ki value
 *
 * \param [in] PID        pointer to PID object
 * 
 * \return Ki value
 */
inline float aeroPID_getKi(aeroPID_t *PID) {return (2 * PID->ki / PID->dT);};

/*!
 * \brief Return Kd value
 *
 * \param [in] PID        pointer to PID object
 * 
 * \return Kd value
 */
inline float aeroPID_getKd(aeroPID_t *PID) {return (0.5 * (2 + PID->nd * PID->dT) * PID->kd / PID->nd);};

/*!
 * \brief Set proportional gain value
 *
 * \param [in] PID        pointer to PID object
 * \param [in] kp_val     proportional gain value
 */
inline void aeroPID_setKp(aeroPID_t *PID, float kp_val) {PID->kp = kp_val;};

/*!
 * \brief Set integral gain value
 *
 * \param [in] PID        pointer to PID object
 * \param [in] ki_val     integral gain value
 */
inline void aeroPID_setKi(aeroPID_t *PID, float ki_val) {PID->ki = 0.5 * ki_val * PID->dT;};

/*!
 * \brief Set derivative gain value
 *
 * \param [in] PID        pointer to PID object
 * \param [in] kd_val     derivative gain value
 * \param [in] nd_val     derivative filter constant N: derivative in Laplace=s/(1+s/N)
 */
inline void aeroPID_setKd(aeroPID_t *PID, float kd_val, float nd_val) {PID->nd = nd_val; PID->kd = (2 * kd_val * nd_val) / (2 + nd_val * PID->dT); PID->kf = (2 - nd_val * PID->dT) / (2 + nd_val * PID->dT);};

/*!
 * \brief Set integral component value
 *
 * \param [in] PID        pointer to PID object
 * \param [in] value      value of integral component
 */
inline void aeroPID_setIntegralValue(aeroPID_t *PID, float value) {PID->DuI = value;};

/*!
 * \brief Set integral saturation values
 *
 * \param [in] PID        pointer to PID object
 * \param [in] sat_min    lower integral saturation limit
 * \param [in] sat_max    upper integral saturation limit
 */
inline void aeroPID_setIntegralSaturation(aeroPID_t *PID, float sat_min, float sat_max) {PID->intMin = sat_min; PID->intMax = sat_max;};

/*!
 * \brief Reset PID
 *
 * \param [in] PID        pointer to PID object
 */
inline void aeroPID_reset(aeroPID_t *PID) {PID->eOld = 0; PID->DuD = 0; PID->DuI = 0;};

#ifdef __cplusplus
}
#endif

#endif // __AEROPID_H__

