/* BEGIN Header */
/**
 ******************************************************************************
 * \file    queue.c
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

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>
#include "queue.h"

/* Private  functions ---------------------------------------------------------*/

queueStatus_t queueInit(queue_t* queue, STYPE size)
{
    queue->data = NULL;
    queue->data = malloc(sizeof(QTYPE) * size);
    if (queue->data == NULL)
    {
        queue->size = 0;
        return QUEUE_ERROR;
    }
    
    queue->size = size;
    queue->items = 0;
    queue->_front = 0;
    queue->_rear = 0;
    return QUEUE_SUCCESS;
}

queueStatus_t queueEnqueue(queue_t* queue, QTYPE value)
{
    if (queue->items == queue->size)
    {
        return QUEUE_FULL;
    }
    
    if (queue->_rear >= queue->size)
    {
        queue->_rear = 0;
    }
    
    queue->data[queue->_rear] = value;
    queue->_rear++;
    queue->items++;
    
    return QUEUE_SUCCESS;
}

queueStatus_t queueBequeue(queue_t* queue, QTYPE value)
{
    if (queue->items == queue->size)
    {
        return QUEUE_FULL;
    }
    
    queue->_front = ((queue->_front == 0) ? (queue->size - 1) : (queue->_front - 1));
    queue->data[queue->_front] = value;
    queue->items++;
    
    return QUEUE_SUCCESS;
}

queueStatus_t queueDequeue(queue_t* queue, QTYPE* value)
{
    if(queue->items == 0)
    {
        return QUEUE_EMPTY;
    }
    
    *value = queue->data[queue->_front];
    queue->items--;
    
    if (queue->_front == queue->size - 1)
    {
        queue->_front = 0;
    }
    else 
    {
        queue->_front++;
    }
    
    return QUEUE_SUCCESS;
}

queueStatus_t queueDequeueRight(queue_t* queue, QTYPE* value)
{
    if(queue->items == 0)
    {
        return QUEUE_EMPTY;
    }
    
    queue->_rear = ((queue->_rear == 0) ? (queue->size - 1) : (queue->_rear - 1));
    *value = queue->data[queue->_rear];
    queue->items--;
    
    return QUEUE_SUCCESS;
    
}

queueStatus_t queuePeek(queue_t* queue, QTYPE* value)
{
    if(queue->items == 0)
    {
        return QUEUE_EMPTY;
    }
    
    *value = queue->data[queue->_front];
    
    return QUEUE_SUCCESS;
}

queueStatus_t queuePeekRight(queue_t* queue, QTYPE* value)
{
    if(queue->items == 0)
    {
        return QUEUE_EMPTY;
    }
    
    *value = queue->data[((queue->_rear == 0) ? (queue->size - 1) : (queue->_rear - 1))];
    
    return QUEUE_SUCCESS;
}


void queueInfo(queue_t* queue, STYPE* size, STYPE* items)
{
    *size = queue->size;
    *items = queue->items;
}

queueStatus_t queueDelete(queue_t* queue)
{
    
    if(queue->data == NULL)
    {
        return QUEUE_ERROR;
    }
    
    free(queue->data);
    
    return QUEUE_SUCCESS;
}
