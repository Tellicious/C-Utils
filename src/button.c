/* BEGIN Header */
/**
 ******************************************************************************
 * \file    button.c
 * \author  Andrea Vivani
 * \brief   Implementation of button object with debouncing and multiple press 
 *          types detection
 ******************************************************************************
 * \copyright
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

/* Includes ------------------------------------------------------------------*/

#include "button.h"

/* Macros ---------------------------------------------------------------------*/

/* Private  functions ---------------------------------------------------------*/

void buttonInit(button_t *button, uint32_t longPressTicks)
{
    button->status = BUTTON_RELEASED;
    button->press = BUTTON_NO_PRESS;
    button->validUpTick = 0;
    button->validDownTick = 1;
    button->pulses = 0;
    button->event = 0;
    button->longPressTicks = longPressTicks;
}

void buttonEvent(button_t *button, buttonStatus_t status, uint32_t ticks)
{

    if (status == BUTTON_PRESSED)
    {
    	button->lastUpTick = ticks;
        if (((button->lastUpTick - button->lastDownTick) >= BUTTON_DEBOUNCE_TICKS) && (button->validUpTick >= button->validDownTick))
        {
        	button->validDownTick = button->lastDownTick;
            button->pulseUpWidth = button->validDownTick - button->validUpTick;
        }
    }
    else
    {
        button->lastDownTick = ticks;
        if (((button->lastDownTick - button->lastUpTick) >= BUTTON_DEBOUNCE_TICKS) && (button->validDownTick >= button->validUpTick))
        {
           	button->event = 1;
        	button->validUpTick = button->lastUpTick;
            button->pulseDownWidth = button->validUpTick - button->validDownTick;
            (button->pulseDownWidth > BUTTON_RESET_TICKS) ? (button->pulses = 1) : (button->pulses++);
        }

    }  
}

buttonPressType_t buttonGetPress(button_t *button, buttonStatus_t status, uint32_t ticks)
{
    // Update button status and pulses
    if ((status == BUTTON_PRESSED) && ((ticks - button->lastUpTick) > BUTTON_DEBOUNCE_TICKS))
    {
        if (button->validDownTick >= button->validUpTick)
        {
        	button->event = 1;
        	button->validUpTick = button->lastUpTick;
            button->pulseDownWidth = (button->validUpTick - button->validDownTick);
            (button->pulseDownWidth > BUTTON_RESET_TICKS) ? (button->pulses = 1) : (button->pulses++);
        }
    }
    else if ((status == BUTTON_RELEASED) && ((ticks - button->lastDownTick) > BUTTON_DEBOUNCE_TICKS))
    {
        if (button->validUpTick >= button->validDownTick)
        {
            button->validDownTick = button->lastDownTick;
            button->pulseUpWidth = button->validDownTick - button->validUpTick;
        }
    }

    button->press = BUTTON_NO_PRESS;
    button->status = (button->validUpTick > button->validDownTick) ? BUTTON_PRESSED : BUTTON_RELEASED;

    // Check button press type 
    if ((button->status == BUTTON_PRESSED) && ((ticks - button->validUpTick) > button->longPressTicks) && button->event)
    {
        button->press = BUTTON_LONG_PRESS;
        button->event = 0;
        button->pulses = 0;
    }
    else if ((button->status == BUTTON_RELEASED) && ((ticks - button->validDownTick) > BUTTON_RESET_TICKS) && button->event)
    {
    	switch (button->pulses)
    	{
    	case 1:
    		button->press = BUTTON_SHORT_PRESS;
    		break;
    	case 2:
    		button->press = BUTTON_DOUBLE_PRESS;
    		break;
    	case 3:
    		button->press = BUTTON_TRIPLE_PRESS;
    		break;
    	default:
    		button->press = BUTTON_NO_PRESS;
    		break;
    	}
    	button->event = 0;
    	button->pulses = 0;
    }

    return button->press;
}
