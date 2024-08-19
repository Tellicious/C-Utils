/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_LKHashTable.c
 * \author          Andrea Vivani
 * \brief           Unit tests for LKHashTable.c
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
#include "LKHashTable.h"

#include <cmocka.h>

/* Functions -----------------------------------------------------------------*/

static void test_lkHashTableInit(void** state) {
    (void)state; /* unused */
    lkHashTable_t lkht;
    assert_int_equal(lkHashTableInit(&lkht, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkht.size, 10);
    assert_int_equal(lkht.items, 0);
    assert_non_null(lkht.entries);
    for (uint32_t i = 0; i < lkht.size; i++) {
        assert_int_equal(lkht.entries[i].size, UINT16_MAX);
        assert_int_equal(lkht.entries[i].items, 0);
    }
    test_free(lkht.entries);
}

static void test_lkHashTablePutAndGet(void** state) {
    (void)state; /* unused */
    lkHashTable_t lkht;
    assert_int_equal(lkHashTableInit(&lkht, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    int value1 = 42, value2 = 43, out_value;
    assert_int_equal(lkHashTablePut(&lkht, "key1", &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkHashTablePut(&lkht, "key2", &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkht.items, 2);
    assert_int_equal(lkHashTableGet(&lkht, "key1", &out_value, LKHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    assert_int_equal(lkHashTableGet(&lkht, "key1", &out_value, LKHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_BUCKET_EMPTY);
    assert_int_equal(lkHashTableGet(&lkht, "key2", &out_value, LKHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(lkHashTableGet(&lkht, "key3", &out_value, LKHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_BUCKET_EMPTY);
    assert_int_equal(lkHashTableGet(&lkht, "key2", &out_value, LKHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    test_free(lkht.entries);
}

static void test_lkHashTableInfo(void** state) {
    (void)state; /* unused */
    lkHashTable_t lkht;
    assert_int_equal(lkHashTableInit(&lkht, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    uint32_t size, items;
    lkHashTableInfo(&lkht, &size, &items);
    assert_int_equal(size, 10);
    assert_int_equal(items, 0);
    int value = 42;
    lkHashTablePut(&lkht, "key1", &value);
    lkHashTableInfo(&lkht, &size, &items);
    assert_int_equal(items, 1);
    lkHashTableDelete(&lkht);
}

static void test_lkHashTableFlush(void** state) {
    (void)state; /* unused */
    lkHashTable_t lkht;
    assert_int_equal(lkHashTableInit(&lkht, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    int value1 = 42, value2 = 43;
    assert_int_equal(lkHashTablePut(&lkht, "key1", &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkHashTablePut(&lkht, "key2", &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkht.items, 2);
    assert_int_equal(lkHashTableFlush(&lkht), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkht.items, 0);
    for (uint32_t i = 0; i < lkht.size; i++) {
        assert_int_equal(lkht.entries[i].items, 0);
    }
    test_free(lkht.entries);
}

static void test_lkHashTableDelete(void** state) {
    (void)state; /* unused */
    lkHashTable_t lkht;
    assert_int_equal(lkHashTableInit(&lkht, sizeof(int), 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(lkHashTableDelete(&lkht), UTILS_STATUS_SUCCESS);
}

int main(void) {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_lkHashTableInit),
                                       cmocka_unit_test(test_lkHashTablePutAndGet),
                                       cmocka_unit_test(test_lkHashTableInfo), cmocka_unit_test(test_lkHashTableFlush),
                                       cmocka_unit_test(test_lkHashTableDelete)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
