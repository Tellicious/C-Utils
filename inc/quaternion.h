/* BEGIN Header */
/**
 ******************************************************************************
 * @file    quaternion.h
 * @author  Andrea Vivani
 * @brief   Implementation of quaternion manipulation functions
 ******************************************************************************
 * @copyright
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
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/

#define AVOID_GIMBAL_LOCK

/* Typedefs ------------------------------------------------------------------*/

typedef struct {
    float q0, q1, q2, q3;
} quaternion_t;

typedef struct {
    float thx, thy, thz;
} eulerAngles_t;

/* Function prototypes -------------------------------------------------------*/

/*!
 * @brief Normalize quaternion (in place)
 *
 * @param[in] q        pointer to quaternion object
 */
void quaternionNorm(quaternion_t *q);

/*!
 * @brief Quaternion multiplication qo = qa * qb
 *
 * @param[in] qa       pointer to left-hand side quaternion object
 * @param[in] qb       pointer to right-hand side quaternion object
 * @param[out] qo     pointer to result quaternion object
 *
 * @attention qo can be different from qa/qb, or the same as qa/qb
 */
void quaternionMult(quaternion_t *qa, quaternion_t *qb, quaternion_t *qo);

/*!
 * @brief Quaternion rotation
 *
 * @param[in] qr       pointer to rotation quaternion object
 * @param[in] qv       pointer to quaternion object to rotate
 * @param[out] qo     pointer to result quaternion object qo = qr' * qv * qr
 */
void quaternionRotation(quaternion_t *qr, quaternion_t *qv, quaternion_t *qo);

/*!
 * @brief Quaternion conjugate
 *
 * @param[in] qa       pointer to left-hand side quaternion object
 * @param[out] qo     pointer to result quaternion object
 */
void quaternionConj(quaternion_t *qa, quaternion_t *qo);

/*!
 * @brief Convert quaternion to Euler angles
 *
 * @param[in] qr       pointer to input quaternion object
 * @param[out] ea     pointer to result euler angles
 */
void quaternionToEuler(quaternion_t *qr, eulerAngles_t *ea);

#ifdef __cplusplus
}
#endif

#endif // __QUATERNION_H__
