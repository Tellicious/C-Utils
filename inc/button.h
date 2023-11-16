/* BEGIN Header */
/**
 ******************************************************************************
 * @file    button.h
 * @author  Andrea Vivani
 * @brief   Implementation of button object with debouncing and multiple press 
 *          types detection
 ******************************************************************************
 * @copyright
 *
 * Copyright 2023 Andrea Vivani
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Macros --------------------------------------------------------------------*/

#define BUTTON_RESET_TICKS 400

/* Typedefs ------------------------------------------------------------------*/

/*!
 * Button status
 */
typedef enum
{
    BUTTON_RELEASED = 00,
    BUTTON_PRESSED = 01,
} buttonStatus_t;

/*!
 * Button press type
 */
typedef enum
{
    BUTTON_NO_PRESS = 00,
    BUTTON_SHORT_PRESS = 01,
    BUTTON_DOUBLE_PRESS = 02,
    BUTTON_TRIPLE_PRESS = 03,
    BUTTON_LONG_PRESS = 04,
} buttonPressType_t;

/*!
 * Button struct
 */
typedef struct
{
    buttonStatus_t status;
    buttonPressType_t press;
    uint32_t validTick[2], lastTick[2], debounceTicks, longPressTicks;
    uint8_t pulses, event;
} button_t;

/* Function prototypes --------------------------------------------------------*/

/*!
 * @brief Init button structure
 *
 * @param[in] button           pointer to button object
 * @param[in] debounceTicks    number of ticks used for debouncing. Use 20ms as a starting point
 * @param[in] longPressTicks   number of ticks that the button needs to be pressed to detect long-press
 */
void buttonInit(button_t *button, uint32_t debounceTicks, uint32_t longPressTicks);

/*!
 * @brief Button event update, to be called either in an EXT interrupt or in a timer
 *
 * @param[in] button           pointer to button object
 * @param[in] status           current button status
 * @param[in] ticks            current system ticks
 */
void buttonEvent(button_t *button, buttonStatus_t status, uint32_t ticks);

/*!
 * @brief Get button status, to be called in the main loop
 *
 * @param[in] button           pointer to button object
 * @param[in] ticks            current system ticks
 * 
 * @return BUTTON_NO_PRESS, BUTTON_SHORT_PRESS, BUTTON_DOUBLE_PRESS, BUTTON_TRIPLE_PRESS, BUTTON_LONG_PRESS according to current button press type
 */
buttonPressType_t buttonGetPress(button_t *button, uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif // __BUTTON_H__
