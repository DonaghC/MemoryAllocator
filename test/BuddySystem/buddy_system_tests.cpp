#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "BuddySystem/buddy_system_memory_allocator.h"

const std::size_t NODESIZE = BuddySystemMemoryAllocator<0>::node_size;

TEST(Test, Test)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;

    BuddySystemMemoryAllocator<8> bs(arr);
}