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

    // Number of bytes allocated.
    virtual std::size_t allocated() const = 0;

    // Total number of available bytes in memory buffer.
    virtual std::size_t length() const = 0;

    // Deallocates all blocks and returns this object to it's initialisation state.
    virtual void reset() = 0;

};  // class MemoryAllocator

#endif // MEMORY_ALLOCATOR_H