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

    }

    // Add node to the correct position in free list based on memory addresses in ascending order.
    void add_node(DLLNode* new_node)
    {
        add_node(new_node, find_pos(new_node));
    }

    // Remove node from free list by updating pointers between adjacent nodes.
    DLLNode* remove_node(DLLNode* node)
    {
        //placeholder
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

    // Number of nodes in free list.
    std::size_t node_count = 0;

    // First node in the free list.
    DLLNode* head_node;

    // Returns the address of the node preceding the correct position of 'new_node' based on
    //  ascending memory addresses.
    DLLNode* find_pos(DLLNode* new_node)
    {
        //placeholder
        return new_node;
    }

}; // class FreeList

#endif // FREE_LIST_H