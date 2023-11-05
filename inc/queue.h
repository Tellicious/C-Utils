/* BEGIN Header */
/**
 ******************************************************************************
 * \file    queue.h
 * \author  Andrea Vivani
 * \brief   Implementation of queue structure
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
#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Macros --------------------------------------------------------------------*/

#define QTYPE uint8_t
#define STYPE uint16_t

/* Typedefs ------------------------------------------------------------------*/

/*!
 * Queue struct
 */
typedef struct
{
    QTYPE *data;
    STYPE size, _front, _rear, items;
} queue_t;

/*!
 * Queue return status
 */
typedef enum
{
    QUEUE_SUCCESS = 00,
    QUEUE_ERROR = 01,
    QUEUE_EMPTY = 02,
    QUEUE_FULL = 03,
    QUEUE_NOT_ENOUGH_ITEMS = 04,
    QUEUE_NOT_ENOUGH_SPACE = 05
} queueStatus_t;

/* Function prototypes --------------------------------------------------------*/
/*!
 * \brief Init queue structure
 *
 * \param [in] queue        pointer to queue object
 * \param [in] size         required queue size
 *
 * \return QUEUE_SUCCESS if queue is initialized, QUEUE_ERROR if data was not allocated correctly
 */
queueStatus_t queueInit(queue_t *queue, STYPE size);

/*!
 * \brief Add data to end of queue
 *
 * \param [in] queue        pointer to queue object
 * \param [in] value        value to be enqueued
 *
 * \return QUEUE_SUCCESS if data can be enqueued correctly, QUEUE_FULL if queue is full
 */
queueStatus_t queueEnqueue(queue_t *queue, QTYPE value);

/*!
 * \brief Add an array of data to end of queue
 *
 * \param [in] queue        pointer to queue object
 * \param [in] data         pointer to array of data to be enqueued
 * \param [in] num          number of values to be enqueued
 *
 * \return QUEUE_SUCCESS if data can be enqueued correctly, QUEUE_NOT_ENOUGH_SPACE if queue cannot hold all elements
 */
queueStatus_t queueEnqueueArr(queue_t *queue, QTYPE *data, STYPE num);

/*!
 * \brief Add data to beginning of queue
 *
 * \param [in] queue        pointer to queue object
 * \param [in] value        value to be bequeued
 *
 * \return QUEUE_SUCCESS if data can be bequeued correctly, QUEUE_FULL if queue is full
 */
queueStatus_t queueBequeue(queue_t *queue, QTYPE value);

/*!
 * \brief Add an array of data to beginning of queue
 *
 * \param [in] queue        pointer to queue object
 * \param [in] data         pointer to array of data to be bequeued
 * \param [in] num          number of values to be bequeued
 *
 * \return QUEUE_SUCCESS if data can be bequeued correctly, QUEUE_NOT_ENOUGH_SPACE if queue cannot hold all elements
 */
queueStatus_t queueBequeueArr(queue_t *queue, QTYPE *data, STYPE num);

/*!
 * \brief Read data from beginning of queue, removing it
 *
 * \param [in] queue        pointer to queue object
 * \param [out] value       pointer to value to be dequeued
 *
 * \return QUEUE_SUCCESS if data can be dequeued correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queueDequeue(queue_t *queue, QTYPE *value);

/*!
 * \brief Read an array of data from beginning of queue, removing it
 *
 * \param [in] queue        pointer to queue object
 * \param [out] data        pointer to array of data to be dequeued
 * \param [in] num          number of values to be dequeued
 *
 * \return QUEUE_SUCCESS if data can be bequeued correctly, QUEUE_NOT_ENOUGH_ITEMS if queue doesn't hold all requested items
 */
queueStatus_t queueDequeueArr(queue_t *queue, QTYPE *data, STYPE num);

/*!
 * \brief Read data from end of queue, removing it
 *
 * \param [in] queue        pointer to queue object
 * \param [out] value       pointer to value to be dequeued
 *
 * \return QUEUE_SUCCESS if data can be dequeued correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queueDequeueRight(queue_t *queue, QTYPE *value);

/*!
 * \brief Read data from beginning of queue, without removing it
 *
 * \param [in] queue        pointer to queue object
 * \param [out] value      pointer to value to be read
 *
 * \return QUEUE_SUCCESS if data can be read correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePeek(queue_t *queue, QTYPE *value);

/*!
 * \brief Read data from end of queue, without removing it
 *
 * \param [in] queue        pointer to queue object
 * \param [out] value       pointer to value to be read
 *
 * \return QUEUE_SUCCESS if data can be read correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePeekRight(queue_t *queue, QTYPE *value);

/*!
 * \brief Returns queue info
 *
 * \param [in] queue        pointer to queue object
 * \param [out] size        pointer to size
 * \param [out] items       pointer to number of items currently queued
 */
void queueInfo(queue_t *queue, STYPE *size, STYPE *items);

/*!
 * \brief Flush queue setting all values to 0
 *
 * \param [in] queue        pointer to queue object
 *
 * \return QUEUE_SUCCESS if queue is flushed correctly, QUEUE_ERROR if data cannot be flushed
 */
queueStatus_t queueFlush(queue_t *queue);

/*!
 * \brief Delete queue
 *
 * \param [in] queue        pointer to queue object
 *
 * \return QUEUE_SUCCESS if queue data is deleted, QUEUE_ERROR if data was not deleted correctly
 */
queueStatus_t queueDelete(queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif // __QUEUE_H__

