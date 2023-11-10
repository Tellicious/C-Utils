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

//-----------------------Constructor-----------------------//
matrixStatus_t matrixInit(matrix_t *matrix, uint8_t rows, uint8_t cols)
{
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = calloc(rows * cols, sizeof(float));

    if (matrix->data == NULL)
    {
        return MATRIX_ERROR;
    }
    
    return MATRIX_SUCCESS;

}

//---------------------Identity Matrix----------------------//
void matrixIdentity(matrix_t *matrix)
{
    uint16_t ii;
    matrixZeros(matrix);
    for (ii = 0; ii < ((matrix->cols < matrix->rows)? matrix->cols : matrix->rows); ii++)
        ELEMP(matrix, ii, ii) = 1.0f;
    return;
}

//==========================================Operations=============================================//
//--------------------matrix_t addition----------------------//
void matrixAdd(matrix_t *lhs, matrix_t *rhs, matrix_t *result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs->cols * lhs->rows); ii++) {
        result->data[ii] = lhs->data[ii] + rhs->data[ii];
    }
    return;
}

//--------------------Scalar addition----------------------//
void matrixAddScalar(matrix_t *lhs, float sc, matrix_t *result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs->cols * lhs->rows); ii++) {
        result->data[ii] = lhs->data[ii] + sc;
    }
    return;
}

//------------------matrix_t subtraction--------------------//
void matrixSub(matrix_t *lhs, matrix_t *rhs, matrix_t *result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs->cols * lhs->rows); ii++) {
        result->data[ii] = lhs->data[ii] - rhs->data[ii];
    }
    return;
}

//---------------matrix_t multiplication------------------//
matrixStatus_t matrixMult(matrix_t *lhs, matrix_t *rhs, matrix_t *result)
{
    //	uint8_t i, j, k;
    if (lhs->cols != rhs->rows) {
        return MATRIX_ERROR;
    }
    //	matrixZeros(result);
    //	for (i = 0; i < lhs->rows; i++)
    //		for (j = 0; j < rhs->cols; j++)
    //			for (k = 0; k < lhs->cols; k++)
    //				ELEMP(result, i, j) += ELEMP(lhs, i, k) * ELEMP(rhs, k, j);
    float *pIn1 = lhs->data;                /* input data matrix pointer A */
    float *pIn2 = rhs->data;                /* input data matrix pointer B */
    float *pInA = lhs->data;                /* input data matrix pointer A  */
    float *pOut = result->data;                 /* output data matrix pointer */
    float *px;                                 /* Temporary output data matrix pointer */
    float sum;                                 /* Accumulator */
    
    /* Run the below code for Cortex-M4 and Cortex-M3 */
    
    float in1, in2, in3, in4;
    uint16_t col, i = 0u, j, row = lhs->rows, colCnt;      /* loop counters */
    
    {
        /* The following loop performs the dot-product of each row in lhs with each column in rhs */
        /* row loop */
        do
        {
            /* Output pointer is set to starting address of the row being processed */
            px = pOut + i;
            
            /* For every row wise process, the column loop counter is to be initiated */
            col = rhs->cols;
            
            /* For every row wise process, the pIn2 pointer is set
             ** to the starting address of the rhs data */
            pIn2 = rhs->data;
            
            j = 0u;
            
            /* column loop */
            do
            {
                /* Set the variable sum, that acts as accumulator, to zero */
                sum = 0.0f;
                
                /* Initiate the pointer pIn1 to point to the starting address of the column being processed */
                pIn1 = pInA;
                
                /* Apply loop unrolling and compute 4 MACs simultaneously. */
                colCnt = lhs->cols >> 2u;
                
                /* matrix multiplication        */
                while(colCnt > 0u)
                {
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                    in3 = *pIn2;
                    pIn2 += rhs->cols;
                    in1 = pIn1[0];
                    in2 = pIn1[1];
                    sum += in1 * in3;
                    in4 = *pIn2;
                    pIn2 += rhs->cols;
                    sum += in2 * in4;
                    
                    in3 = *pIn2;
                    pIn2 += rhs->cols;
                    in1 = pIn1[2];
                    in2 = pIn1[3];
                    sum += in1 * in3;
                    in4 = *pIn2;
                    pIn2 += rhs->cols;
                    sum += in2 * in4;
                    pIn1 += 4u;
                    
                    /* Decrement the loop count */
                    colCnt--;
                }
                
                /* If the columns of lhs is not a multiple of 4, compute any remaining MACs here.
                 ** No loop unrolling is used. */
                colCnt = lhs->cols % 0x4u;
                
                while(colCnt > 0u)
                {
                    /* c(m,n) = a(1,1)*b(1,1) + a(1,2) * b(2,1) + .... + a(m,p)*b(p,n) */
                    sum += *pIn1++ * (*pIn2);
                    pIn2 += rhs->cols;
                    
                    /* Decrement the loop counter */
                    colCnt--;
                }
                
                /* Store the result in the destination buffer */
                *px++ = sum;
                
                /* Update the pointer pIn2 to point to the  starting address of the next column */
                j++;
                pIn2 = rhs->data + j;
                
                /* Decrement the column loop counter */
                col--;
                
            } while(col > 0u);
            /* Update the pointer pInA to point to the  starting address of the next row */
            i = i + rhs->cols;
            pInA = pInA + lhs->cols;
            
            /* Decrement the row loop counter */
            row--;
            
        } while(row > 0u);
    }
    return MATRIX_SUCCESS;
    
}

//------matrix_t multiplication with lhs transposed------//
matrixStatus_t matrixMult_lhsT(matrix_t *lhs, matrix_t *rhs, matrix_t *result)
{
    uint8_t i, j, k;
    if (lhs->rows != rhs->rows) {
        return MATRIX_ERROR;
    }
    matrixZeros(result);
    for (i = 0; i < lhs->cols; i++)
        for (j = 0; j < rhs->cols; j++)
            for (k = 0; k < lhs->rows; k++)
                ELEMP(result, i, j) += ELEMP(lhs, k, i) * ELEMP(rhs, k, j);
    return MATRIX_SUCCESS;
}

//------matrix_t multiplication with rhs transposed------//
matrixStatus_t matrixMult_rhsT(matrix_t *lhs, matrix_t *rhs, matrix_t *result)
{
    uint8_t i, j, k;
    if (lhs->cols != rhs->cols) {
        return MATRIX_ERROR;
    }
    matrixZeros(result);
    for (i = 0; i < lhs->rows; i++)
        for (j = 0; j < rhs->rows; j++)
            for (k = 0; k < lhs->cols; k++)
                ELEMP(result, i, j) += ELEMP(lhs, i, k) * ELEMP(rhs, j, k);
    return MATRIX_SUCCESS;
}

//---------------Scalar multiplication------------------//
void matrixMultScalar(matrix_t *lhs, float sc, matrix_t *result)
{
    uint16_t ii;
    for (ii = 0; ii < (lhs->cols * lhs->rows); ii++) {
        result->data[ii] = lhs->data[ii] * sc;
    }
    return;
}

//--------------------Inverse LU------------------------//
void matrixInversed(matrix_t *lhs, matrix_t *result)
{
    matrix_t Eye;
    matrixInit(&Eye,lhs->rows, lhs->cols);
    matrixIdentity(&Eye);
    LinSolveLU(lhs, &Eye, result);
    matrixDelete(&Eye);
    return;
}

//-----------------Robust Inverse LUP-------------------//
void matrixInversed_rob(matrix_t *lhs, matrix_t *result)
{
    matrix_t Eye;
    matrixInit(&Eye,lhs->rows, lhs->cols);
    matrixIdentity(&Eye);
    LinSolveLUP(lhs, &Eye, result);
    matrixDelete(&Eye);
    return;
}

//-----------------Transposed--------------------//
void matrixTrans(matrix_t *lhs, matrix_t *result)
{
    uint8_t ii, jj;
    for (ii = 0; ii < lhs->rows; ii++)
        for (jj = 0; jj < lhs->cols; jj++)
            ELEMP(result, jj, ii) = ELEMP(lhs, ii, jj);
    return;
}

//-----------------Nomalized--------------------//
void matrixNormalized(matrix_t *lhs, matrix_t *result)
{
    float k = 1.0f / matrixNorm(lhs);
    matrixMultScalar(lhs, k, result);
    return;
}

//-------Moore-Penrose pseudo inverse---------//
void matrixPseudo_inv(matrix_t *lhs, matrix_t *result)
{
    matrix_t tran, mult1;
    matrixInit(&tran, lhs->cols, lhs->rows);
    matrixInit(&mult1, lhs->cols, lhs->cols);
    matrixTrans(lhs, &tran);
    matrixMult(&tran, lhs, &mult1);
    LinSolveLU(&mult1, &tran, result);
    matrixDelete(&tran);
    matrixDelete(&mult1);
    return;
}

//=======================================matrix_t Data=========================================//

//-----------Returns the determinant----------//
float matrixDet(matrix_t *matrix)
{
    matrix_t L, U, P;
    matrixInit(&L, matrix->rows, matrix->rows);
    matrixInit(&U, matrix->rows, matrix->rows);
    matrixInit(&P, matrix->rows, 1);
    int16_t ii;
    int8_t det_f;
    float determinant = 1.0f;
    
    if (matrix->rows != matrix->cols) {
        matrixDelete(&L);
        matrixDelete(&U);
        matrixDelete(&P);
        return 0.0f;
    }
    
    if (LU_Cormen(matrix, &L, &U)) {
        for (ii = 0; ii < matrix->rows; ii++) {
            determinant *= ELEM(U, ii, ii);
        }
    }
    
    else {
        det_f = LUP_Cormen(matrix, &L, &U, &P);
        if (det_f) {
            for (ii = 0; ii < matrix->rows; ii++) {
                determinant *= ELEM(U, ii, ii);
            }
            determinant *= det_f;
        }
        else {
            determinant = 0.0f;
        }
    }
    
    matrixDelete(&L);
    matrixDelete(&U);
    matrixDelete(&P);
    
    return determinant;
}

//-------------Returns the norm--------------//
float matrixNorm(matrix_t *matrix)
{
    float result = 0.0f;
    uint16_t i;
    for (i = 0; i < (matrix->rows * matrix->cols); i++) {
        result += matrix->data[i] * matrix->data[i];
    }
    result = sqrtf(result);
    return result;
}

//-------------Deletes the data--------------//
matrixStatus_t matrixDelete(matrix_t *matrix)
{
    if(matrix->data == NULL)
    {
        return MATRIX_ERROR;
    }
    
    free(matrix->data);
    
    return MATRIX_SUCCESS;
}
