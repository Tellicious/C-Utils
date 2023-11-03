/* BEGIN Header */
/**
 ******************************************************************************
 * \file    numMethods.h
 * \author  Andrea Vivani
 * \brief   Implementation of several numerical methods
 ******************************************************************************
 * \copyright
 *
 * Copyright 2016 Andrea Vivani
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
#ifndef __NUM_METHODS_H__
#define __NUM_METHODS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "matrix.h"

/* Function prototypes -------------------------------------------------------*/

/*!
 * \brief Solve an Ax = B system with forward substitution
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 *
 * \attention assumes that the matrix A is already a lower triangular one. No check is performed within function!
 */
void fwsub(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Solve an Ax = PB system with forward substitution (with permutation)
 *
 * \param [in] A                     pointer to A matrix object
 * \param [in] B                     pointer to B matrix object
 * \param [in] P                     pointer to P matrix object
 * \param [out] result        pointer to result matrix object
 *
 * \attention assumes that the matrix A is already a lower triangular one. No check is performed within function!
 */
void fwsubPerm(matrix_t A, matrix_t B, matrix_t P, matrix_t result);

/*!
 * \brief Solve AX = B system with backward substitution
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 *
 * \attention assumes that the matrix A is already an upper triangular one. No check is performed within function!
 */
void bksub(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Solve AX = PB system with backward substitution (with permutation)
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [in] P                      pointer to P matrix object
 * \param [out] result         pointer to result matrix object
 *
 * \attention assumes that the matrix A is already an upper triangular one. No check is performed within function!
 */
void bksubPerm(matrix_t A, matrix_t B, matrix_t P, matrix_t result);

/*!
 * \brief Calculate L (lower triangular) and U (upper triangular) matrices such that A = LU with Crout's Method
 *
 * \param [in] A                      pointer to A matrix object
 * \param [out] L                    pointer to L matrix object
 * \param [out] U                    pointer to U matrix object
 *
 * \return 0 if success, 1 if errors
 */
uint8_t LU_Crout(matrix_t A, matrix_t L, matrix_t U);

/*!
 * \brief Calculate L (lower triangular) and U (upper triangular) matrices such that A = LU with Cormen's Method
 *
 * \param [in] A                      pointer to A matrix object
 * \param [out] L                    pointer to L matrix object
 * \param [out] U                    pointer to U matrix object
 *
 * \return 0 if success, 1 if errors
 */
uint8_t LU_Cormen(matrix_t A, matrix_t L, matrix_t U);

/*!
 * \brief Calculate L (lower triangular), U (upper triangular) and P (permutation) matrices such that A = LUP with Cormen's Method
 *
 * \param [in] A                      pointer to A matrix object
 * \param [out] L                    pointer to L matrix object
 * \param [out] U                    pointer to U matrix object
 * \param [in] P                      pointer to P matrix object
 *
 * \return factor to be multiplied by determinant of U to obtain determinant of A
 */
int8_t LUP_Cormen(matrix_t A, matrix_t L, matrix_t U, matrix_t P);

/*!
 * \brief Solve AX = B system using LU factorization
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 */
void LinSolveLU(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Solve AX = B system using LUP factorization
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 */
void LinSolveLUP(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Solve AX = B system using Gauss elimination with partial pivoting
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 */
void LinSolveGauss(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Calculate sort of quadratic form result=A*B*(~A)
 *
 * \param [in] A                      pointer to A matrix object
 * \param [in] B                      pointer to B matrix object
 * \param [out] result         pointer to result matrix object
 */
void QuadProd(matrix_t A, matrix_t B, matrix_t result);

/*!
 * \brief Gauss-Newton sensor calibration with 9 parameters
 * \attention Approximates Data to a sphere of radius k by calculating 6 gains (s) and 3 biases (b), useful to calibrate some sensors (meas_sphere=S*(meas-B) with S symmetric)
 * \attention Data has n>=9 rows corresponding to the number of measures and 3 columns corresponding to the 3 axis
 * \attention b1=out(0,0);
 * b2=out(1,0);
 * b3=out(2,0);
 * s11=out(3,0);
 * s12=out(4,0);
 * s13=out(5,0);
 * s22=out(6,0);
 * s23=out(7,0);
 * s33=out(8,0);
 *
 * \param [in] Data                 pointer to raw data matrix object Data
 * \param [in] k                        radius of sphere to be approximated
 * \param [in] X0                      pointer to starting vector X0 (usually [0 0 0 1 0 0 1 0 1])
 * \param [in] nmax                 maximum number of iterations (200 is generally fine, even if it usually converges within 10 iterations)
 * \param [in] tol                   stopping tolerance (1e-6 is generally fine)
 * \param [out] result          pointer to result matrix object S
 *
 * \return 0 if success, 1 if errors
 */
uint8_t GaussNewton_Sens_Cal_9(matrix_t Data, float k, matrix_t X0, uint16_t nmax, float tol, matrix_t result);

/*!
 * \brief Gauss-Newton sensor calibration with 6 parameters
 * \attention Approximates Data to a sphere of radius k by calculating 3 gains (s) and 3 biases (b), useful to calibrate some sensors (meas_sphere=S*(meas-B) with S symmetric)
 * \attention Data has n>=6 rows corresponding to the number of measures and 3 columns corresponding to the 3 axis
 * \attention b1=out(0,0);
 * b2=out(1,0);
 * b3=out(2,0);
 * s11=out(3,0);
 * s22=out(4,0);
 * s33=out(5,0);
 *
 * \param [in] Data                 pointer to raw data matrix object Data
 * \param [in] k                        radius of sphere to be approximated
 * \param [in] X0                      pointer to starting vector X0 (usually [0 0 0 1 1 1])
 * \param [in] nmax                 maximum number of iterations (200 is generally fine, even if it usually converges within 10 iterations)
 * \param [in] tol                   stopping tolerance (1e-6 is generally fine)
 * \param [out] result          pointer to result matrix object S
 *
 * \return 0 if success, 1 if errors
 */
uint8_t GaussNewton_Sens_Cal_6(matrix_t Data, float k, matrix_t X0, uint16_t nmax, float tol, matrix_t result);

#ifdef __cplusplus
}
#endif

#endif // __NUM_METHODS_H__
