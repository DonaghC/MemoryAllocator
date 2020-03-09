#ifndef POOL_ALLOCATION_MEMORY_ALLOCATOR_H
#define POOL_ALLOCATION_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

#include "PoolAllocation/free_list.h"
#include "memory_allocator.h"

template<std::size_t block_size>
class PoolAllocationMemoryAllocator : public MemoryAllocator
{

public:

    // Type of free list node
    using FLNode = FreeList::SLLNode;

    // Constructor that takes in a reference to a memory buffer of template type T.
    template <class T>
    PoolAllocationMemoryAllocator(T& buffer) : 
        mem(buffer.data()),
        allocated_bytes(node_size),
        total_bytes(
            reinterpret_cast<std::uint8_t*>(buffer.end()) 
            - reinterpret_cast<std::uint8_t*>(buffer.begin())
            ),
        total_blocks(total_bytes / (node_size + block_size))
    {
        void* cursor = mem;
        
        FLNode* node = reinterpret_cast<FLNode*>(mem);
        fl.add_node(mem);

        FLNode* prev_node = node;

        while (cursor + node_size + block_size <= buffer.end())
        {
            cursor = mem + node_size + block_size;
            node = reinterpret_cast<FLNode*>(cursor);
            fl.add_node(node, prev_node);

            prev_node = node;
        }
    }

    void* allocate(std::size_t bytes)
    {
        if (bytes <= block_size)
        {
            fl.remove_node(fl.head_node());
            allocated_bytes += bytes;
            allocated_blocks++;
            return reinterpret_cast<FLNode*>(reinterpret_cast<void*>(node) + node_size);
        }

        return nullptr;
    }

    void deallocate(void* addr)
    {
        void* newnode_addr = addr - node_size;
        FLNode* node = reinterpret_cast<FLNode*>(newnode_addr);
        fl.add_node(node);

        allocated_bytes -= node->value;
        allocated_blocks--;
    }

    // Returns number of bytes allocated to memory buffer.
    std::size_t allocated() const
    {
        return allocated_bytes;
    }

    // Returns number of blocks allocated.
    std::size_t allocated_blocks() const
    {
        return allocated_blocks;
    }

    // Returns total number of blocks in memory buffer.
    std::size_t total_blocks() const
    {
        return total_blocks;
    }

    // Returns size of memory buffer in bytes.
    std::size_t length() const
    {
        return total_bytes;
    }

    // Returns the length of each block in bytes.
    static const std::size_t block_length() const
    {
        return block_size;
    }

    // Return free list.
    FreeList free_list() const
    {
        return fl;
    }

    // Size of free list node in bytes.
    static const std::size_t node_size = sizeof(FLNode);

private:

    // Pointer to memory buffer managed by this object.
    void* mem;

    // Free list to keep track of all unallocated blocks of memory.
    FreeList<block_size> fl;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

    // Number of blocks allocated
    std::size_t allocated_blocks;

    // Total number of blocks in memory buffer
    const std::size_t total_blocks;

}; // class PoolAllocationMemoryAllocator

#endif // POOL_ALLOCATION_MEMORY_ALLOCATOR_H