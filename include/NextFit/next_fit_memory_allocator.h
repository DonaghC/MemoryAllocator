#ifndef NEXT_FIT_MEMORY_ALLOCATOR_H
#define NEXT_FIT_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

#include "../FirstFit/free_list.h"
#include "memory_allocator.h"

// Implementation of a memory allocator that uses the next fit algorithm to allocate memory.
class NextFitMemoryAllocator : public MemoryAllocator
{
public:

    // Type of free list node;
    using FLNode = FreeList::DLLNode;

    // Constructor that takes in a reference to a memory buffer of template type T.
    template <class T>
    NextFitMemoryAllocator(T& buffer) : 
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

        cursor = fl.head();
    }

    // Allocate a number of bytes and return the address of the allocation.
    void* allocate(std::size_t bytes)
    {
        if (cursor == nullptr)
        {
            // should only be nullptr when no free blocks
            return nullptr;
        }

        if (bytes > 0)
        {
            FLNode* node = cursor;

            int i=0;
            while (i < fl.count())
            {
                if (node->value < bytes)
                {
                    if (node->next == nullptr)
                    {
                        node = fl.head();
                    }
                    else
                    {
                        node = node->next;
                    }

                    i++;
                }
                else if (node->value >= bytes + node_size)
                {
                    fl.remove_node(node);
                    allocated_bytes += bytes + node_size;

                    void* curr_node_addr = reinterpret_cast<void*>(node);
                    FLNode* newnode = reinterpret_cast<FLNode*>(curr_node_addr + bytes + node_size);
                    newnode->value = node->value - bytes - node_size;
                    fl.add_node(newnode);

                    cursor = newnode;

                    node->value = bytes;
                    
                    return reinterpret_cast<FLNode*>(curr_node_addr + node_size);
                }
                else
                {
                    cursor = node->next;

                    node->value = bytes;
                    fl.remove_node(node);
                    allocated_bytes += bytes;
                    return reinterpret_cast<FLNode*>(reinterpret_cast<void*>(node) + node_size);
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

        FLNode* new_cursor = node;

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

            new_cursor = node->prev;
        }
        else if (merge_with_next)
        {
            FLNode* removed = fl.remove_node(node->next);
            allocated_bytes -= (node_size + node->value);
            node->value += node_size + removed->value;

            new_cursor = node;
        }
        else if (merge_with_prev)
        {
            fl.remove_node(node);
            node->prev->value += node_size + node->value;
            allocated_bytes -= (node_size + node->value);

            new_cursor = node->prev;
        }
        else
        {
            allocated_bytes -= node->value;
        }

        if (cursor == nullptr || cursor == addr+32)
        {
            cursor = new_cursor;
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

    // Returns cursor
    FLNode* get_cursor() const
    {
        return cursor;
    }

    // Size of free list node in bytes.
    static const std::size_t node_size = sizeof(FLNode);

private:

    // Pointer to memory buffer managed by this object.
    void* mem;

    // Free list to keep track of all unallocated blocks of memory.
    FreeList fl;

    // Keep track of place in free list.
    FLNode* cursor;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

}; // class NextFitMemoryAllocator

#endif // NEXT_FIT_MEMORY_ALLOCATOR_H