/* BEGIN Header */
/**
 ******************************************************************************
 * \file            test_button.c
 * \author          Andrea Vivani
 * \brief           Unit tests for button.c
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
#include "button.h"

#include <cmocka.h>

/* Functions -----------------------------------------------------------------*/

static void test_buttonInit(void** state) {
    (void)state; /* unused */
    button_t button;
    /* Check button type normal */
    buttonInit(&button, BUTTON_TYPE_NORMAL, 20, 400, 1000, 2000);
    assert_int_equal(button.type, BUTTON_TYPE_NORMAL);
    assert_int_equal(button.status, BUTTON_RELEASED);
    assert_int_equal(button.press, BUTTON_NO_PRESS);
    assert_int_equal(button.validTick[0], 1);
    assert_int_equal(button.validTick[1], 0);
    assert_int_equal(button.lastTick[0], 0);
    assert_int_equal(button.lastTick[1], 0);
    assert_int_equal(button.pulses, 0);
    assert_int_equal(button.event, 1);
    assert_int_equal(button.debounceTicks, 20);
    assert_int_equal(button.resetTicks, 400);
    assert_int_equal(button.longPressTicks, 1000);
    assert_int_equal(button.veryLongPressTicks, 2000);
}

static void test_buttonEvent(void** state) {
    (void)state; /* unused */
    button_t button;
    buttonInit(&button, BUTTON_TYPE_NORMAL, 20, 400, 1000, 2000);

    buttonEvent(&button, BUTTON_PRESSED, 100);
    assert_int_equal(button.status, BUTTON_PRESSED);
    assert_int_equal(button.lastTick[BUTTON_PRESSED], 100);

    buttonEvent(&button, BUTTON_RELEASED, 150);
    assert_int_equal(button.status, BUTTON_RELEASED);
    assert_int_equal(button.lastTick[BUTTON_RELEASED], 150);
    assert_int_equal(button.pulses, 1);
}

static void test_buttonGetPress(void** state) {
    (void)state; /* unused */
    button_t button;
    buttonInit(&button, BUTTON_TYPE_NORMAL, 20, 400, 1000, 2000);

    /* De-bouncing check */
    buttonEvent(&button, BUTTON_PRESSED, 100);
    buttonEvent(&button, BUTTON_RELEASED, 119);
    buttonPressType_t press = buttonGetPress(&button, 200);
    assert_int_equal(press, BUTTON_NO_PRESS);

    /* Short press check */
    buttonEvent(&button, BUTTON_PRESSED, 210);
    buttonEvent(&button, BUTTON_RELEASED, 230);
    press = buttonGetPress(&button, 630);
    assert_int_equal(press, BUTTON_SHORT_PRESS);

    /* Short press with de-bouncing check */
    buttonEvent(&button, BUTTON_PRESSED, 700);
    buttonEvent(&button, BUTTON_RELEASED, 710);
    buttonEvent(&button, BUTTON_PRESSED, 715);
    buttonEvent(&button, BUTTON_RELEASED, 717);
    buttonEvent(&button, BUTTON_PRESSED, 720);
    buttonEvent(&button, BUTTON_RELEASED, 721);
    buttonEvent(&button, BUTTON_PRESSED, 722);
    buttonEvent(&button, BUTTON_PRESSED, 733);
    buttonEvent(&button, BUTTON_RELEASED, 750);
    press = buttonGetPress(&button, 1150);
    assert_int_equal(press, BUTTON_SHORT_PRESS);

    /* Double press check */
    buttonEvent(&button, BUTTON_PRESSED, 1160);
    buttonEvent(&button, BUTTON_RELEASED, 1210);
    buttonEvent(&button, BUTTON_PRESSED, 1300);
    buttonEvent(&button, BUTTON_RELEASED, 1350);
    press = buttonGetPress(&button, 1750);
    assert_int_equal(press, BUTTON_DOUBLE_PRESS);

    /* Double press with de-bouncing check */
    buttonEvent(&button, BUTTON_PRESSED, 1800);
    buttonEvent(&button, BUTTON_RELEASED, 1810);
    buttonEvent(&button, BUTTON_PRESSED, 1815);
    buttonEvent(&button, BUTTON_RELEASED, 1850);
    buttonEvent(&button, BUTTON_PRESSED, 1860);
    buttonEvent(&button, BUTTON_RELEASED, 1862);
    buttonEvent(&button, BUTTON_PRESSED, 1900);
    buttonEvent(&button, BUTTON_RELEASED, 1910);
    buttonEvent(&button, BUTTON_PRESSED, 1915);
    buttonEvent(&button, BUTTON_RELEASED, 1950);
    press = buttonGetPress(&button, 2360);
    assert_int_equal(press, BUTTON_DOUBLE_PRESS);

    /* Triple press check */
    buttonEvent(&button, BUTTON_PRESSED, 2400);
    buttonEvent(&button, BUTTON_RELEASED, 2450);
    buttonEvent(&button, BUTTON_PRESSED, 2500);
    buttonEvent(&button, BUTTON_RELEASED, 2550);
    buttonEvent(&button, BUTTON_PRESSED, 2600);
    buttonEvent(&button, BUTTON_RELEASED, 2650);
    press = buttonGetPress(&button, 3050);
    assert_int_equal(press, BUTTON_TRIPLE_PRESS);

    /* Multiple press check */
    buttonEvent(&button, BUTTON_PRESSED, 3200);
    buttonEvent(&button, BUTTON_RELEASED, 3250);
    buttonEvent(&button, BUTTON_PRESSED, 3300);
    buttonEvent(&button, BUTTON_RELEASED, 3350);
    buttonEvent(&button, BUTTON_PRESSED, 3400);
    buttonEvent(&button, BUTTON_RELEASED, 3450);
    buttonEvent(&button, BUTTON_PRESSED, 3500);
    buttonEvent(&button, BUTTON_RELEASED, 3550);
    press = buttonGetPress(&button, 3951);
    assert_int_equal(press, BUTTON_MULTIPLE_PRESS);

    /* Long press check */
    buttonEvent(&button, BUTTON_PRESSED, 3960);
    buttonEvent(&button, BUTTON_RELEASED, 4961);
    press = buttonGetPress(&button, 5405);
    assert_int_equal(press, BUTTON_LONG_PRESS);

    /* Very long press check */
    buttonEvent(&button, BUTTON_PRESSED, 5500);
    press = buttonGetPress(&button, 7501);
    assert_int_equal(press, BUTTON_VERYLONG_PRESS);

    /* Release press check */
    buttonEvent(&button, BUTTON_RELEASED, 7600);
    press = buttonGetPress(&button, 7501);
    assert_int_equal(press, BUTTON_RELEASE_PRESS);

    /* Very long press with de-bouncing check */
    buttonEvent(&button, BUTTON_PRESSED, 8400);
    buttonEvent(&button, BUTTON_RELEASED, 8419);
    buttonEvent(&button, BUTTON_PRESSED, 8425);
    buttonEvent(&button, BUTTON_RELEASED, 8430);
    buttonEvent(&button, BUTTON_PRESSED, 8435);
    buttonEvent(&button, BUTTON_RELEASED, 8437);
    buttonEvent(&button, BUTTON_PRESSED, 8440);
    press = buttonGetPress(&button, 10441);
    assert_int_equal(press, BUTTON_VERYLONG_PRESS);

    /* Check pulsating button type */
    button_t buttonP;
    buttonInit(&buttonP, BUTTON_TYPE_PULSATING, 20, 400, 1000, 2000);
    buttonEvent(&buttonP, BUTTON_PRESSED, 3200);
    buttonEvent(&buttonP, BUTTON_RELEASED, 3250);
    buttonEvent(&buttonP, BUTTON_PRESSED, 3300);
    buttonEvent(&buttonP, BUTTON_RELEASED, 3350);
    buttonEvent(&buttonP, BUTTON_PRESSED, 3400);
    buttonEvent(&buttonP, BUTTON_RELEASED, 3450);
    buttonEvent(&buttonP, BUTTON_PRESSED, 3500);
    buttonEvent(&buttonP, BUTTON_RELEASED, 3550);
    press = buttonGetPress(&buttonP, 3560);
    assert_int_equal(press, BUTTON_PULSATING_PRESS);
    press = buttonGetPress(&buttonP, 3970);
    assert_int_equal(press, BUTTON_RELEASE_PRESS);
}

static void test_buttonGetStatus(void** state) {
    (void)state; /* unused */
    button_t button;
    buttonInit(&button, BUTTON_TYPE_NORMAL, 20, 400, 1000, 2000);

    buttonEvent(&button, BUTTON_PRESSED, 100);
    assert_int_equal(buttonGetStatus(&button, 150), BUTTON_PRESSED);

    buttonEvent(&button, BUTTON_RELEASED, 200);
    assert_int_equal(buttonGetStatus(&button, 250), BUTTON_RELEASED);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_buttonInit),
        cmocka_unit_test(test_buttonEvent),
        cmocka_unit_test(test_buttonGetPress),
        cmocka_unit_test(test_buttonGetStatus),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
