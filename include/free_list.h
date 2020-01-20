#ifndef FREE_LIST_H
#define FREE_LIST_H

#include <cstddef>

// Doubly linked list data structure used for keeping track of free blocks of memory in memory
//  memory allocator objects.
class FreeList
{
public:

    // Doubly linked list node used in free list.
    struct DLLNode
    {
        std::size_t value;
        DLLNode* next;
        DLLNode* prev;
    };

    // Add node to free list after a given node 'prev_node'.
    void add_node(DLLNode* new_node, DLLNode* prev_node)
    {
        if (prev_node == nullptr)
        {
            new_node->next = head_node;
            new_node->prev = nullptr;
            head_node->prev = new_node;

            head_node = new_node;
        }
        else
        {
            new_node->next = prev_node->next;
            new_node->prev = prev_node;

            if (prev_node->next != nullptr)
            {
                prev_node->next->prev = new_node;
            }

            prev_node->next = new_node;
        }

        node_count++;
    }

    // Add node to the correct position in free list based on memory addresses in ascending order.
    void add_node(DLLNode* new_node)
    {
        if (node_count > 0)
        {
            add_node(new_node, find_prev(new_node));
        }
        else
        {
            new_node->prev = nullptr;
            new_node->next = nullptr;

            head_node = new_node;

            node_count++;
        }
    }

    // Remove node from free list by updating pointers between adjacent nodes.
    DLLNode* remove_node(DLLNode* node)
    {
        if (node_count == 1)
        {
            head_node = nullptr;
            node_count--;
            return node;
        }

        if (node->prev == nullptr)
        {
            head_node = node->next;
        }
        else
        {
            node->prev->next = node->next;
        }

        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
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
    DLLNode* head()
    {
        return head_node;
    }

private:

    // Returns the node preceding the correct position of 'new_node' based on ascending memory
    //  addresses.
    DLLNode* find_prev(DLLNode* new_node)
    {
        DLLNode* cursor = head_node;

        if (cursor->value > new_node->value)
        {
            return nullptr;
        }

        while (cursor->next != nullptr)
        {
            if (cursor->next->value > new_node->value)
            {
                return cursor;
            }

            cursor = cursor->next;
        }

        return cursor;
    }

    // Number of nodes in free list.
    std::size_t node_count = 0;

    // First node in the free list.
    DLLNode* head_node = nullptr;

}; // class FreeList

#endif // FREE_LIST_H