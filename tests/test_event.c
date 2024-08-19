/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_event.c
 * \author          Andrea Vivani
 * \brief           Unit tests for event.c
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
#include "event.h"

#include <cmocka.h>

/* Functions -----------------------------------------------------------------*/

static int basic_event_triggered = 0;
static int extended_event_triggered = 0;
static void* extended_event_value = NULL;

void basic_event_handler(void) { basic_event_triggered++; }

void extended_event_handler(void* val) {
    extended_event_triggered++;
    extended_event_value = val;
}

static void test_eventInit(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_BASIC, 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(event.type, EVENT_TYPE_BASIC);
    assert_int_equal(event.count, 0);
    assert_int_equal(event.size, 10);
    assert_non_null(event.eventsList);
    eventDelete(&event);
}

static void test_eventInitStatic(void** state) {
    (void)state; /* unused */
    event_t event;
    eventExtCBType_t buffer[10];
    eventInitStatic(&event, buffer, EVENT_TYPE_BASIC, 10);
    assert_int_equal(event.type, EVENT_TYPE_BASIC);
    assert_int_equal(event.count, 0);
    assert_int_equal(event.size, 10);
    assert_ptr_equal(event.eventsList, buffer);
}

static void test_eventRegister(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_BASIC, 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(eventRegister(&event, basic_event_handler), UTILS_STATUS_SUCCESS);
    assert_int_equal(event.count, 1);
    eventDelete(&event);
}

static void test_eventRegisterEx(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_EXTENDED, 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(eventRegisterEx(&event, extended_event_handler), UTILS_STATUS_SUCCESS);
    assert_int_equal(event.count, 1);
    eventDelete(&event);
}

static void test_eventDispatch(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_BASIC, 10), UTILS_STATUS_SUCCESS);
    eventRegister(&event, basic_event_handler);
    basic_event_triggered = 0;
    assert_int_equal(eventDispatch(&event), UTILS_STATUS_SUCCESS);
    assert_int_equal(basic_event_triggered, 1);
    eventDelete(&event);
}

static void test_eventDispatchEx(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_EXTENDED, 10), UTILS_STATUS_SUCCESS);
    eventRegisterEx(&event, extended_event_handler);
    extended_event_triggered = 0;
    int test_value = 42;
    extended_event_value = NULL;
    assert_int_equal(eventDispatchEx(&event, &test_value), UTILS_STATUS_SUCCESS);
    assert_int_equal(extended_event_triggered, 1);
    assert_ptr_equal(extended_event_value, &test_value);
    eventDelete(&event);
}

static void test_eventDelete(void** state) {
    (void)state; /* unused */
    event_t event;
    assert_int_equal(eventInit(&event, EVENT_TYPE_BASIC, 10), UTILS_STATUS_SUCCESS);
    assert_int_equal(eventDelete(&event), UTILS_STATUS_SUCCESS);
}

int main(void) {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_eventInit),     cmocka_unit_test(test_eventInitStatic),
                                       cmocka_unit_test(test_eventRegister), cmocka_unit_test(test_eventRegisterEx),
                                       cmocka_unit_test(test_eventDispatch), cmocka_unit_test(test_eventDispatchEx),
                                       cmocka_unit_test(test_eventDelete)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
