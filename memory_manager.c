#include "memory_manager.h"

struct MemoryBlock{
    void *ptr;
    size_t size;
    int free;
    struct MemoryBlock* next;
    struct MemoryBlock* prev;
};

static void * m_block;
static size_t m_size;
static struct MemoryBlock * first_block;

/**
 * @brief  Creates a new memory block and metadata for it
 * @param  ptr  Pointer to the first memory block.
 * @param  block2   Pointer to the second memory block.
 * @return MemoryBlock*     Pointer to the new memory block.
 * @note    - Only merges if both memory blocks are free.
 */
struct MemoryBlock* add_mem(void* ptr, size_t size, int free, struct MemoryBlock* prev, struct MemoryBlock* next){
    if (size == 0) return next;
    struct MemoryBlock* new_block = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
    *new_block = (struct MemoryBlock){ptr,size,free,next,prev};
    prev ? prev->next = new_block: 0;
    next ? next->prev = new_block : 0;
    return new_block;
}

/**
 * @brief  Splits one memory block into two
 * @param  current_block    Pointer to the memory block.
 * @param  size     Size of the first part of the memory block.
 */
void split_mem(struct MemoryBlock* current_block, size_t size) {
    size_t new_size = current_block->size-size;
    current_block->free = 0;
    current_block->size = size;
    void * next_ptr = (char*)current_block->ptr + current_block->size;
    current_block->next = add_mem(next_ptr, new_size, 1, current_block, current_block->next);
}

/**
 * @brief  Attempts to merge two memory blocks
 * @param  block1   Pointer to the first memory block.
 * @param  block2   Pointer to the second memory block.
 * @note    - Only merges if both memory blocks are free.
 */
void merge_mem(struct MemoryBlock* block1, struct MemoryBlock* block2){
    if (block1 == NULL || block2 == NULL) return;
    if (block1->free == 0 || block2->free == 0) return;
    block1->next = block2->next;
    block1->size = block1->size+block2->size;
    if (block2->next != NULL) block2->next->prev = block1;
    free(block2);
}

/**
 * @brief  Searches for a matching memory block
 * @param  block     Pointer to the memory block ptr.
 * @return MemoryBlock*     Pointer to the memory block.
 * @note    - Returns NULL if no matching block is found.
 */

struct MemoryBlock* get_block(void* block){
    struct MemoryBlock * current_block = first_block;
    while (current_block != NULL && current_block->ptr != block){
        current_block = current_block->next;
    }
    if (current_block->ptr != block || current_block == NULL) return NULL;
    return current_block;
}

/**
 * @brief  Initializes the memory manager with a specified size of memory pool.
 * @param  size     Size of memory pool.
 * @note
 * - If size <0, no pool will be allocated. If size=0 an empty pool will be created.
 */
void mem_init(size_t size){
    if (size < 0) return;
    m_block = malloc(size);
    m_size = size;
    first_block = add_mem(m_block, m_size, 1, NULL, NULL);
}

/**
 * @brief  Allocates a block of memory of the specified size.
 * @param  size Size of memory block.
 * @return void*    A pointer to the block.
 * @note
 * - If memory allocation fails it will return NULL
 */
void* mem_alloc(size_t size){
    size = (size+7)&~7;
    struct MemoryBlock* current_block = first_block;
    while (current_block != NULL && (current_block->free != 1 || current_block->size < size)){
        current_block = current_block->next;
    }
    if (current_block == NULL) return NULL;
    split_mem(current_block, size);
    return current_block->ptr;
}

/**
 * @brief  Frees the specified block of memory
 * @param  block    Pointer to memory block.
 * @note
 * - Does nothing if block is not in the pool.
 */
void mem_free(void* block){
    struct MemoryBlock * current_block = get_block(block);
    if (current_block == NULL) return;
    current_block->free = 1;
    merge_mem(current_block, current_block->next);
    merge_mem(current_block->prev, current_block);
}

/**
 * @brief  Changes the size of the memory block, possibly moving it. 
 * @param  block    Pointer to memory block.
 * @return void*    A pointer to the block.
 * @note
 * - Does nothing if block is not in the pool.
 * - Returns NULL if not enough memory is available in the pool. While the memory block remains unchanged.
 */

void* mem_resize(void* block, size_t size){
    size = (size+7)&~7;
    struct MemoryBlock* current_block = get_block(block);
    if (current_block == NULL) return NULL;
    
    size_t old_size = current_block->size;
    current_block->free = 1;
    merge_mem(current_block, current_block->next);
    
    if (current_block->size >= size){
        split_mem(current_block, size);
        return current_block->ptr;   
    }
    else{
        void* ptr = mem_alloc(size);
        if (ptr == NULL){
            current_block->free = 0;
            return NULL;
        } 
        memcpy(ptr, current_block->ptr, old_size);
        return ptr;
    }
}


/**
 * @brief  Frees up the memory pool
 */
void mem_deinit(){
    struct MemoryBlock* current_block = first_block;
    while (current_block != NULL){
        struct MemoryBlock* ptr = current_block->next;
        free(current_block);
        current_block = ptr;
    }
    free(m_block);
    first_block = NULL;
}

