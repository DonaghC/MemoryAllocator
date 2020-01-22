#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>

// Abstract class (interface) for a memory allocator class.
class MemoryAllocator
{
public:

    // Allocate a number of bytes and return the address of the allocation.
    virtual void* allocate(std::size_t bytes) = 0;

    // Deallocate a block of memory to free it up for re-allocation.
    virtual void deallocate(void* addr) = 0;

};  // class MemoryAllocator

#endif // MEMORY_ALLOCATOR_H