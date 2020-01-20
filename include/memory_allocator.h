#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>

#include "free_list.h"

// Abstract class (interface) for a memory allocator class.
class MemoryAllocator
{
public:

    // Type of free list node.
    using FLNode = FreeList::DLLNode;

    // Allocate a number of bytes and return the address of the allocation.
    virtual void* allocate(std::size_t bytes) = 0;

    // Deallocate a block of memory to free it up for re-allocation.
    virtual void deallocate(FLNode* addr) = 0;

};  // class MemoryAllocator

#endif // MEMORY_ALLOCATOR_H