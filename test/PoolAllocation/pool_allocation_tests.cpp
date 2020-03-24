#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "PoolAllocation/pool_allocation_memory_allocator.h"

const std::size_t NODESIZE = PoolAllocationMemoryAllocator<0>::node_size;

TEST(Constructor, NoRem)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;

    PoolAllocationMemoryAllocator<8> pa(arr);

    EXPECT_EQ(pa.length()%pa.block_length(), 0);
    EXPECT_EQ(pa.total_blocks(), 10);
    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Constructor, NodesizeRem)
{
    std::array<std::uint8_t, ((24+NODESIZE)*10) + NODESIZE> arr;

    PoolAllocationMemoryAllocator<24> pa(arr);

    EXPECT_EQ(pa.length()%pa.block_length(), pa.block_length() - NODESIZE);
    EXPECT_EQ(pa.total_blocks(), 10);
    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Constructor, LargeRem)
{
    std::array<std::uint8_t, ((24+NODESIZE)*10) + NODESIZE + 12> arr;

    PoolAllocationMemoryAllocator<24> pa(arr);

    EXPECT_EQ(pa.length()%pa.block_length(), (0.5*pa.block_length()) - NODESIZE);
    EXPECT_EQ(pa.total_blocks(), 10);
    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Allocate, TooManyBytes)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr = pa.allocate(24);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Allocate, Nothing)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr = pa.allocate(0);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Allocate, First)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr = pa.allocate(pa.block_length() - 2);

    EXPECT_EQ(addr, reinterpret_cast<void*>(arr.data()) + NODESIZE);
    EXPECT_EQ(pa.free_list().count(), 9);
    EXPECT_EQ(pa.allocated(), (10*NODESIZE) + pa.block_length());
}

TEST(Allocate, NoSpac_Alloc2Bytes)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    for (int i=0; i<10; i++)
    {
        pa.allocate(2);
    }

    auto addr = pa.allocate(2);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(pa.allocated_blocks(), 10);
    EXPECT_EQ(pa.allocated(), 10*(NODESIZE+pa.block_length()));
}

TEST(Allocate, NoSpac_AllocBlocksizeBytes)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    for (int i=0; i<10; i++)
    {
        pa.allocate();
    }

    auto addr = pa.allocate(2);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(pa.allocated_blocks(), 10);
    EXPECT_EQ(pa.allocated(), 10*(NODESIZE+pa.block_length()));
}

TEST(Deallocate, First)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr = pa.allocate();

    pa.deallocate(addr);

    EXPECT_EQ(pa.free_list().count(), 10);
    EXPECT_EQ(pa.allocated(), 10*NODESIZE);
}

TEST(Deallocate, Reallocate_1stBlock)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    std::array<void*, 10> allocs;

    for (int i=0; i<10; i++)
    {
        allocs[i] = pa.allocate();
    }

    pa.deallocate(allocs[0]);

    auto addr = pa.allocate();

    EXPECT_EQ(addr, allocs[0]);
    EXPECT_EQ(pa.free_list().count(), 0);
    EXPECT_EQ(pa.allocated_blocks(), 10);
}

TEST(Deallocate, Reallocate_6thBlock)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    std::array<void*, 10> allocs;

    for (int i=0; i<10; i++)
    {
        allocs[i] = pa.allocate();
    }

    pa.deallocate(allocs[5]);

    auto addr = pa.allocate();

    EXPECT_EQ(addr, allocs[5]);
    EXPECT_EQ(pa.free_list().count(), 0);
    EXPECT_EQ(pa.allocated_blocks(), 10);
}

TEST(Deallocate, Reallocate_10thBlock)
{
    std::array<std::uint8_t, (8+NODESIZE)*10> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    std::array<void*, 10> allocs;

    for (int i=0; i<10; i++)
    {
        allocs[i] = pa.allocate();
    }

    pa.deallocate(allocs[9]);

    auto addr = pa.allocate();

    EXPECT_EQ(addr, allocs[9]);
    EXPECT_EQ(pa.free_list().count(), 0);
    EXPECT_EQ(pa.allocated_blocks(), 10);
}

TEST(Deallocate, OrderA)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr1);
    pa.deallocate(addr2);
    pa.deallocate(addr3);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}

TEST(Deallocate, OrderB)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr1);
    pa.deallocate(addr3);
    pa.deallocate(addr2);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}

TEST(Deallocate, OrderC)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr2);
    pa.deallocate(addr1);
    pa.deallocate(addr3);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}

TEST(Deallocate, OrderD)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr2);
    pa.deallocate(addr3);
    pa.deallocate(addr1);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}

TEST(Deallocate, OrderE)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr3);
    pa.deallocate(addr1);
    pa.deallocate(addr2);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}

TEST(Deallocate, OrderF)
{
    std::array<std::uint8_t, (8+NODESIZE)*3> arr;
    PoolAllocationMemoryAllocator<8> pa(arr);

    auto addr1 = pa.allocate();
    auto addr2 = pa.allocate();
    auto addr3 = pa.allocate();

    pa.deallocate(addr3);
    pa.deallocate(addr2);
    pa.deallocate(addr1);

    EXPECT_EQ(pa.free_list().count(), 3);
    EXPECT_EQ(pa.allocated_blocks(), 0);
    EXPECT_EQ(pa.allocated(), 3*NODESIZE);
}