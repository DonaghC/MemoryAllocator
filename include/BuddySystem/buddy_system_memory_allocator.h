#ifndef BUDDY_SYSTEM_MEMORY_ALLOCATOR_H
#define BUDDY_SYSTEM_MEMORY_ALLOCATOR_H

#include <cstddef>
#include <cstdint>

#include "BuddySystem/buddy_system_free_list.h"
#include "memory_allocator.h"

// Implementation of a memory allocator that uses a variation on the buddy system algorithm to allocate memory.
// The variation is that instead of using free lists that double in size, this uses free lists to double in size plus room
// for a new node. This is because of the use of a free list to keep track of every node.
template<std::size_t smallest_block_size>
class BuddySystemMemoryAllocator : public MemoryAllocator
{
public:

    // Type of free list node
    template <std::size_t block_size>
    using FLNode = typename BuddySystemFreeList<block_size>::SLLNode;

    // Constructor that takes in a reference to a memory buffer of template type T.
    template <class T>
    BuddySystemMemoryAllocator(T& buffer) : 
        mem(buffer.data()),
        allocated_bytes(node_size),
        total_bytes(
            reinterpret_cast<std::uint8_t*>(buffer.end()) 
            - reinterpret_cast<std::uint8_t*>(buffer.begin())
            )
    {
        assert(smallest_block_size <= total_bytes);

        void* cursor = mem;
        
        FLNode<block_size4>* node4 = reinterpret_cast<FLNode<block_size4>*>(mem);
        node4->value = block_size4;
        fl4.add_node(node4);
        cursor += node_size + block_size4;

        FLNode<block_size4>* prev_node4 = node4;

        while (cursor + node_size + block_size4 <= buffer.end())
        {
            node4 = reinterpret_cast<FLNode<block_size4>*>(cursor);
            node4->value = block_size4;
            fl4.add_node(node4, prev_node4);

            prev_node4 = node4;
            cursor += node_size + block_size4;
            allocated_bytes += node_size;
        }

        FLNode<block_size3>* node3;
        FLNode<block_size3>* prev_node3 = nullptr;
        while (cursor + node_size + block_size3 <= buffer.end())
        {
            node3 = reinterpret_cast<FLNode<block_size3>*>(cursor);
            node3->value = block_size3;
            fl3.add_node(node3, prev_node3);

            prev_node3 = node3;
            cursor += node_size + block_size3;
            allocated_bytes += node_size;
        }

        FLNode<block_size2>* node2;
        FLNode<block_size2>* prev_node2 = nullptr;
        while (cursor + node_size + block_size2 <= buffer.end())
        {
            node2 = reinterpret_cast<FLNode<block_size2>*>(cursor);
            node2->value = block_size2;
            fl2.add_node(node2, prev_node2);

            prev_node2 = node2;
            cursor += node_size + block_size2;
            allocated_bytes += node_size;
        }

        FLNode<smallest_block_size>* node;
        FLNode<smallest_block_size>* prev_node = nullptr;
        while (cursor + node_size + smallest_block_size <= buffer.end())
        {
            node = reinterpret_cast<FLNode<smallest_block_size>*>(cursor);
            node->value = smallest_block_size;
            fl1.add_node(node, prev_node);

            prev_node = node;
            cursor += node_size + smallest_block_size;
            allocated_bytes += node_size;
        }
    }

    // Allocate a number of bytes and return the address of the allocation.
    void* allocate(std::size_t bytes)
    {
        if (bytes <= 0)
        {
            return nullptr;
        }

        if (bytes <= smallest_block_size)
        {
            if (fl1.count() == 0)
            {
                if (!divide_node<block_size2>(fl2, fl1))
                {
                    return nullptr;
                }
            }

            auto node = fl1.head();
            fl1.remove_node(node);

            allocated_bytes += smallest_block_size;

            return node+node_size;
        }
        else if (bytes <= block_size2)
        {
            if (fl2.count() == 0)
            {
                if (!divide_node<block_size3>(fl3, fl2))
                {
                    return nullptr;
                }
            }

            auto node = fl2.head();
            fl2.remove_node(node);

            allocated_bytes += block_size2;

            return node+node_size;
        }
        else if (bytes <= block_size3)
        {
            if (fl3.count() == 0)
            {
                if (!divide_node<block_size4>(fl4, fl3))
                {
                    return nullptr;
                }
            }

            auto node = fl3.head();
            fl3.remove_node(node);

            allocated_bytes += block_size3;

            return node+node_size;
        }
        else if (bytes <= block_size4)
        {
            if (fl4.count() == 0)
            {
                return nullptr;
            }

            auto node = fl4.head();
            fl4.remove_node(node);

            allocated_bytes += block_size4;

            return node+node_size;
        }

        return nullptr;
    }

    // Deallocate a block of memory to free it up for re-allocation.
    void deallocate(void* addr)
    {
        void* newnode_addr = addr - node_size;
        auto temp_node = reinterpret_cast<FLNode<smallest_block_size>*>(newnode_addr);
        
        switch(temp_node->value)
        {
            case block_size4:
                return _deallocate<block_size4>(reinterpret_cast<FLNode<block_size4>*>(newnode_addr), fl4);
            case block_size3:
                return _deallocate<block_size3>(reinterpret_cast<FLNode<block_size3>*>(newnode_addr), fl3);
            case block_size2:
                return _deallocate<block_size2>(reinterpret_cast<FLNode<block_size2>*>(newnode_addr), fl2);
            case smallest_block_size:
                return _deallocate<smallest_block_size>(temp_node, fl1);
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

    // Returns array of the lengths of each different size block in bytes.
    static std::array<std::size_t, 4> block_lengths()
    {
        return std::array<std::size_t, 4>(smallest_block_size, block_size2, block_size3, block_size4);
    }

    // Return smallest free list.
    auto free_list1() const
    {
        return fl1;
    }

    // Return second smallest free list.
    auto free_list2() const
    {
        return fl2;
    }

    // Return second largest free list.
    auto free_list3() const
    {
        return fl3;
    }

    // Return largest free list.
    auto free_list4() const
    {
        return fl4;
    }

    // Size of free list node in bytes.
    static const std::size_t node_size = sizeof(FLNode<smallest_block_size>);

private:

    // Divide a node from one free list with block sizes double that of another free list.
    // Add the 2 new nodes to the smaller free list and return true if this was successful.
    template <std::size_t block_size>
    bool divide_node(BuddySystemFreeList<block_size> &larger_fl, BuddySystemFreeList<(block_size/2)-(node_size/2)> &smaller_fl)
    {
        static const std::size_t smaller_block_size = (block_size/2)-(node_size/2);
        
        if (larger_fl.count() == 0)
        {
            switch(block_size)
            {
                case block_size3:
                    return divide_node<block_size4>(fl4, fl3);
                case block_size2:
                    return divide_node<block_size3>(fl3, fl2);
                default:
                    return false;
            }
        }

        auto addr1 = larger_fl.remove_node(larger_fl.head());

        auto node1 = reinterpret_cast<FLNode<smaller_block_size>*>(addr1-node_size);
        node1->value = smaller_block_size;
        smaller_fl.add_node(node1);

        auto node2 = reinterpret_cast<FLNode<smaller_block_size>*>(addr1+smaller_block_size);
        node2->value = smaller_block_size;
        smaller_fl.add_node(node2);

        allocated_bytes += node_size;

        return true;
    }

    // Called by the public deallocate method to deallocate blocks of size 'block_size'.
    template <std::size_t block_size>
    void _deallocate(FLNode<block_size>* node, BuddySystemFreeList<block_size> &fl)
    {
        if (block_size != block_size4)
        {
            auto prev = fl.find_prev(node);
            if (reinterpret_cast<void*>(prev) + block_size + node_size == reinterpret_cast<void*>(node))
            {
                merge_nodes<block_size>(prev, node, fl);

                allocated_bytes -= block_size;

                return;
            }
            else if (reinterpret_cast<void*>(node) + block_size + node_size == reinterpret_cast<void*>(node->next))
            {
                merge_nodes<block_size>(node, node->next, fl);

                allocated_bytes -= block_size;

                return;
            }
        }

        fl.add_node(node);

        allocated_bytes -= block_size;
    }

    // Merge 2 nodes 'node1' and 'node2' of value 'block_size' and remove them from fl 'fl' (if they are part of it)
    //  and merge them both into node1. Add node1 into a larger free list and return address of it.
    template <std::size_t block_size>
    void merge_nodes(FLNode<block_size>* node1, FLNode<block_size>* node2, BuddySystemFreeList<block_size> &fl)
    {
        fl.remove_node(node1);
        fl.remove_node(node2);

        node1->value += node2->value + node_size;

        void* new_node = reinterpret_cast<void*>(node1);

        switch(block_size)
        {
            case block_size3:
                fl4.add_node(reinterpret_cast<FLNode<block_size4>*>(new_node));
                break;
            case block_size2:
                fl3.add_node(reinterpret_cast<FLNode<block_size3>*>(new_node));
                break;
            case smallest_block_size:
                fl2.add_node(reinterpret_cast<FLNode<block_size2>*>(new_node));
        }

        allocated_bytes -= node_size;
    }

    // Pointer to memory buffer managed by this object.
    void* mem;

    // Size of blocks in second free list.
    static const std::size_t block_size2 = (smallest_block_size*2) + node_size;

    // Size of blocks in third free list.
    static const std::size_t block_size3 = (smallest_block_size*4) + (node_size*3);

    // Size of blocks in fourth/largest free list.
    static const std::size_t block_size4 = (smallest_block_size*8) + (node_size*7); 

    // Free list to keep track of unallocated blocks of memory of the smallest block size.
    BuddySystemFreeList<smallest_block_size> fl1;

    // Free list to keep track of unallocated blocks of memory of the smallest block size *2.
    BuddySystemFreeList<(smallest_block_size*2) + node_size> fl2;

    // Free list to keep track of unallocated blocks of memory of the smallest block size *4.
    BuddySystemFreeList<(smallest_block_size*4) + (node_size*3)> fl3;

    // Free list to keep track of unallocated blocks of memory of the smallest block size *8.
    BuddySystemFreeList<(smallest_block_size*8) + (node_size*7)> fl4;

    // Number of bytes used in memory buffer.
    std::size_t allocated_bytes;

    // Length of memory buffer in bytes.
    const std::size_t total_bytes;

}; // class BuddySystemMemoryAllocator

#endif // BUDDY_SYSTEM_MEMORY_ALLOCATOR_H