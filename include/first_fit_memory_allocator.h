#ifndef FIRST_FIT_MEMORY_ALLOCATOR_H
#define FIRST_FIT_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

#include "free_list.h"
#include "memory_allocator.h"

// Implementation of a memory allocator that uses the first fit algorithm to allocate memory.
class FirstFitMemoryAllocator : public MemoryAllocator
{
public:

    // Constructor that takes in a reference to a memory buffer of template type T.
    template <class T>
    FirstFitMemoryAllocator(T& buffer) : 
        mem(buffer.data()),
        allocated_bytes(node_size),
        total_bytes(
            reinterpret_cast<std::uint8_t*>(buffer.end()) 
            - reinterpret_cast<std::uint8_t*>(buffer.begin())
            )
    {
        FLNode* start_node = reinterpret_cast<FLNode*>(mem);
        start_node->value = total_bytes - node_size;
        free_list.add_node(start_node);
    }

    // Allocate a number of bytes and return the address of the allocation.
    void* allocate(std::size_t bytes)
    {
        //placeholder
        return nullptr;
    }

    // Deallocate a block of memory to free it up for re-allocation.
    void deallocate(FLNode* addr)
    {

    }

    // Returns number of bytes allocated to memory buffer.
    std::size_t allocated() const
    {
        return allocated_bytes;
    }

    // Returns size of memory buffer in bytes.
    std::size_t length() const
    {
        return total_bytes;
    }

    // Size of free list node in bytes.
    static const std::size_t node_size = sizeof(FLNode);

private:

    // Pointer to memory buffer managed by this object.
    void* mem;

    // Free list to keep track of all unallocated blocks of memory.
    FreeList free_list;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

}; // class FirstFitMemoryAllocator

#endif // FIRST_FIT_MEMORY_ALLOCATOR_H