/* BEGIN Header */
/**
 ******************************************************************************
 * \file    timer.h
 * \author  Andrea Vivani
 * \brief   Implementation of timer functions
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Typedefs ------------------------------------------------------------------*/

typedef volatile struct {
    uint32_t target_tick;
    uint32_t interval;
    uint8_t flag;
    uint16_t event_cnt;
} userTimer_t;

/* Function prototypes --------------------------------------------------------*/

uint32_t timerGetTimeMS();

/*!
 * \brief Setup timer object
 *
 * \param [in] t             pointer to timer object
 * \param [in] interval  	 required timer interval
 */
inline void timerInit(userTimer_t *t, uint32_t interval) {t->interval = interval; t->flag = 0; t->event_cnt = 0;};

/*!
 * \brief Start timer object
 *
 * \param [in] t             pointer to timer object
 * \param [in] currentTick   value of current tick
 */
inline void timerStart(userTimer_t *t, uint32_t currentTick) {t->target_tick = currentTick + t->interval; t->flag = 1;};

/*!
 * \brief Stop timer object
 *
 * \param [in] t 			 pointer to timer object
 */
inline void timerStop(userTimer_t *t) {t->flag = 0;};

/*!
 * \brief Reset timer object
 *
 * \param [in] t             pointer to timer object
 */
inline void timerClear(userTimer_t *t) {t->event_cnt = 0;};

/*!
 * \brief Process timer object
 *
 * \param [in] t             pointer to timer object
 * \param [in] currentTick   value of current tick
 *
 * \attention to be called on a regular basis to update timers
 */
void timerProcess(userTimer_t *t, uint32_t currentTick);

/*!
 * \brief Check if any timer event exists
 *
 * \param [in] t             pointer to timer object
 *
 * \return number of events for the specified timer
 *
 */
inline uint16_t timerEventExists(userTimer_t *t) {return t->event_cnt;};

#ifdef __cplusplus
}
#endif

#endif // __TIMER_H__
