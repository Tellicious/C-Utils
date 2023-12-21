/* BEGIN Header */
/**
 ******************************************************************************
 * \file            PID.h
 * \author          Andrea Vivani
 * \brief           Implementation of PID controller with several anti-windup
 *                  options
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
#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stdint.h"

/* Typedefs ------------------------------------------------------------------*/

typedef struct {
    float output; /* output value */
    float kp;     /* proportional gain */
    float ki;     /* integral gain */
    float kd;     /* derivative gain */
    float nd;     /* derivative filter constant N: derivative in Laplace=s/(1+s/N) */
    float dT;     /* loop time in s */
    float kb;     /* back-calculation coefficient */
    float kf;     /* derivative filter constant */
    float satMin; /* lower integral saturation limit */
    float satMax; /* upper integral saturation limit */
    float oldE;   /* previous step error */
    float tmp;    /* previous step error (used for integral clamping) or back-calculation coefficient */
    float DuD;    /* derivative action contribution */
    float DuI;    /* integral action contribution */
} PID_t;

/* Function prototypes -------------------------------------------------------*/

/**
 * \brief           PID initialization
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       kp_val: proportional gain
 * \param[in]       ki_val: integral gain
 * \param[in]       kd_val: derivative gain
 * \param[in]       nd_val: derivative filter constant N: derivative in Laplace=s/(1+s/N)
 * \param[in]       kb_val: back-calculation coefficient value. Set to 0 if PID_calcBackCalc() is not used
 * \param[in]       dT_ms: loop time in ms
 * \param[in]       sat_min: lower saturation limit
 * \param[in]       sat_max: upper saturation limit
 */
void PID_init(PID_t* PID, float kp_val, float ki_val, float kd_val, float nd_val, float kb_val, float dT_ms,
              float sat_min, float sat_max);

/**
 * \brief           PID calculation without anti-windup (with output clamping)
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       set_point: set point value
 * \param[in]       measure: measure value
 * 
 * \attention       to be called regularly every dT ms
 */
void PID_calc(PID_t* PID, float set_point, float measure);

/**
 * \brief           PID calculation with aero-specific anti-windup, clamping only integral term
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       set_point: set point value
 * \param[in]       measure: measure value
 * 
 * \attention       to be called regularly every dT ms
 * 
 * \return          0 if success, 1 if integral term is saturated
 */
uint8_t PID_calcAeroClamp(PID_t* PID, float set_point, float measure);

/**
 * \brief           PID calculation with integral-clamping anti-windup, clamping entire output
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       set_point: set point value
 * \param[in]       measure: measure value
 * 
 * \attention       to be called regularly every dT ms
 * 
 * \return          0 if success, 1 if integral term is saturated
 */
uint8_t PID_calcIntegralClamp(PID_t* PID, float set_point, float measure);

/**
 * \brief           PID calculation with back-calculation anti-windup, clamping entire output
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       set_point: set point value
 * \param[in]       measure: measure value
 * 
 * \attention       to be called regularly every dT ms
 * 
 * \return          0 if success, 1 if integral term is saturated
 */
uint8_t PID_calcBackCalc(PID_t* PID, float set_point, float measure);

/**
 * \brief           Return PID output value
 *
 * \param[in]       PID: pointer to PID object
 * 
 * \return          output value
 */
static inline float PID_getOutput(PID_t* PID) { return PID->output; };

/**
 * \brief           Return Kp value
 *
 * \param[in]       PID: pointer to PID object
 * 
 * \return          Kp value
 */
static inline float PID_getKp(PID_t* PID) { return PID->kp; };

/**
 * \brief           Return Ki value
 *
 * \param[in]       PID: pointer to PID object
 * 
 * \return          Ki value
 */
static inline float PID_getKi(PID_t* PID) { return (2 * PID->ki / PID->dT); };

/**
 * \brief           Return Kd value
 *
 * \param[in]       PID: pointer to PID object
 * 
 * \return          Kd value
 */
static inline float PID_getKd(PID_t* PID) { return (0.5 * (2 + PID->nd * PID->dT) * PID->kd / PID->nd); };

/**
 * \brief           Set proportional gain value
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       kp_val: proportional gain value
 */
static inline void PID_setKp(PID_t* PID, float kp_val) { PID->kp = kp_val; };

/**
 * \brief           Set integral gain value
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       ki_val: integral gain value
 */
static inline void PID_setKi(PID_t* PID, float ki_val) { PID->ki = 0.5 * ki_val * PID->dT; };

/**
 * \brief           Set derivative gain value
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       kd_val: derivative gain value
 * \param[in]       nd_val: derivative filter constant N: derivative in Laplace=s/(1+s/N)
 */
static inline void PID_setKd(PID_t* PID, float kd_val, float nd_val) {
    PID->nd = nd_val;
    PID->kd = (2 * kd_val * nd_val) / (2 + nd_val * PID->dT);
    PID->kf = (2 - nd_val * PID->dT) / (2 + nd_val * PID->dT);
};

/**
 * \brief           Set back-calculation coefficient
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       kb_val: back-calculation coefficient value
 */
static inline void PID_setKb(PID_t* PID, float kb_val) { PID->kb = 0.5 * kb_val * PID->dT; };

/**
 * \brief           Set integral component value
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       value: value of integral component
 */
static inline void PID_setIntegralValue(PID_t* PID, float value) { PID->DuI = value; };

/**
 * \brief           Set output saturation values
 *
 * \param[in]       PID: pointer to PID object
 * \param[in]       sat_min: lower saturation limit
 * \param[in]       sat_max: upper saturation limit
 */
static inline void PID_setIntegralSaturation(PID_t* PID, float sat_min, float sat_max) {
    PID->satMin = sat_min;
    PID->satMax = sat_max;
};

/**
 * \brief           Reset PID
 *
 * \param[in]       PID: pointer to PID object
 */
static inline void PID_reset(PID_t* PID) {
    PID->oldE = 0;
    PID->DuD = 0;
    PID->DuI = 0;
    PID->tmp = 0;
};

#ifdef __cplusplus
}
#endif

#endif /* __PID_H__ */
