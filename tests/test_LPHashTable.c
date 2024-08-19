/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_LPHashTable.c
 * \author          Andrea Vivani
 * \brief           Unit tests for LPHashTable.c
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
#include "LPHashTable.h"

#include <cmocka.h>

/* Functions -----------------------------------------------------------------*/

static void test_lpHashTableInit(void** state) {
    (void)state; /* unused */
    lpHashTable_t lpht;
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 10, LPHT_RESIZABLE), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpht.size, 10);
    assert_int_equal(lpht.items, 0);
    assert_non_null(lpht.entries);
    test_free(lpht.entries);
}

static void test_lpHashTablePutAndGet(void** state) {
    (void)state; /* unused */
    lpHashTable_t lpht;
    int value1 = 42, value2 = 43, out_value;
    /* Test of resizable hash-table */
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 2, LPHT_RESIZABLE), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTablePut(&lpht, "key1", &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTablePut(&lpht, "key2", &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpht.items, 2);
    assert_int_equal(lpht.size, 4);
    assert_int_equal(lpHashTableGet(&lpht, "key3", &out_value, LPHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_ERROR);
    assert_int_equal(lpHashTableGet(&lpht, "key1", &out_value, LPHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value1);
    assert_int_equal(lpHashTableGet(&lpht, "key1", &out_value, LPHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_ERROR);
    assert_int_equal(lpHashTableGet(&lpht, "key2", &out_value, LPHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    assert_int_equal(out_value, value2);
    assert_int_equal(lpHashTableGet(&lpht, "key3", &out_value, LPHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_ERROR);
    assert_int_equal(lpHashTableGet(&lpht, "key2", &out_value, LPHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    test_free(lpht.entries);
    /* Test of not-resizable hash-table */
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 2, LPHT_NOT_RESIZABLE), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTablePut(&lpht, "key1", &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTablePut(&lpht, "key2", &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpht.items, 2);
    assert_int_equal(lpht.size, 2);
    assert_int_equal(lpHashTableGet(&lpht, "key3", &out_value, LPHT_DO_NOT_REMOVE_ITEM), UTILS_STATUS_ERROR);
    assert_int_equal(lpHashTableGet(&lpht, "key1", &out_value, LPHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTableGet(&lpht, "key2", &out_value, LPHT_REMOVE_ITEM), UTILS_STATUS_SUCCESS);
    test_free(lpht.entries);
}

static void test_lpHashTableInfo(void** state) {
    (void)state; /* unused */
    lpHashTable_t lpht;
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 10, LPHT_RESIZABLE), UTILS_STATUS_SUCCESS);
    uint32_t size, items;
    lpHashTableInfo(&lpht, &size, &items);
    assert_int_equal(size, 10);
    assert_int_equal(items, 0);
    int value = 42;
    lpHashTablePut(&lpht, "key1", &value);
    lpHashTableInfo(&lpht, &size, &items);
    assert_int_equal(items, 1);
    lpHashTableDelete(&lpht);
}

static void test_lpHashTableFlush(void** state) {
    (void)state; /* unused */
    lpHashTable_t lpht;
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 10, LPHT_RESIZABLE), UTILS_STATUS_SUCCESS);
    int value1 = 42, value2 = 43;
    assert_int_equal(lpHashTablePut(&lpht, "key1", &value1), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTablePut(&lpht, "key2", &value2), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpht.items, 2);
    assert_int_equal(lpHashTableFlush(&lpht), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpht.items, 0);
    for (uint32_t i = 0; i < lpht.size; i++) {
        assert_null(lpht.entries[i].key);
        assert_null(lpht.entries[i].value);
    }
    test_free(lpht.entries);
}

static void test_lpHashTableDelete(void** state) {
    (void)state; /* unused */
    lpHashTable_t lpht;
    assert_int_equal(lpHashTableInit(&lpht, sizeof(int), 10, LPHT_RESIZABLE), UTILS_STATUS_SUCCESS);
    assert_int_equal(lpHashTableDelete(&lpht), UTILS_STATUS_SUCCESS);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lpHashTableInit),   cmocka_unit_test(test_lpHashTablePutAndGet),
        cmocka_unit_test(test_lpHashTableInfo),   cmocka_unit_test(test_lpHashTableFlush),
        cmocka_unit_test(test_lpHashTableDelete),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
