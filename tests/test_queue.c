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
#include <string.h>
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

static void test_queueInitStatic(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    assert_int_equal(queue.size, 10 * sizeof(int));
    assert_int_equal(queue.items, 0);
    assert_ptr_equal(queue.data, buffer);
}

static void test_queuePush(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[4 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 4);
    int value1 = 42;
    int value2 = 43;
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(queue.data, &value1, sizeof(int));
    assert_memory_equal(&queue.data[sizeof(int)], &value2, sizeof(int));
    assert_int_equal(queuePush(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value2), UTILS_STATUS_SUCCESS);
    /* Test queue full */
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_FULL);
    /* Test wrap-around during push */
    memset(buffer, 0x00, 4 * sizeof(int));
    queue.items = 0;
    queue._front = 3 * sizeof(int);
    queue._rear = 3 * sizeof(int);
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(&queue.data[3 * sizeof(int)], &value1, sizeof(int));
    assert_memory_equal(queue.data, &value2, sizeof(int));
}

static void test_queuePushFront(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[2 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 2);
    int value1 = 42;
    int value2 = 43;
    queue._front = 1 * sizeof(int);
    queue._rear = 1 * sizeof(int);
    queuePush(&queue, &value1);
    queuePushFront(&queue, &value2);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(queue.data, &value2, sizeof(int));
    assert_int_equal(queuePushFront(&queue, &value1), UTILS_STATUS_FULL);
    /* Test wrap-around during pushFront */
    memset(buffer, 0x00, 2 * sizeof(int));
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    queuePush(&queue, &value1);
    queuePushFront(&queue, &value2);
    assert_int_equal(queue.items, 2 * sizeof(int));
    assert_memory_equal(&queue.data[1 * sizeof(int)], &value2, sizeof(int));
}

static void test_queuePushArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 10 * sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], values, 5 * sizeof(int));
    /* Test queue full and almost full */
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_ERROR);
    queue.items = queue.size - 4;
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_ERROR);
    /* Test wrap-around during pushArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    queue.items = 0;
    queue._front = 7 * sizeof(int);
    queue._rear = 7 * sizeof(int);
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(&queue.data[7 * sizeof(int)], values, 3 * sizeof(int));
    assert_memory_equal(queue.data, &values[3], 2 * sizeof(int));
    /* Test wrap-around from the first element of pushArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    queue.items = 0;
    queue._front = 10 * sizeof(int);
    queue._rear = 10 * sizeof(int);
    assert_int_equal(queuePushArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
}

static void test_queuePushFrontArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 5 * sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], values, 5 * sizeof(int));
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_SUCCESS);
    assert_int_equal(queue.items, 10 * sizeof(int));
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
    /* Test queue full and almost full */
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_ERROR);
    queue.items = queue.size - 4;
    assert_int_equal(queuePushFrontArr(&queue, values, 5), UTILS_STATUS_ERROR);
    /* Wrap-around during pushFrontArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    queue.items = 0;
    queue._front = 2 * sizeof(int);
    queue._rear = 2 * sizeof(int);
    queuePushFrontArr(&queue, values, 5);
    assert_memory_equal(queue.data, &values[3], 2 * sizeof(int));
    assert_memory_equal(&queue.data[7 * sizeof(int)], values, 3 * sizeof(int));
    /* Wrap around from last element of pushFrontArr already contained in the first standard test */
}

static void test_queuePop(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[3 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 3);
    int value1 = 42;
    int value2 = 43;
    int out_value = 0;
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    assert_int_equal(queue.items, 0);
    /* Test wrap-around during pop */
    memset(buffer, 0x00, 3 * sizeof(int));
    queue.items = 0;
    queue._front = 2 * sizeof(int);
    queue._rear = 2 * sizeof(int);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_memory_equal(&queue.data[2 * sizeof(int)], &value1, sizeof(int));
    assert_memory_equal(queue.data, &value2, sizeof(int));
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
}

static void test_queuePopBack(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[4 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 4);
    int value1 = 42;
    int value2 = 43;
    int out_value = 0;
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(queue.items, sizeof(int));
    /* Test wrap-around during popBack */
    memset(buffer, 0x00, 4 * sizeof(int));
    out_value = 0;
    queue.items = 0;
    queue._front = 3 * sizeof(int);
    queue._rear = 3 * sizeof(int);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_memory_equal(&queue.data[3 * sizeof(int)], &value1, sizeof(int));
    assert_memory_equal(queue.data, &value2, sizeof(int));
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
}

static void test_queuePopArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    queuePushArr(&queue, values, 5);
    int out_values[5] = {0};
    assert_int_equal(queuePopArr(&queue, out_values, 5), UTILS_STATUS_SUCCESS);
    assert_memory_equal(out_values, values, 5 * sizeof(int));
    assert_int_equal(queue.items, 0);
    /* Test queue empty and almost empty */
    assert_int_equal(queuePopArr(&queue, out_values, 5), UTILS_STATUS_ERROR);
    queue.items = 4;
    assert_int_equal(queuePopArr(&queue, out_values, 5), UTILS_STATUS_ERROR);
    /* Test wrap-around during popArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    memset(out_values, 0x00, 5 * sizeof(int));
    queue.items = 0;
    queue._front = 7 * sizeof(int);
    queue._rear = 7 * sizeof(int);
    queuePushArr(&queue, values, 5);
    assert_memory_equal(&queue.data[7 * sizeof(int)], values, 3 * sizeof(int));
    assert_memory_equal(queue.data, &values[3], 2 * sizeof(int));
    queuePopArr(&queue, out_values, 5);
    assert_memory_equal(values, out_values, 5 * sizeof(int));
    /* Test wrap-around from first element of popArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    memset(out_values, 0x00, 5 * sizeof(int));
    queue.items = 0;
    queue._front = 10 * sizeof(int);
    queue._rear = 10 * sizeof(int);
    queuePushArr(&queue, values, 5);
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
    queuePopArr(&queue, out_values, 5);
    assert_memory_equal(values, out_values, 5 * sizeof(int));
}

static void test_queuePopBackArr(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int values[] = {1, 2, 3, 4, 5};
    int values2[] = {11, 12, 13, 14, 15};
    queuePushArr(&queue, values, 5);
    int out_values[5] = {0};
    assert_int_equal(queuePopBackArr(&queue, out_values, 5), UTILS_STATUS_SUCCESS);
    assert_memory_equal(out_values, values, 5 * sizeof(int));
    assert_int_equal(queue.items, 0);
    /* Test queue empty and almost empty */
    assert_int_equal(queuePopBackArr(&queue, out_values, 5), UTILS_STATUS_ERROR);
    queue.items = 4;
    assert_int_equal(queuePopBackArr(&queue, out_values, 5), UTILS_STATUS_ERROR);
    /* Test wrap-around during popArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    memset(out_values, 0x00, 5 * sizeof(int));
    queue.items = 0;
    queue._front = 7 * sizeof(int);
    queue._rear = 7 * sizeof(int);
    queuePushArr(&queue, values, 5);
    assert_memory_equal(&queue.data[7 * sizeof(int)], values, 3 * sizeof(int));
    assert_memory_equal(queue.data, &values[3], 2 * sizeof(int));
    queuePopBackArr(&queue, out_values, 5);
    assert_memory_equal(values, out_values, 5 * sizeof(int));
    /* Test wrap-around from last element of popArr */
    memset(buffer, 0x00, 10 * sizeof(int));
    memset(out_values, 0x00, 5 * sizeof(int));
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    queuePushFrontArr(&queue, values2, 5);
    queuePushFrontArr(&queue, values, 5);
    assert_memory_equal(queue.data, values, 5 * sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], values2, 5 * sizeof(int));
    queuePopBackArr(&queue, out_values, 5);
    assert_memory_equal(values2, out_values, 5 * sizeof(int));
}

static void test_queuePeek(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value1 = 42;
    int value2 = 43;
    int out_value = 0;
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    assert_int_equal(queue.items, 2 * sizeof(int));
    /* Test wrap-around during peekBack */
    memset(buffer, 0x00, 10 * sizeof(int));
    out_value = 0;
    queue.items = 0;
    queue._front = 9 * sizeof(int);
    queue._rear = 9 * sizeof(int);
    queuePush(&queue, &value1);
    assert_memory_equal(&queue.data[9 * sizeof(int)], &value1, sizeof(int));
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    queuePop(&queue, &out_value);
    queuePush(&queue, &value2);
    assert_memory_equal(queue.data, &value2, sizeof(int));
    assert_int_equal(queuePeek(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
}

static void test_queuePeekBack(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value1 = 42;
    int value2 = 43;
    int out_value = 0;
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_EMPTY);
    queuePush(&queue, &value1);
    queuePush(&queue, &value2);
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(queue.items, 2 * sizeof(int));
    /* Test wrap-around during peekBack */
    memset(buffer, 0x00, 10 * sizeof(int));
    out_value = 0;
    queue.items = 0;
    queue._front = 0;
    queue._rear = 0;
    queuePushFront(&queue, &value1);
    assert_memory_equal(&queue.data[9 * sizeof(int)], &value1, sizeof(int));
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    queuePush(&queue, &value2);
    assert_memory_equal(queue.data, &value2, sizeof(int));
    assert_int_equal(queuePeekBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
}

static void test_queuePushPop(void** state) {
    (void)state; /* unused */
    queue_t queue;
    uint8_t buffer[10 * sizeof(int)];
    queueInitStatic(&queue, buffer, sizeof(int), 10);
    int value1 = 42;
    int value2 = 43;
    int value3 = 44;
    int values1[] = {1, 2, 3, 4};
    int values2[] = {12, 13, 14};
    int out_value = 0;
    int out_values1[2];
    int out_values2[4];
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePushFront(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePushArr(&queue, values1, 4), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePushFrontArr(&queue, values2, 3), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value3), UTILS_STATUS_SUCCESS);
    assert_memory_equal(queue.data, &value1, sizeof(int));
    assert_memory_equal(&queue.data[1 * sizeof(int)], values1, 4 * sizeof(int));
    assert_memory_equal(&queue.data[5 * sizeof(int)], &value3, sizeof(int));
    assert_memory_equal(&queue.data[6 * sizeof(int)], values2, 3 * sizeof(int));
    assert_memory_equal(&queue.data[9 * sizeof(int)], &value2, sizeof(int));
    assert_int_equal(queuePopBackArr(&queue, out_values2, 4), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_values2[0], values1[1]);
    assert_int_equal(out_values2[1], values1[2]);
    assert_int_equal(out_values2[2], values1[3]);
    assert_int_equal(out_values2[3], value3);
    assert_int_equal(queuePopArr(&queue, out_values1, 2), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_values1[0], values2[0]);
    assert_int_equal(out_values1[1], values2[1]);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values1[0]);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values2[2]);
    assert_int_equal(queuePopBackArr(&queue, out_values1, 2), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_values1[0], value2);
    assert_int_equal(out_values1[1], value1);
    /* Mixed test */
    memset(out_values1, 0x00, 2 * sizeof(int));
    memset(out_values2, 0x00, 4 * sizeof(int));
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePushFront(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePopBackArr(&queue, out_values1, 2), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_values1[0], value2);
    assert_int_equal(out_values1[1], value1);
    assert_int_equal(queuePushArr(&queue, values1, 4), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values1[0]);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values1[1]);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values1[2]);
    assert_int_equal(queuePop(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values1[3]);
    assert_int_equal(queuePushFrontArr(&queue, values2, 3), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values2[2]);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values2[1]);
    assert_int_equal(queuePopBack(&queue, &out_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, values2[0]);
    assert_int_equal(queuePush(&queue, &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePush(&queue, &value3), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePushFront(&queue, &value3), UTILS_STATUS_SUCCESS);
    assert_int_equal(queuePopArr(&queue, out_values2, 4), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_values2[0], value3);
    assert_int_equal(out_values2[1], value1);
    assert_int_equal(out_values2[2], value2);
    assert_int_equal(out_values2[3], value3);
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

static void test_queueDelete(void** state) {
    (void)state; /* unused */
    queue_t queue;
    assert_int_equal(queueInit(&queue, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(queueDelete(&queue), UTILS_STATUS_SUCCESS);
    /* Check null deletion */
    queue.data = NULL;
    assert_int_equal(queueDelete(&queue), UTILS_STATUS_ERROR);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queueInit),    cmocka_unit_test(test_queueInitStatic),
        cmocka_unit_test(test_queuePush),    cmocka_unit_test(test_queuePushFront),
        cmocka_unit_test(test_queuePushArr), cmocka_unit_test(test_queuePushFrontArr),
        cmocka_unit_test(test_queuePop),     cmocka_unit_test(test_queuePopBack),
        cmocka_unit_test(test_queuePopArr),  cmocka_unit_test(test_queuePopBackArr),
        cmocka_unit_test(test_queuePeek),    cmocka_unit_test(test_queuePeekBack),
        cmocka_unit_test(test_queuePushPop), cmocka_unit_test(test_queueFlush),
        cmocka_unit_test(test_queueDelete),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
