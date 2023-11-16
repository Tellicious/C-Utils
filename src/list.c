/* BEGIN Header */
/**
 ******************************************************************************
 * \file    list.c
 * \author  Andrea Vivani
 * \brief   Implementation of linked list with dynamic memory allocation
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
#include <string.h>
#include "list.h"

/* Private  functions ---------------------------------------------------------*/

void listInit(list_t *list, size_t itemSize, LIST_STYPE size)
{
    list->_front = NULL;
    list->_rear = NULL;
    list->itemSize = itemSize;
    list->size = size;
    list->items = 0;
}

listStatus_t listPush(list_t *list, void *value)
{
    if (list->items >= list->size)
    {
        return LIST_FULL;
    }

    node_t *ptr;
    ptr = malloc(sizeof(node_t));
    ptr->data = calloc(1, list->itemSize);
    memcpy(ptr->data, value, list->itemSize);
    ptr->next = NULL;

    if ((list->_front == NULL) && (list->_rear == NULL))
    {
        list->_front = list->_rear = ptr;
    }
    else
    {
        list->_rear->next = ptr;
        list->_rear = ptr;
    }
    list->items++;
    return LIST_SUCCESS;
}

listStatus_t listPushFront(list_t *list, void *value)
{
    if (list->items >= list->size)
    {
        return LIST_FULL;
    }

    node_t *ptr;
    ptr = malloc(sizeof(node_t));
    ptr->data = calloc(1, list->itemSize);
    memcpy(ptr->data, value, list->itemSize);
    ptr->next = NULL;

    if ((list->_front == NULL) && (list->_rear == NULL))
    {
        list->_front = list->_rear = ptr;
    }
    else
    {
        ptr->next = list->_front;
        list->_front = ptr;
    }

    list->items++;
    return LIST_SUCCESS;
}

listStatus_t listInsert(list_t *list, void *value, LIST_STYPE position)
{
    LIST_STYPE ii;

    if (list->items >= list->size)
    {
        return LIST_FULL;
    }

    if (position > list->items) 
    {
        return LIST_ERROR;
    }

    node_t *ptr;
    ptr = malloc(sizeof(node_t));
    ptr->data = calloc(1, list->itemSize);
    memcpy(ptr->data, value, list->itemSize);
    ptr->next = NULL;

    if ((list->_front == NULL) && (list->_rear == NULL))
    {
        list->_front = list->_rear = ptr;
    } 
    else if (position == list->items)
    {
        list->_rear->next = ptr;
        list->_rear = ptr;
    }
    else 
    {   
        //search for node at position - 1
        node_t *prev = list->_front;
        for (ii = 1; ii < position; ii++)
        {
            prev = prev->next;
        } 
        ptr->next = prev->next;
        prev->next = ptr;
    }

    list->items++;
    return LIST_SUCCESS;
}

listStatus_t listUpdate(list_t *list, void *value, LIST_STYPE position)
{
    LIST_STYPE ii;

    if ((position + 1) > list->items)
    {
        return LIST_ERROR;
    }

    node_t *ptr = list->_front;

    //search for node at position
    for (ii = 0; ii < position; ii++)
    {
        ptr = ptr->next;
    }

    memcpy(ptr->data, value, list->itemSize);

    return LIST_SUCCESS;
}

listStatus_t listPop(list_t *list, void *value)
{
    if (!list->items)
    {
        return LIST_EMPTY;
    }

    node_t *ptr = list->_front;
    memcpy(value, ptr->data, list->itemSize);
    list->_front = ptr->next;
    free(ptr->data);
    free(ptr);

    list->items--;
    if(!list->items)
    {
        list->_rear = NULL;
    }
    return LIST_SUCCESS;
}

listStatus_t listPopBack(list_t *list, void *value)
{
    LIST_STYPE ii;

    if (!list->items)
    {
        return LIST_EMPTY;
    }
    
    memcpy(value, list->_rear->data, list->itemSize);

    //search for node at end - 1
    node_t *prev = list->_front;
    for (ii = 2; ii < list->items; ii++)
    {
        prev = prev->next;
    }
    prev->next = NULL;

    free(list->_rear->data);
    free(list->_rear);
    list->_rear = prev;

    list->items--;
    if(!list->items)
    {
        list->_front = list->_rear = NULL;
    }
    return LIST_SUCCESS;
}

listStatus_t listRemove(list_t *list, void *value, LIST_STYPE position)
{
    LIST_STYPE ii;

    if (!list->items)
    {
        return LIST_EMPTY;
    }

    if ((position + 1) > list->items)
    {
        return LIST_ERROR;
    }

    node_t *prev = NULL;
    node_t *ptr = list->_front;

    //search for node at position - 1 and node at position
    for (ii = 0; ii < position; ii++)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    memcpy(value, ptr->data, list->itemSize);
    
    if (position == 0)
    {
        list->_front = ptr->next;
    } 
    else if ((position + 1) == list->items)
    {
        prev->next = NULL;
        list->_rear = prev;
    }
    else
    {
        prev->next = ptr->next;
    }

    free(ptr->data);
    free(ptr);

    list->items--;
    if(!list->items)
    {
        list->_front = list->_rear = NULL;
    }
    return LIST_SUCCESS;
}

listStatus_t listPeek(list_t *list, void *value)
{
    if(list->items == 0)
    {
        return LIST_EMPTY;
    }
    
    memcpy(value, list->_front->data, list->itemSize);
    
    return LIST_SUCCESS;
}

listStatus_t listPeekBack(list_t *list, void *value)
{
    if(list->items == 0)
    {
        return LIST_EMPTY;
    }
    
    memcpy(value, list->_rear->data, list->itemSize);

    return LIST_SUCCESS;
}

listStatus_t listPeekAtPos(list_t *list, void *value, LIST_STYPE position)
{
    LIST_STYPE ii;

    if(list->items == 0)
    {
        return LIST_EMPTY;
    }

    if ((position + 1) > list->items)
    {
        return LIST_ERROR;
    }

    node_t *ptr = list->_front;

    //search for node at position
    for (ii = 0; ii < position; ii++)
    {
        ptr = ptr->next;
    }

    memcpy(value, ptr->data, list->itemSize);

    return LIST_SUCCESS;
}

listStatus_t listFlush(list_t *list)
{
    if (!list->items)
    {
        return LIST_EMPTY;
    }

    node_t *ptr = list->_front;
    while (ptr != NULL)
    {
        list->_front = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = list->_front;
    }
    list->items = 0;
    list->_rear = NULL;

    return LIST_SUCCESS;
}
