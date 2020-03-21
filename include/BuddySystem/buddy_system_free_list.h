#ifndef BUDDY_SYSTEM_FREE_LIST_H
#define BUDDY_SYSTEM_FREE_LIST_H

#include <cstddef>

template <std::size_t block_size>
class BuddySystemFreeList
{
public:

    struct SLLNode
    {
        std::size_t value;
        SLLNode* next;
    };

    // Add node to free list after a given node 'prev_node'.
    void add_node(SLLNode* new_node, SLLNode* prev_node)
    {
        if (prev_node == nullptr)
        {
            new_node->next = head_node;
            head_node = new_node;
        }
        else
        {
            new_node->next = prev_node->next;
            prev_node->next = new_node;
        }

        node_count++;
    }

    // Add node to the correct position in free list based on memory addresses in ascending order.
    void add_node(SLLNode* new_node)
    {
        if (node_count > 0)
        {
            add_node(new_node, find_prev(new_node));
        }
        else
        {
            new_node->next = nullptr;

            head_node = new_node;

            node_count++;
        }
    }

    // Remove node from free list by updating pointers between adjacent nodes.
    SLLNode* remove_node(SLLNode* node)
    {
        if (node_count <= 0 || node < head_node)
        {
            return nullptr;
        }

        if (node_count == 1)
        {
            head_node = nullptr;
            node_count--;
            return node;
        }

        if (node == head_node)
        {
            head_node = node->next;
        }
        else
        {
            find_prev(node)->next = node->next;
        }
        
        node_count--;
        return node;        
    }

    // Returns number of nodes in free list.
    std::size_t count() const
    {
        return node_count;
    }

    // Returns the first node in the free list.
    SLLNode* head()
    {
        return head_node;
    }

    // Returns the node preceding the correct position of 'new_node' based on ascending memory
    //  addresses.
    SLLNode* find_prev(SLLNode* new_node)
    {
        SLLNode* cursor = head_node;

        if (cursor > new_node)
        {
            return nullptr;
        }

        while (cursor->next != nullptr)
        {
            if (cursor->next > new_node)
            {
                return cursor;
            }

            cursor = cursor->next;
        }

        return cursor;
    }

private:

    // Size of each node in bytes
    static const std::size_t node_size = block_size;

    // Number of nodes in free list.
    std::size_t node_count = 0;

    // First node in the free list.
    SLLNode* head_node = nullptr;

}; // class BuddySystemFreeList

#endif // BUDDY_SYSTEM_FREE_LIST_H