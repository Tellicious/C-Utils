/* BEGIN Header */
/**
 ******************************************************************************
 * @file    LKHashTable.c
 * @author  Andrea Vivani
 * @brief   Implementation of a simple dynamic linked hash-table
 ******************************************************************************
 * @copyright
 *
 * Copyright 202x Andrea Vivani
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

#include "LKHashTable.h"
#include <stdlib.h>
#include <string.h>
#include "hashFunctions.h"

/* Macros --------------------------------------------------------------------*/

#define LPHT_HASHFUN(x) hash_FNV1A(x)

/* Private  functions ---------------------------------------------------------*/

lkHashTableStatus_t lkHashTableInit(lkHashTable_t *lkht, size_t itemSize, uint32_t size)
{
    uint32_t ii;
    lkht->items = 0;
    lkht->size = size;
    lkht->itemSize = itemSize;

    lkht->entries = calloc(lkht->size, sizeof(list_t));
    if (lkht->entries == NULL)
    {
        return LKHT_ERROR;
    }

    for (ii = 0; ii < lkht->size; ii++)
    {
        listInit(&(lkht->entries[ii]), sizeof(lkHashTableEntry_t), UINT16_MAX);
    }

    return LKHT_SUCCESS;
}

lkHashTableStatus_t lkHashTablePut(lkHashTable_t *lkht, char *key, void *value)
{
    if ((value == NULL) || (key == NULL)) 
    {
        return LKHT_ERROR;
    }
    
    if (lkht->items == lkht->size)
    {
        return LKHT_FULL;
    }

    // limit hash to current memory size
    uint32_t ii = LPHT_HASHFUN(key) & (lkht->size - 1);

    lkHashTableEntry_t entry;

    // check if entry exists and update it
    LIST_STYPE idx;

    for (idx = 0; idx < lkht->entries[ii].items; idx++)
    {
        if (listPeekAtPos(&(lkht->entries[ii]), &entry, idx) == LIST_SUCCESS)
        {
            if (!strcmp(key, entry.key))
            {
                memcpy(entry.value, value, lkht->itemSize);
                return LKHT_SUCCESS;
            }
        }
    }
    
    // Set entry and update length
    entry.key = strdup(key);
    entry.value = calloc(1, lkht->itemSize);
    memcpy(entry.value, value, lkht->itemSize);

    if (listPush(&(lkht->entries[ii]), &entry) == LIST_SUCCESS)
    {
        lkht->items++;
        return LKHT_SUCCESS;
    }
    return LKHT_ERROR;
}

lkHashTableStatus_t lkHashTableGet(lkHashTable_t *lkht, char *key, void *value, lkHashTableRemoval_t remove)
{
    
    if (!lkht->items)
    {
        return LKHT_EMPTY;
    }
    
    uint32_t ii = LPHT_HASHFUN(key) & (lkht->size - 1);

    if(!lkht->entries[ii].items)
    {
        return LKHT_BUCKET_EMPTY;
    }

    lkHashTableEntry_t entry;

    // check if entry exists and return it
    LIST_STYPE idx;

    for (idx = 0; idx < lkht->entries[ii].items; idx++)
    {
        if (listPeekAtPos(&(lkht->entries[ii]), &entry, idx) == LIST_SUCCESS)
        {
            if (!strcmp(key, entry.key))
            {
                if (remove == LKHT_REMOVE_ITEM)
                {
                    listRemove(&(lkht->entries[ii]), &entry, idx);
                    lkht->items--;
                    memcpy(value, entry.value, lkht->itemSize);
                    free(entry.key);
                    free(entry.value);
                    return LKHT_SUCCESS;
                }
                memcpy(value, entry.value, lkht->itemSize);
                return LKHT_SUCCESS;
            }
        }
    }
    
    return LKHT_ERROR;
}

lkHashTableStatus_t lkHashTableFlush(lkHashTable_t *lkht)
{
    uint32_t ii;

    if (!lkht->items)
    {
        return LKHT_EMPTY;
    }

    lkHashTableEntry_t entry;
    LIST_STYPE idx;

    for (ii = 0; ii < lkht->size; ii++)
    {
         idx = lkht->entries[ii].items;

        while(idx--)
        {
            listPop(&(lkht->entries[ii]), &entry);
            free(entry.key);
            free(entry.value);
        }
    }

    free(lkht->entries);
    lkht->entries = NULL;
    lkht->items = 0;

    return LKHT_SUCCESS;
}
