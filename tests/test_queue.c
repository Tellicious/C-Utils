/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_queue.c
 * \author          Andrea Vivani
 * \brief           Unit tests for queue.c
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
#include "queue.h"

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

#ifdef ADVUTILS_USE_DYNAMIC_ALLOCATION

static void test_queueInit(void** state) {
    (void)state; /* unused */
    queue_t queue;
    assert_int_equal(queueInit(&queue, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.size, 10 * sizeof(int));
    assert_int_equal(queue.items, 0);
    assert_non_null(queue.data);
    queueDelete(&queue);
    /* Check null initialization */
    skipAssert = 0;
    expect_assert_failure(queueInit(&queue, sizeof(int), 0));
    skipAssert = 1;
    assert_int_equal(queueInit(&queue, sizeof(int), 0), UTILS_STATUS_ERROR);
    skipAssert = 0;
}

#endif /* ADVUTILS_USE_DYNAMIC_ALLOCATION */

#ifdef ADVUTILS_USE_STATIC_ALLOCATION

static void test_queueInitStatic(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    assert_int_equal(queue.size, 10 * sizeof(int));
    assert_int_equal(queue.items, 0);
    assert_ptr_equal(queue.data, buffer);
}

#endif /* ADVUTILS_USE_STATIC_ALLOCATION */

static void test_queuePush(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[2 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 2);
    int value = 42;
    assert_int_equal(queuePush(&queue, &value), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(queue.data, &value, sizeof(int));
    assert_memory_equal(&queue.data[sizeof(int)], &value, sizeof(int));
    assert_int_equal(queuePush(&queue, &value), UTILS_STATUS_FULL);
}

static void test_queuePushFront(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[2 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 2);
    int value1 = 42;
    int value2 = 43;
    queuePush(&queue, &value1);
    queuePushFront(&queue, &value2);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(&queue.data[queue._front], &value2, sizeof(int));
    assert_int_equal(queuePushFront(&queue, &value1), UTILS_STATUS_FULL);
}

static void test_queuePushArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[5 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 5);
    int values[] = {1, 2, 3, 4, 5};
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
    /* Test queue full */
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_ERROR);
}

static void test_queuePushFrontArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 5);
    int values[] = {1, 2, 3, 4, 5};
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(&queue.data[queue._front], values, 5 * sizeof(int));
    /* Test queue full */
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_ERROR);
}

static void test_queuePop(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value = 42;
    int out_value;
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value);
    assert_int_equal(queue.items, 0);
}

static void test_queuePopBack(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value1 = 42;
    int value2 = 43;
    int out_value;
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(queue.items, sizeof(int));
}

static void test_queuePopArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    queuePushArr(&queue, values, 5);
    int out_values[5];
    assert_int_equal(queuePopArr(&queue, out_values, 5), UTILS_STATUS_SUCCESS);
    assert_memory_equal(out_values, values, 5 * sizeof(int));
    assert_int_equal(queue.items, 0);
    assert_int_equal(queuePopArr(&queue, out_values, 5), UTILS_STATUS_ERROR);
}

static void test_queuePeek(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value = 42;
    int out_value;
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value);
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value);
    assert_int_equal(queue.items, sizeof(int));
}

static void test_queuePeekBack(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value1 = 42;
    int value2 = 43;
    int out_value;
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(queue.items, 2 * sizeof(int));
}

static void test_queueFlush(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value = 42;
    queuePush(&queue, &value);
    assert_int_equal(queueFlush(&queue), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 0);
    /* Check null deletion */
    queue.data = NULL;
    assert_int_equal(queueFlush(&queue), UTILS_STATUS_ERROR);
}

//TODO Clean-up queue wrap-around testing
static void test_queueWrapAround(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    int value;
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    queuePop(&queue, &value);
    queuePop(&queue, &value);
    queuePop(&queue, &value);
    queuePop(&queue, &value);
    queuePop(&queue, &value);
    value = 33;
    queuePush(&queue, &value);
    queuePush(&queue, &value);
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_memory_equal(&queue.data[7 * sizeof(int)], values, 3 * sizeof(int));
    assert_memory_equal(queue.data, &values[3], 2 * sizeof(int));
    /* Test wrap-around from first element */
    for (uint8_t ii = 0; ii < 10; ii++) {
        buffer[ii] = 0;
    }
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    queuePushArr(&queue, values, 5);
    queuePushArr(&queue, values, 5);
    queuePopArr(&queue, values, 5);
    queuePushArr(&queue, values, 5);
    assert_memory_equal(&queue.data[0], values, 5 * sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], values, 5 * sizeof(int));
    /* Test wrap-around also with standard push */
    for (uint8_t ii = 0; ii < 10; ii++) {
        buffer[ii] = 0;
    }
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    queuePushArr(&queue, values, 5);
    queuePushArr(&queue, values, 5);
    queuePopArr(&queue, values, 5);
    queuePush(&queue, &value);
    queuePush(&queue, &value);
    assert_memory_equal(&queue.data[0], &value, sizeof(int));
    assert_memory_equal(&queue.data[sizeof(int)], &value, sizeof(int));
    /* Test wrap-around with pushFrontArr */
    queuePopBack(&queue, &value);
    queuePopBack(&queue, &value);
    queuePopBack(&queue, &value);
    queuePushFront(&queue, &value);
    queuePushFrontArr(&queue, values, 5);
    assert_memory_equal(&queue.data[0], &values[1], 4 * sizeof(int));
    assert_memory_equal(&queue.data[4 * sizeof(int)], &value, sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], &values[0], 4 * sizeof(int));
    assert_memory_equal(&queue.data[9 * sizeof(int)], &values[0], sizeof(int));
    /* Test wrap-around with pop */
    queuePop(&queue, &value);
    assert_float_equal(value, values[0], 1e-5);
    queuePop(&queue, &value);
    assert_float_equal(value, values[1], 1e-5);
    /* Test wrap-around with popArr */
    for (uint8_t ii = 0; ii < 10; ii++) {
        buffer[ii] = 0;
    }
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    values[0] = 1;
    values[1] = 2;
    values[2] = 3;
    values[3] = 4;
    values[4] = 5;
    queuePushArr(&queue, values, 5);
    queuePushArr(&queue, values, 5);
    queuePopArr(&queue, values, 5);
    queuePop(&queue, &value);
    value = 33;
    queuePush(&queue, &value);
    queuePopArr(&queue, values, 5);
    assert_float_equal(values[0], 2, 1e-5);
    assert_float_equal(values[1], 3, 1e-5);
    assert_float_equal(values[2], 4, 1e-5);
    assert_float_equal(values[3], 5, 1e-5);
    assert_float_equal(values[4], 33, 1e-5);
    for (uint8_t ii = 0; ii < 10; ii++) {
        buffer[ii] = 0;
    }
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    values[0] = 1;
    values[1] = 2;
    values[2] = 3;
    values[3] = 4;
    values[4] = 5;
    queuePushArr(&queue, values, 5);
    queuePushArr(&queue, values, 5);
    queuePopArr(&queue, values, 5);
    queuePush(&queue, &value);
    queuePopArr(&queue, values, 5);
    queuePop(&queue, &value);
    assert_float_equal(value, 33, 1e-5);
}

#ifdef ADVUTILS_USE_DYNAMIC_ALLOCATION

static void test_queueDelete(void** state) {
    (void)state; /* unused */
    queue_t queue;
    assert_int_equal(queueInit(&queue, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(queueDelete(&queue), UTILS_STATUS_SUCCESS);
    /* Check null deletion */
    queue.data = NULL;
    assert_int_equal(queueDelete(&queue), UTILS_STATUS_ERROR);
}

#endif /* ADVUTILS_USE_DYNAMIC_ALLOCATION */

int main(void) {
    const struct CMUnitTest tests[] = {
#ifdef ADVUTILS_USE_DYNAMIC_ALLOCATION
        cmocka_unit_test(test_queueInit),         cmocka_unit_test(test_queueDelete),
#endif
#ifdef ADVUTILS_USE_STATIC_ALLOCATION
        cmocka_unit_test(test_queueInitStatic),   cmocka_unit_test(test_queuePush),
        cmocka_unit_test(test_queuePushFront),    cmocka_unit_test(test_queuePushArr),
        cmocka_unit_test(test_queuePushFrontArr), cmocka_unit_test(test_queuePop),
        cmocka_unit_test(test_queuePopBack),      cmocka_unit_test(test_queuePopArr),
        cmocka_unit_test(test_queuePeek),         cmocka_unit_test(test_queuePeekBack),
        cmocka_unit_test(test_queueFlush),        cmocka_unit_test(test_queueWrapAround),
#endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
