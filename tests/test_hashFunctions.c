/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_hashFunctions.c
 * \author          Andrea Vivani
 * \brief           Unit tests for hashFunctions.c
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
#include <stddef.h>
#include "hashFunctions.h"

#include <cmocka.h>

/* Functions -----------------------------------------------------------------*/

static void test_hash_FNV1A(void** state) {
    (void)state; /* unused */

    // Test known values
    assert_int_equal(hash_FNV1A(""), 2166136261U);
    assert_int_equal(hash_FNV1A("a"), 3826002220U);
    assert_int_equal(hash_FNV1A("abc"), 1081548509U);
    assert_int_equal(hash_FNV1A("message"), 2381907949U);

    // Test edge case: long string
    char long_str[1000];
    for (int i = 0; i < 999; ++i) {
        long_str[i] = 'a';
    }
    long_str[999] = '\0';
    assert_int_not_equal(hash_FNV1A(long_str), 0);

    // Test edge case: different case
    assert_int_not_equal(hash_FNV1A("abc"), hash_FNV1A("ABC"));
}

static void test_hash_djb(void** state) {
    (void)state; /* unused */

    // Test known values
    assert_int_equal(hash_djb(""), 5381U);
    assert_int_equal(hash_djb("a"), 177670U);
    assert_int_equal(hash_djb("abc"), 193485963U);
    assert_int_equal(hash_djb("message"), 210718207996U);

    // Test edge case: long string
    char long_str[1000];
    for (int i = 0; i < 999; ++i) {
        long_str[i] = 'a';
    }
    long_str[999] = '\0';
    assert_int_not_equal(hash_djb(long_str), 0);

    // Test edge case: different case
    assert_int_not_equal(hash_djb("abc"), hash_djb("ABC"));
}

static void test_hash_sdbm(void** state) {
    (void)state; /* unused */

    // Test known values
    assert_int_equal(hash_sdbm(""), 0U);
    assert_int_equal(hash_sdbm("a"), 97U);
    assert_int_equal(hash_sdbm("abc"), 96354U);
    assert_int_equal(hash_sdbm("message"), 418229425U);

    // Test edge case: long string
    char long_str[1000];
    for (int i = 0; i < 999; ++i) {
        long_str[i] = 'a';
    }
    long_str[999] = '\0';
    assert_int_not_equal(hash_sdbm(long_str), 0);

    // Test edge case: different case
    assert_int_not_equal(hash_sdbm("abc"), hash_sdbm("ABC"));
}

int main(void) {
    const struct CMUnitTest test_hashFunctions[] = {
        cmocka_unit_test(test_hash_FNV1A),
        cmocka_unit_test(test_hash_djb),
        cmocka_unit_test(test_hash_sdbm),
    };
    return cmocka_run_group_tests(test_hashFunctions, NULL, NULL);
}
