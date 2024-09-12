/* BEGIN Header */
/**
 ******************************************************************************
 * \file            movingAvg.c
 * \author          Andrea Vivani
 * \brief           Unit tests for movingAvg.c
 ******************************************************************************
 * \copyright
 *
 * Copyright 2024 Andrea Vivani
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

#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "movingAvg.h"

#include <cmocka.h>

/* Support functions ---------------------------------------------------------*/

void* ADVUtils_testCalloc(const size_t number_of_elements, const size_t size) {
    if (number_of_elements > 0) {
        return test_calloc(number_of_elements, size);
    } else {
        return NULL;
    }
}

void* ADVUtils_testMalloc(const size_t size) {
    if (size > 0) {
        return test_malloc(size);
    } else {
        return NULL;
    }
}

static uint8_t skipAssert = 0;

void ADVUtils_testAssert(const int result, const char* const expression, const char* const file, const int line) {
    if (skipAssert) {
        return;
    } else {
        mock_assert(result, expression, file, line);
    }
}

/* Functions -----------------------------------------------------------------*/

static void test_movingAvgInit(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    assert_int_equal(movingAvgInit(&movingAvg, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(movingAvg.size, 5);
    assert_non_null(movingAvg.data);
    movingAvgDelete(&movingAvg);
    /* Check null initialization */
    skipAssert = 0;
    expect_assert_failure(movingAvgInit(&movingAvg, 0));
    skipAssert = 1;
    assert_int_equal(movingAvgInit(&movingAvg, 0), UTILS_STATUS_ERROR);
    skipAssert = 0;
}

static void test_movingAvgDelete(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    assert_int_equal(movingAvgInit(&movingAvg, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(movingAvgDelete(&movingAvg), UTILS_STATUS_SUCCESS);
    /* Check null deletion */
    movingAvg.data = NULL;
    assert_int_equal(movingAvgDelete(&movingAvg), UTILS_STATUS_ERROR);
}

static void test_movingAvgInitStatic(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    float buffer[5];
    movingAvgInitStatic(&movingAvg, buffer, 5);
    assert_int_equal(movingAvg.size, 5);
    assert_ptr_equal(movingAvg.data, buffer);
}

static void test_movingAvgCalc(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    float buffer[5];
    movingAvgInitStatic(&movingAvg, buffer, 5);
    movingAvgFlush(&movingAvg);
    assert_float_equal(movingAvgCalc(&movingAvg, 1.0f), 0.2f, 1e-5);
    assert_float_equal(movingAvgCalc(&movingAvg, 2.0f), 0.6f, 1e-5);
    assert_float_equal(movingAvgCalc(&movingAvg, 3.0f), 1.2f, 1e-5);
    assert_float_equal(movingAvgCalc(&movingAvg, 4.0f), 2.0f, 1e-5);
    assert_float_equal(movingAvgCalc(&movingAvg, 5.0f), 3.0f, 1e-5);
    assert_float_equal(movingAvgCalc(&movingAvg, 6.0f), 4.0f, 1e-5);
}

static void test_movingAvgFlush(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    float buffer[5];
    movingAvgInitStatic(&movingAvg, buffer, 5);
    movingAvgCalc(&movingAvg, 1.0f);
    movingAvgCalc(&movingAvg, 2.0f);
    assert_int_equal(movingAvgFlush(&movingAvg), UTILS_STATUS_SUCCESS);
    assert_int_equal(movingAvg.sum, 0);
    assert_int_equal(movingAvg._write, 0);
    /* Check null deletion */
    movingAvg.data = NULL;
    assert_int_equal(movingAvgFlush(&movingAvg), UTILS_STATUS_ERROR);
}

static void test_movingAvgGetLatest(void** state) {
    (void)state; /* unused */
    movingAvg_t movingAvg;
    float buffer[5];
    movingAvgInitStatic(&movingAvg, buffer, 5);
    movingAvgFlush(&movingAvg);
    movingAvgCalc(&movingAvg, 1.0f);
    movingAvgCalc(&movingAvg, 2.0f);
    movingAvgCalc(&movingAvg, 3.0f);
    assert_float_equal(movingAvgGetLatest(&movingAvg), 1.2f, 1e-5);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_movingAvgInit),       cmocka_unit_test(test_movingAvgDelete),
        cmocka_unit_test(test_movingAvgInitStatic), cmocka_unit_test(test_movingAvgCalc),
        cmocka_unit_test(test_movingAvgFlush),      cmocka_unit_test(test_movingAvgGetLatest),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
