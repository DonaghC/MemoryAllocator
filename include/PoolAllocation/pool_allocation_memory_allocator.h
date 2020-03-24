#ifndef POOL_ALLOCATION_MEMORY_ALLOCATOR_H
#define POOL_ALLOCATION_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

#include "PoolAllocation/pool_allocation_free_list.h"
#include "memory_allocator.h"

// Implementation of a memory allocator that uses the pool allocation algorithm to allocate memory.
template<std::size_t block_size>
class PoolAllocationMemoryAllocator : public MemoryAllocator
{
public:

    // Type of free list node
    using FLNode = typename PoolAllocationFreeList<block_size>::SLLNode;

    // Constructor that takes in a reference to a memory buffer of template type T.
    template <class T>
    PoolAllocationMemoryAllocator(T& buffer) : 
        mem(buffer.data()),
        allocated_bytes(node_size),
        total_bytes(
            reinterpret_cast<std::uint8_t*>(buffer.end()) 
            - reinterpret_cast<std::uint8_t*>(buffer.begin())
            ),
        blocks_count(total_bytes / (node_size + block_size))
    {
        assert(block_size <= total_bytes);

        void* cursor = mem;
        
        FLNode* node = reinterpret_cast<FLNode*>(mem);
        fl.add_node(node);
        cursor += node_size + block_size;

        FLNode* prev_node = node;

        while (cursor + node_size + block_size <= buffer.end())
        {
            node = reinterpret_cast<FLNode*>(cursor);
            fl.add_node(node, prev_node);

            prev_node = node;
            cursor += node_size + block_size;
            allocated_bytes += node_size;
        }
    }

    // Allocate a single block and return the address of the allocation.
    void* allocate()
    {
        return allocate(block_size);
    }

    // Allocate a number of bytes and return the address of the allocation.
    void* allocate(std::size_t bytes)
    {
        if (bytes > 0 && bytes <= block_size && fl.count() > 0)
        {
            auto node = fl.remove_node(fl.head());
            allocated_bytes += block_size;
            blocks_allocated++;
            return reinterpret_cast<FLNode*>(reinterpret_cast<void*>(node) + node_size);
        }

        return nullptr;
    }

    // Deallocate a block of memory to free it up for re-allocation.
    void deallocate(void* addr)
    {
        void* newnode_addr = addr - node_size;
        FLNode* node = reinterpret_cast<FLNode*>(newnode_addr);
        fl.add_node(node);

        allocated_bytes -= block_size;
        blocks_allocated--;
    }

    // Deallocates all blocks and returns this object to it's initialisation state
    void reset()
    {
        fl.reset();
        allocated_bytes = node_size;
        blocks_allocated = 0;

        void* cursor = mem;
        
        FLNode* node = reinterpret_cast<FLNode*>(mem);
        fl.add_node(node);
        cursor += node_size + block_size;

        FLNode* prev_node = node;

        while (cursor + node_size + block_size <= mem + total_bytes)
        {
            node = reinterpret_cast<FLNode*>(cursor);
            fl.add_node(node, prev_node);

            prev_node = node;
            cursor += node_size + block_size;
            allocated_bytes += node_size;
        }
    }

    // Returns number of bytes allocated to memory buffer.
    std::size_t allocated() const
    {
        return allocated_bytes;
    }

    // Returns number of blocks allocated.
    std::size_t allocated_blocks() const
    {
        return blocks_allocated;
    }

    // Returns total number of blocks in memory buffer.
    std::size_t total_blocks() const
    {
        return blocks_count;
    }

    // Returns size of memory buffer in bytes.
    std::size_t length() const
    {
        return total_bytes;
    }

    // Returns the length of each block in bytes.
    static std::size_t block_length()
    {
        return block_size;
    }

    // Return free list.
    PoolAllocationFreeList<block_size> free_list() const
    {
        return fl;
    }

    // Size of free list node in bytes.
    static const std::size_t node_size = sizeof(FLNode);

private:

    // Pointer to memory buffer managed by this object.
    void* mem;

    // Free list to keep track of all unallocated blocks of memory.
    PoolAllocationFreeList<block_size> fl;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

    // Number of blocks allocated
    std::size_t blocks_allocated = 0;

    // Total number of blocks in memory buffer
    const std::size_t blocks_count;

}; // class PoolAllocationMemoryAllocator

#endif // POOL_ALLOCATION_MEMORY_ALLOCATOR_H