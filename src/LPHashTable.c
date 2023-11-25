/* BEGIN Header */
/**
 ******************************************************************************
 * @file    LPHashTable.c
 * @author  Andrea Vivani
 * @brief   Implementation of a simple linear probing hash-table
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

/* Includes ------------------------------------------------------------------*/

#include "LPHashTable.h"
#include <stdlib.h>
#include <string.h>
#include "hashFunctions.h"

/* Macros --------------------------------------------------------------------*/

#define LPHT_HASHFUN(x) hash_FNV1A(x)

/* Function prototypes --------------------------------------------------------*/

static lpHashTableStatus_t lpHashTableSetEntry(lpHashTable_t *lpht, char *key, void *value);
static lpHashTableStatus_t lpHashTableXpand(lpHashTable_t *lpht, uint8_t increase);

/* Private  functions ---------------------------------------------------------*/

lpHashTableStatus_t lpHashTableInit(lpHashTable_t *lpht, size_t itemSize, uint32_t init_items, lpHashTableResizable_t resizable)
{
    lpht->items = 0;
    lpht->size = init_items;
    lpht->itemSize = itemSize;
    lpht->resizable = resizable;

    lpht->entries = calloc(lpht->size, sizeof(lpHashTableEntry_t));
    if (lpht->entries == NULL)
    {
        return LPHT_ERROR;
    }

    return LPHT_SUCCESS;
}

lpHashTableStatus_t lpHashTablePut(lpHashTable_t *lpht, char *key, void *value)
{
    if ((value == NULL) || (key == NULL)) 
    {
        return LPHT_ERROR;
    }
    
    // If length will exceed half of current capacity, expand it.
    if ((lpht->items >= (lpht->size * LPHT_MAX_SATURATION)) && (lpht->resizable == LPHT_RESIZABLE))
    {
        if (lpHashTableXpand(lpht, 1) != LPHT_SUCCESS)
        {
            return LPHT_ERROR;
        }
    }
    else if (lpht->items == lpht->size)
    {
        return LPHT_FULL;
    }
    else if ((lpht->items <= (lpht->size * LPHT_MIN_SATURATION)) && (lpht->size > LPHT_MIN_SIZE) && (lpht->resizable == LPHT_RESIZABLE))
    {
        if (lpHashTableXpand(lpht, 0) != LPHT_SUCCESS)
        {
            return LPHT_ERROR;
        }
    }

    // Set entry and update length.

    if (lpHashTableSetEntry(lpht, key, value) == LPHT_SUCCESS)
    {
        return LPHT_SUCCESS;
    }

    return LPHT_ERROR;
}

lpHashTableStatus_t lpHashTableGet(lpHashTable_t *lpht, char *key, void *value, lpHashTableRemoval_t remove)
{
    uint32_t ii = LPHT_HASHFUN(key) & (lpht->size - 1);

    while (lpht->entries[ii].key != NULL)
    {
        if (!strcmp(key, lpht->entries[ii].key))
        {
            memcpy(value, lpht->entries[ii].value, lpht->itemSize);

            if (remove == LPHT_REMOVE_ITEM)
            {
                free(lpht->entries[ii].key);
                free(lpht->entries[ii].value);
                lpht->entries[ii].key = NULL;
                lpht->entries[ii].value = NULL;
                lpht->items--;
            }

            return LPHT_SUCCESS;
        }

        ii++;

        if (ii >= lpht->size)
        {
            ii = 0;
        }
    }
    return LPHT_ERROR;
}

lpHashTableStatus_t lpHashTableFlush(lpHashTable_t *lpht)
{
    uint32_t ii;

    if (!lpht->items)
    {
        return LPHT_EMPTY;
    }

    for (ii = 0; ii < lpht->size; ii++)
    {
        free(lpht->entries[ii].key);
        free(lpht->entries[ii].value);
        lpht->entries[ii].key = NULL;
        lpht->entries[ii].value = NULL;
    }

    lpht->items = 0;

    return LPHT_SUCCESS;
}

static lpHashTableStatus_t lpHashTableSetEntry(lpHashTable_t *lpht, char *key, void *value)
{
    // limit hash to current memory size
    uint32_t ii = LPHT_HASHFUN(key) & (lpht->size - 1);

    // Loop till we find an empty entry.
    while (lpht->entries[ii].key != NULL)
    {
        if (!strcmp(key, lpht->entries[ii].key))
        {
            // Found key (it already exists), update value.
            memcpy(lpht->entries[ii].value, value, lpht->itemSize);
            return LPHT_SUCCESS;
        }
        ii++;
        if (ii >= lpht->size)
        {
            ii = 0;
        }
    }
    
    // Didn't find key, allocate+copy if needed, then insert it.
    lpht->entries[ii].key = strdup(key);
    lpht->entries[ii].value = calloc(1, lpht->itemSize);

    if ((lpht->entries[ii].key == NULL) || (lpht->entries[ii].value == NULL))
    {
        free(lpht->entries[ii].key);
        free(lpht->entries[ii].value);
        return LPHT_ERROR;
    }

    memcpy(lpht->entries[ii].value, value, lpht->itemSize);
    
    lpht->items++;

    return LPHT_SUCCESS;
}

static lpHashTableStatus_t lpHashTableXpand(lpHashTable_t *lpht, uint8_t increase)
{
    // Allocate new entries array.
    uint32_t ii;
    uint32_t old_size = lpht->size;
    
    // hash is currently a uint32_t so Hash-table size should not exceed that
    if (increase)
    {
        // increase table size
        if (lpht->size == UINT32_MAX)
        {
            return LPHT_ERROR;
        }
        else if (lpht->size >= (UINT32_MAX >> 1))
        {
            lpht->size = UINT32_MAX;
        }
        else
        {
            lpht->size *= 2;
        }
    }
    else
    {
        // decrease table size
        if (lpht->size >= (LPHT_MIN_SIZE * 2))
        {
            lpht->size = (lpht->size >> 1);
        }
        else
        {
            lpht->size = LPHT_MIN_SIZE;
        }
    }

    lpHashTableEntry_t *old_entries = lpht->entries;
    lpht->entries = calloc(lpht->size, sizeof(lpHashTableEntry_t));
    if (lpht->entries == NULL)
    {
        lpht->size = old_size;
        lpht->entries = old_entries;
        return LPHT_ERROR;
    }

    // Iterate entries, move all non-empty ones to new table's entries.
    lpht->items = 0;

    for (ii = 0; ii < old_size; ii++)
    {
        if (old_entries[ii].key != NULL)
        {
            lpHashTableSetEntry(lpht, old_entries[ii].key, old_entries[ii].value);
            free(old_entries[ii].key);
            free(old_entries[ii].value);
        }
    }

    free(old_entries);

    return LPHT_SUCCESS;
}

void lpHashTableIt(lpHashTableIterator_t *it, lpHashTable_t *lpht)
{
    it->_lpht = lpht;
    it->_index = 0;
    return;
}

lpHashTableStatus_t lpHashTableItNext(lpHashTableIterator_t *it)
{
    while (it->_index < it->_lpht->size)
    {
        if (it->_lpht->entries[it->_index].key != NULL)
        {
            // Found next non-empty item, update iterator key and value.
            lpHashTableEntry_t entry = it->_lpht->entries[it->_index];
            it->key = entry.key;
            it->value = entry.value;

            return LPHT_SUCCESS;
        }
        it->_index++;
    }
    return LPHT_ERROR;
}
