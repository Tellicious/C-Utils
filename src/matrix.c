/* BEGIN Header */
/**
 ******************************************************************************
 * \file    matrix.c
 * \author  Andrea Vivani
 * \brief   Implementation of matrix manipulation functions
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

/* Includes ------------------------------------------------------------------*/

#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include "numMethods.h"

//==========================================Assignment=============================================//

//-----------------------Constructor with external data-----------------------//
matrix_t matrixInit(uint8_t rows, uint8_t cols, float *data)
{
    matrix_t matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = data;
    return matrix;
}

//---------------------Identity Matrix----------------------//
void matrixIdentity(matrix_t matrix)
{
    uint16_t ii;
    matrixZeros(matrix);
    for (ii = 0; ii < ((matrix.cols < matrix.rows)? matrix.cols : matrix.rows); ii++)
        ELEM(matrix, ii, ii) = 1.0f;
    return;
}

//==========================================Operations=============================================//
//--------------------matrix_t addition----------------------//
void matrixAdd(const matrix_t lhs, const matrix_t rhs, matrix_t result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs.cols * lhs.rows); ii++) {
        result.data[ii] = lhs.data[ii] + rhs.data[ii];
    }
    return;
}

//--------------------Scalar addition----------------------//
void matrixAddScalar(const matrix_t lhs, const float sc, matrix_t result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs.cols * lhs.rows); ii++) {
        result.data[ii] = lhs.data[ii] + sc;
    }
    return;
}

//------------------matrix_t subtraction--------------------//
void matrixSub(const matrix_t lhs, const matrix_t rhs, matrix_t result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs.cols * lhs.rows); ii++) {
        result.data[ii] = lhs.data[ii] - rhs.data[ii];
    }
    return;
}

//---------------matrix_t multiplication------------------//
uint8_t matrixMult(const matrix_t lhs, const matrix_t rhs, matrix_t result)
{
    //	uint8_t i, j, k;
    if (lhs.cols != rhs.rows) {
        return 1;
    }
    //	matrixZeros(result);
    //	for (i = 0; i < lhs.rows; i++)
    //		for (j = 0; j < rhs.cols; j++)
    //			for (k = 0; k < lhs.cols; k++)
    //				ELEM(result, i, j) += ELEM(lhs, i, k) * ELEM(rhs, k, j);
    float *pIn1 = lhs.data;                /* input data matrix pointer A */
    float *pIn2 = rhs.data;                /* input data matrix pointer B */
    float *pInA = lhs.data;                /* input data matrix pointer A  */
    float *pOut = result.data;                 /* output data matrix pointer */
    float *px;                                 /* Temporary output data matrix pointer */
    float sum;                                 /* Accumulator */
    
    /* Run the below code for Cortex-M4 and Cortex-M3 */
    
    float in1, in2, in3, in4;
    uint16_t col, i = 0u, j, row = lhs.rows, colCnt;      /* loop counters */
    
    {
        /* The following loop performs the dot-product of each row in lhs with each column in rhs */
        /* row loop */
        do
        {
            /* Output pointer is set to starting address of the row being processed */
            px = pOut + i;
            
            /* For every row wise process, the column loop counter is to be initiated */
            col = rhs.cols;
            
            /* For every row wise process, the pIn2 pointer is set
             ** to the starting address of the rhs data */
            pIn2 = rhs.data;
            
            j = 0u;
            
            /* column loop */
            do
            {
                /* Set the variable sum, that acts as accumulator, to zero */
                sum = 0.0f;
                
                /* Initiate the pointer pIn1 to point to the starting address of the column being processed */
                pIn1 = pInA;
                
                /* Apply loop unrolling and compute 4 MACs simultaneously. */
                colCnt = lhs.cols >> 2u;
                
                /* matrix multiplication        */
                while(colCnt > 0u)
                {
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                    in3 = *pIn2;
                    pIn2 += rhs.cols;
                    in1 = pIn1[0];
                    in2 = pIn1[1];
                    sum += in1 * in3;
                    in4 = *pIn2;
                    pIn2 += rhs.cols;
                    sum += in2 * in4;
                    
                    in3 = *pIn2;
                    pIn2 += rhs.cols;
                    in1 = pIn1[2];
                    in2 = pIn1[3];
                    sum += in1 * in3;
                    in4 = *pIn2;
                    pIn2 += rhs.cols;
                    sum += in2 * in4;
                    pIn1 += 4u;
                    
                    /* Decrement the loop count */
                    colCnt--;
                }
                
                /* If the columns of lhs is not a multiple of 4, compute any remaining MACs here.
                 ** No loop unrolling is used. */
                colCnt = lhs.cols % 0x4u;
                
                while(colCnt > 0u)
                {
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                    sum += *pIn1++ * (*pIn2);
                    pIn2 += rhs.cols;
                    
                    /* Decrement the loop counter */
                    colCnt--;
                }
                
                /* Store the result in the destination buffer */
                *px++ = sum;
                
                /* Update the pointer pIn2 to point to the  starting address of the next column */
                j++;
                pIn2 = rhs.data + j;
                
                /* Decrement the column loop counter */
                col--;
                
            } while(col > 0u);
            /* Update the pointer pInA to point to the  starting address of the next row */
            i = i + rhs.cols;
            pInA = pInA + lhs.cols;
            
            /* Decrement the row loop counter */
            row--;
            
        } while(row > 0u);
    }
    return 0;
    
}

//------matrix_t multiplication with lhs transposed------//
uint8_t matrixMult_lhsT(const matrix_t lhs, const matrix_t rhs, matrix_t result)
{
    uint8_t i, j, k;
    if (lhs.rows != rhs.rows) {
        return 1;
    }
    matrixZeros(result);
    for (i = 0; i < lhs.cols; i++)
        for (j = 0; j < rhs.cols; j++)
            for (k = 0; k < lhs.rows; k++)
                ELEM(result, i, j) += ELEM(lhs, k, i) * ELEM(rhs, k, j);
    return 0;
}

//------matrix_t multiplication with rhs transposed------//
uint8_t matrixMult_rhsT(const matrix_t lhs, const matrix_t rhs, matrix_t result)
{
    uint8_t i, j, k;
    if (lhs.cols != rhs.cols) {
        return 1;
    }
    matrixZeros(result);
    for (i = 0; i < lhs.rows; i++)
        for (j = 0; j < rhs.rows; j++)
            for (k = 0; k < lhs.cols; k++)
                ELEM(result, i, j) += ELEM(lhs, i, k) * ELEM(rhs, j, k);
    return 0;
}

//---------------Scalar multiplication------------------//
void matrixMultScalar(const matrix_t lhs, const float sc, matrix_t result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs.cols * lhs.rows); ii++) {
        result.data[ii] = lhs.data[ii] * sc;
    }
    return;
}

//--------------------Inverse LU------------------------//
void matrixInversed(const matrix_t lhs, matrix_t result)
{
    float _eye_data[lhs.rows * lhs.cols];
    matrix_t Eye = matrixInit(lhs.rows, lhs.cols, _eye_data);
    matrixIdentity(Eye);
    LinSolveLU(lhs, Eye, result);
    return;
}

//-----------------Robust Inverse LUP-------------------//
void matrixInversed_rob(const matrix_t lhs, matrix_t result)
{
    float _eye_data[lhs.rows * lhs.cols];
    matrix_t Eye = matrixInit(lhs.rows, lhs.cols, _eye_data);
    matrixIdentity(Eye);
    LinSolveLUP(lhs, Eye, result);
    return;
}

//-----------------Transposed--------------------//
void matrixTrans(const matrix_t lhs, matrix_t result)
{
    uint8_t ii, jj;
    for (ii = 0; ii < lhs.rows; ii++)
        for (jj = 0; jj < lhs.cols; jj++)
            ELEM(result, jj, ii) = ELEM(lhs, ii, jj);
    return;
}

//-----------------Nomalized--------------------//
void matrixNormalized(const matrix_t lhs, matrix_t result)
{
    float k = 1.0f / matrixNorm(lhs);
    matrixMultScalar(lhs, k, result);
    return;
}

//-------Moore-Penrose pseudo inverse---------//
void matrixPseudo_inv(const matrix_t lhs, matrix_t result)
{
    float _tran_data[lhs.cols * lhs.rows];
    float _mult1_data[lhs.cols * lhs.cols];
    matrix_t tran = matrixInit(lhs.cols, lhs.rows, _tran_data);
    matrix_t mult1 = matrixInit(lhs.cols, lhs.cols, _mult1_data);
    matrixTrans(lhs, tran);
    matrixMult(tran, lhs, mult1);
    LinSolveLU(mult1, tran, result);
    return;
}

//=======================================matrix_t Data=========================================//

//-----------Returns the determinant----------//
float matrixDet(const matrix_t matrix)
{
    float _L_data[matrix.rows * matrix.rows];
    float _U_data[matrix.rows * matrix.rows];
    float _P_data[matrix.rows];
    matrix_t L = matrixInit(matrix.rows, matrix.rows, _L_data);
    matrix_t U = matrixInit(matrix.rows, matrix.rows, _U_data);
    matrix_t P = matrixInit(matrix.rows, 1, _P_data);
    int16_t ii;
    int8_t det_f;
    float determinant = 1.0f;
    
    if (matrix.rows != matrix.cols) {
        return 0.0f;
    }
    
    if (LU_Cormen(matrix, L, U)) {
        for (ii = 0; ii < matrix.rows; ii++) {
            determinant *= ELEM(U, ii, ii);
        }
    }
    
    else {
        det_f = LUP_Cormen(matrix, L, U, P);
        if (det_f) {
            for (ii = 0; ii < matrix.rows; ii++) {
                determinant *= ELEM(U, ii, ii);
            }
            determinant *= det_f;
        }
        else {
            determinant = 0.0f;
        }
    }
    
    return determinant;
}

//-------------Returns the norm--------------//
float matrixNorm(const matrix_t matrix)
{
    float result = 0.0f;
    uint16_t i;
    for (i = 0; i < (matrix.rows * matrix.cols); i++) {
        result += matrix.data[i] * matrix.data[i];
    }
    result = sqrtf(result);
    return result;
}
