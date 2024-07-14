/* BEGIN Header */
/**
 ******************************************************************************
 * \file            event.c
 * \author          Andrea Vivani
 * \brief           Implementation of event handler
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

/* Configuration check -------------------------------------------------------*/
#if !defined(ADVUTILS_USE_DYNAMIC_ALLOCATION) && !defined(ADVUTILS_USE_STATIC_ALLOCATION)
#error Either ADVUTILS_USE_DYNAMIC_ALLOCATION or ADVUTILS_USE_STATIC_ALLOCATION must be set for ADVUtils to work
#endif

/* Includes ------------------------------------------------------------------*/

#include "event.h"
#ifdef ADVUTILS_MEMORY_MGMT_HEADER
#if !defined(ADVUTILS_MALLOC) || !defined(ADVUTILS_CALLOC) || !defined(ADVUTILS_FREE)
#error ADVUTILS_MALLOC, ADVUTILS_CALLOC and ADVUTILS_FREE must be defined by the user!
#else
#include ADVUTILS_MEMORY_MGMT_HEADER
#endif /* !defined(ADVUTILS_MALLOC) || !defined(ADVUTILS_CALLOC) || !defined(ADVUTILS_FREE) */
#else
#include <stdlib.h>
#endif /* ADVUTILS_MEMORY_MGMT_HEADER */

/* Macros --------------------------------------------------------------------*/

#ifndef ADVUTILS_MEMORY_MGMT_HEADER
#define ADVUTILS_MALLOC malloc
#define ADVUTILS_CALLOC calloc
#define ADVUTILS_FREE   free
#endif /* ADVUTILS_MEMORY_MGMT_HEADER */

#ifndef ADVUTILS_ASSERT
#ifdef DEBUG
#define ADVUTILS_ASSERT(x)                                                                                             \
    if ((x) == 0) {                                                                                                    \
        for (;;)                                                                                                       \
            ;                                                                                                          \
    }
#else
#define ADVUTILS_ASSERT(x)
#endif /* DEBUG */
#endif /* ADVUTILS_ASSERT */

/* Functions -----------------------------------------------------------------*/

#ifdef ADVUTILS_USE_DYNAMIC_ALLOCATION

void eventInit(event_t* event, eventType_t type, uint16_t size) {
    event->type = type;
    event->count = 0;
    event->size = size;
    event->eventsList = ADVUTILS_CALLOC(size, sizeof(eventExtCBType_t));
    ADVUTILS_ASSERT(event->eventsList != NULL);
}

#endif /* ADVUTILS_USE_DYNAMIC_ALLOCATION */

#ifdef ADVUTILS_USE_STATIC_ALLOCATION

void eventInitStatic(event_t* event, eventExtCBType_t* data, eventType_t type, uint16_t size) {
    event->type = type;
    event->count = 0;
    event->size = size;
    event->eventsList = data;
}

#endif /* ADVUTILS_USE_STATIC_ALLOCATION */

utilsStatus_t eventRegister(event_t* event, eventCBType_t callback) {
    if (event->type == EVENT_TYPE_EXTENDED) {
        return UTILS_STATUS_ERROR;
    } else if (event->count == event->size) {
        return UTILS_STATUS_FULL;
    }
    event->eventsList[event->count] = (eventExtCBType_t)callback;
    event->count++;
    return UTILS_STATUS_SUCCESS;
}

utilsStatus_t eventRegisterEx(event_t* event, eventExtCBType_t callback) {
    if (event->type == EVENT_TYPE_BASIC) {
        return UTILS_STATUS_ERROR;
    } else if (event->count == event->size) {
        return UTILS_STATUS_FULL;
    }
    event->eventsList[event->count] = callback;
    event->count++;
    return UTILS_STATUS_SUCCESS;
}

utilsStatus_t eventDispatch(event_t* event) {
    if (event->type == EVENT_TYPE_EXTENDED) {
        return UTILS_STATUS_ERROR;
    }
    for (uint16_t ii = 0; ii < event->count; ii++) {
        ((eventCBType_t)event->eventsList[ii])();
    }
    return UTILS_STATUS_SUCCESS;
}

utilsStatus_t eventDispatchEx(event_t* event, void* val) {
    if (event->type == EVENT_TYPE_BASIC) {
        return UTILS_STATUS_ERROR;
    }
    for (uint16_t ii = 0; ii < event->count; ii++) {
        event->eventsList[ii](val);
    }
    return UTILS_STATUS_SUCCESS;
}
