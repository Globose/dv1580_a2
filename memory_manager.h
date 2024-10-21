#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief  Initializes the memory manager with a specified size of memory pool.
 * @param  size     Size of memory pool.
 * @note
 * - If size <0, no pool will be allocated. If size=0 an empty pool will be created.
 */
void mem_init(size_t size);

/**
 * @brief  Allocates a block of memory of the specified size.
 * @param  size Size of memory block.
 * @return void*    A pointer to the block.
 * @note
 * - If memory allocation fails it will return NULL
 */
void* mem_alloc(size_t size);

/**
 * @brief  Frees the specified block of memory
 * @param  block    Pointer to memory block.
 * @note
 * - Does nothing if block is not in the pool.
 */
void mem_free(void* block);

/**
 * @brief  Changes the size of the memory block, possibly moving it. 
 * @param  block    Pointer to memory block.
 * @return void*    A pointer to the block.
 * @note
 * - Does nothing if block is not in the pool.
 * - Returns NULL if not enough memory is available in the pool. While the memory block remains unchanged.
 */
void* mem_resize(void* block, size_t size);

/**
 * @brief  Frees up the memory pool
 */
void mem_deinit();

#endif // MEMORY_MANAGER_H
