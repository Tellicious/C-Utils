/* BEGIN Header */
/**
 ******************************************************************************
 * @file    queue.h
 * @author  Andrea Vivani
 * @brief   Implementation of queue structure
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
#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stddef.h>

/* Macros --------------------------------------------------------------------*/

#define QUEUE_STYPE uint16_t

/* Typedefs ------------------------------------------------------------------*/

/*!
 * Queue struct
 */
typedef struct
{
    uint8_t *data;
    uint8_t itemSize;
    QUEUE_STYPE size, _front, _rear, items;
} queue_t;

/*!
 * Queue return status
 */
typedef enum
{
    QUEUE_SUCCESS = 0,
    QUEUE_ERROR = 1,
    QUEUE_EMPTY = 2,
    QUEUE_FULL = 3,
    QUEUE_NOT_ENOUGH_ITEMS = 4,
    QUEUE_NOT_ENOUGH_SPACE = 5
} queueStatus_t;

/* Function prototypes --------------------------------------------------------*/

/*!
 * @brief Init queue structure
 *
 * @param[in] queue        pointer to queue object
 * @param[in] itemSize     size in bytes of each item in the queue
 * @param[in] size         required queue size (number of objects)
 *
 * @return QUEUE_SUCCESS if queue is initialized, QUEUE_ERROR if data was not allocated correctly
 */
queueStatus_t queueInit(queue_t *queue, size_t itemSize, QUEUE_STYPE size);

/*!
 * @brief Add data to end of queue
 *
 * @param[in] queue        pointer to queue object
 * @param[in] value        pointer to value to be pushed
 *
 * @return QUEUE_SUCCESS if data can be pushed correctly, QUEUE_FULL if queue is full
 */
queueStatus_t queuePush(queue_t *queue, void *value);

/*!
 * @brief Add an array of data to end of queue
 *
 * @param[in] queue        pointer to queue object
 * @param[in] data         pointer to array of data to be pushed
 * @param[in] num          number of values to be pushed
 *
 * @return QUEUE_SUCCESS if data can be pushed correctly, QUEUE_NOT_ENOUGH_SPACE if queue cannot hold all elements
 */
queueStatus_t queuePushArr(queue_t *queue, void *data, QUEUE_STYPE num);

/*!
 * @brief Add data to beginning of queue
 *
 * @param[in] queue        pointer to queue object
 * @param[in] value        pointer to value to be pushed to front
 *
 * @return QUEUE_SUCCESS if data can be pushed to front correctly, QUEUE_FULL if queue is full
 */
queueStatus_t queuePushFront(queue_t *queue, void *value);

/*!
 * @brief Add an array of data to beginning of queue
 *
 * @param[in] queue        pointer to queue object
 * @param[in] data         pointer to array of data to be pushed to front
 * @param[in] num          number of values to be pushed to front
 *
 * @return QUEUE_SUCCESS if data can be pushed to front correctly, QUEUE_NOT_ENOUGH_SPACE if queue cannot hold all elements
 */
queueStatus_t queuePushFrontArr(queue_t *queue, void *data, QUEUE_STYPE num);

/*!
 * @brief Read data from beginning of queue, removing it
 *
 * @param[in] queue        pointer to queue object
 * @param[out] value       pointer to value to be read and removed
 *
 * @return QUEUE_SUCCESS if data can be read and removed correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePop(queue_t *queue, void *value);

/*!
 * @brief Read an array of data from beginning of queue, removing it
 *
 * @param[in] queue        pointer to queue object
 * @param[out] data        pointer to array of data to be read and removed
 * @param[in] num          number of values to be read and removed
 *
 * @return QUEUE_SUCCESS if data can be pushed to front correctly, QUEUE_NOT_ENOUGH_ITEMS if queue doesn't hold all requested items
 */
queueStatus_t queuePopArr(queue_t *queue, void *data, QUEUE_STYPE num);

/*!
 * @brief Read data from end of queue, removing it
 *
 * @param[in] queue        pointer to queue object
 * @param[out] value       pointer to value to be read and removed
 *
 * @return QUEUE_SUCCESS if data can be read and removed correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePopBack(queue_t *queue, void *value);

/*!
 * @brief Read data from beginning of queue, without removing it
 *
 * @param[in] queue        pointer to queue object
 * @param[out] value       pointer to value to be read
 *
 * @return QUEUE_SUCCESS if data can be read correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePeek(queue_t *queue, void *value);

/*!
 * @brief Read data from end of queue, without removing it
 *
 * @param[in] queue        pointer to queue object
 * @param[out] value       pointer to value to be read
 *
 * @return QUEUE_SUCCESS if data can be read correctly, QUEUE_EMPTY if queue is empty
 */
queueStatus_t queuePeekBack(queue_t *queue, void *value);

/*!
 * @brief Returns queue info
 *
 * @param[in] queue        pointer to queue object
 * @param[out] size        pointer to size
 * @param[out] items       pointer to number of items currently queued
 */
static inline void queueInfo(queue_t *queue, QUEUE_STYPE *size, QUEUE_STYPE *items) {*size = queue->size / queue->itemSize; *items = queue->items / queue->itemSize;};

/*!
 * @brief Flush queue setting all values to 0
 *
 * @param[in] queue        pointer to queue object
 *
 * @return QUEUE_SUCCESS if queue is flushed correctly, QUEUE_ERROR if data cannot be flushed
 */
queueStatus_t queueFlush(queue_t *queue);

/*!
 * @brief Delete queue
 *
 * @param[in] queue        pointer to queue object
 *
 * @return QUEUE_SUCCESS if queue data is deleted, QUEUE_ERROR if data was not deleted correctly
 */
queueStatus_t queueDelete(queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif // __QUEUE_H__

