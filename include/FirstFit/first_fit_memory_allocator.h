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

    // Type of free list node;
    using FLNode = FreeList::DLLNode;

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
        fl.add_node(start_node);
    }

    // Allocate a number of bytes and return the address of the allocation.
    void* allocate(std::size_t bytes)
    {
        if (bytes > 0)
        {
            FLNode* cursor = fl.head();
            while (cursor != nullptr)
            {
                if (cursor->value < bytes)
                {
                    cursor = cursor->next;
                }
                else if (cursor->value >= bytes + node_size)
                {
                    fl.remove_node(cursor);
                    allocated_bytes += bytes + node_size;

                    void* curr_node_addr = reinterpret_cast<void*>(cursor);
                    FLNode* newnode = reinterpret_cast<FLNode*>(curr_node_addr + bytes + node_size);
                    newnode->value = cursor->value - bytes - node_size;
                    fl.add_node(newnode);

                    cursor->value = bytes;

                    return reinterpret_cast<FLNode*>(curr_node_addr + node_size);
                }
                else
                {
                    cursor->value = bytes;
                    fl.remove_node(cursor);
                    allocated_bytes += bytes;
                    return reinterpret_cast<FLNode*>(reinterpret_cast<void*>(cursor) + node_size);
                }
            }
        }

        return nullptr;
    }

    // Deallocate a block of memory to free it up for re-allocation.
    void deallocate(void* addr)
    {
        void* newnode_addr = addr - node_size;
        FLNode* node = reinterpret_cast<FLNode*>(newnode_addr);
        fl.add_node(node); // to update prev and next

        bool merge_with_prev = false;
        bool merge_with_next = false;
        
        if (node->prev != nullptr)
        {
            merge_with_prev = (newnode_addr - node->prev->value - node_size == node->prev);
        }

        if (node->next != nullptr)
        {
            merge_with_next = newnode_addr + node_size + node->value == node->next;
        }
        
        if (merge_with_next && merge_with_prev)
        {
            fl.remove_node(node);
            node->prev->value += (2*node_size) + node->value + node->next->value;
            fl.remove_node(node->next);
            allocated_bytes -= (2*node_size + node->value);
        }
        else if (merge_with_next)
        {
            FLNode* removed = fl.remove_node(node->next);
            allocated_bytes -= (node_size + node->value);
            node->value += node_size + removed->value;
        }
        else if (merge_with_prev)
        {
            fl.remove_node(node);
            node->prev->value += node_size + node->value;
            allocated_bytes -= (node_size + node->value);
        }
        else
        {
            allocated_bytes -= node->value;
        }

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
    FreeList fl;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

}; // class FirstFitMemoryAllocator

#endif // FIRST_FIT_MEMORY_ALLOCATOR_H