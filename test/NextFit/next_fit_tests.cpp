#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "NextFit/next_fit_memory_allocator.h"

TEST(Constructor, ByteArray)
{
    std::array<std::uint8_t, 256> arr;

    NextFitMemoryAllocator nf(arr);

    EXPECT_EQ(nf.length(), 256);
    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.get_cursor(), nf.free_list().head());
}

TEST(Constructor, DWordArray)
{
    std::array<std::uint32_t, 256> arr;

    NextFitMemoryAllocator nf(arr);

    EXPECT_EQ(nf.length(), 1024);
    EXPECT_EQ(nf.get_cursor(), nf.free_list().head());
}

TEST(Allocate, First)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block = nf.allocate(32);

    EXPECT_EQ(block, reinterpret_cast<void*>(arr.data()) + nf.node_size);
    EXPECT_EQ(nf.allocated(), 32 + 2*nf.node_size);
    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.get_cursor(), block + 32);
}

TEST(Allocate, Nothing)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    std::size_t allocated_before = nf.allocated();
    auto cursor_before = nf.get_cursor();

    void* block = nf.allocate(0);

    EXPECT_EQ(block, nullptr);
    EXPECT_EQ(nf.allocated(), allocated_before);
    EXPECT_EQ(nf.get_cursor(), cursor_before);
}

TEST(Allocate, 3Times)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block1 = nf.allocate(32);
    nf.allocate(32);
    nf.allocate(32);

    EXPECT_EQ(nf.allocated(), 96 + 4*nf.node_size);
    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.get_cursor(), block1 + 96 + 2*nf.node_size);
}

TEST(Allocate, NoNodeSpace)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block = nf.allocate(256-nf.node_size);

    EXPECT_NE(block, nullptr);
    EXPECT_EQ(nf.allocated(), nf.length());
    EXPECT_EQ(nf.free_list().count(), 0);
    EXPECT_EQ(nf.get_cursor(), nullptr);
}

TEST(Allocate, HalfNodeSpace)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    nf.allocate(256-(1.5*nf.node_size));

    EXPECT_EQ(nf.allocated(), nf.length()-0.5*nf.node_size);
    EXPECT_EQ(nf.free_list().count(), 0);
    EXPECT_EQ(nf.get_cursor(), nullptr);
}

TEST(Allocate, ZeroNode)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block = nf.allocate(256-(2*nf.node_size));

    EXPECT_EQ(nf.allocated(), nf.length());
    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.free_list().head()->value, 0);
    EXPECT_EQ(nf.get_cursor(), block + 256 - (2*nf.node_size));
}

TEST(Allocate, NoSpace)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    nf.allocate(80);
    void* block2 = nf.allocate(80);
    void* block = nf.allocate(80);

    EXPECT_EQ(block, nullptr);
    EXPECT_EQ(nf.allocated(), 160 + 3*nf.node_size);
    EXPECT_EQ(nf.get_cursor(), block2 + 80);
}

TEST(Deallocate, OnlyBlock)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block = nf.allocate(32);

    nf.deallocate(block);

    EXPECT_EQ(nf.allocated(), 1*nf.node_size);
    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.free_list().head()->value, 256-nf.node_size);
    EXPECT_EQ(nf.get_cursor(), block - nf.node_size);
}

TEST(Deallocate, 1Block_NoSpace)
{
    std::array<std::uint8_t, (8*(8+NextFitMemoryAllocator::node_size))> arr;
    NextFitMemoryAllocator nf(arr);

    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    void* block4 = nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);

    nf.deallocate(block4);

    EXPECT_EQ(nf.free_list().count(), 1);
    EXPECT_EQ(nf.get_cursor(), block4 - nf.node_size);
}

TEST(Deallocate, 2Blocks_NoSpace_OrderA)
{
    std::array<std::uint8_t, (8*(8+NextFitMemoryAllocator::node_size))> arr;
    NextFitMemoryAllocator nf(arr);

    nf.allocate(8);
    void* block2 = nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    void* block7 = nf.allocate(8);
    nf.allocate(8);

    nf.deallocate(block2);
    nf.deallocate(block7);

    EXPECT_EQ(nf.free_list().count(), 2);
    EXPECT_EQ(nf.get_cursor(), block2 - nf.node_size);
}

TEST(Deallocate, 2Blocks_NoSpace_OrderB)
{
    std::array<std::uint8_t, (8*(8+NextFitMemoryAllocator::node_size))> arr;
    NextFitMemoryAllocator nf(arr);

    nf.allocate(8);
    void* block2 = nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    nf.allocate(8);
    void* block7 = nf.allocate(8);
    nf.allocate(8);

    nf.deallocate(block7);
    nf.deallocate(block2);

    EXPECT_EQ(nf.free_list().count(), 2);
    EXPECT_EQ(nf.get_cursor(), block7 - nf.node_size);
}

TEST(Deallocate, Reallocate)
{
    std::array<std::uint8_t, 256> arr;
    NextFitMemoryAllocator nf(arr);

    void* block1 = nf.allocate(32);
    void* block2 = nf.allocate(32);
    nf.deallocate(block1);

    void* block3 = nf.allocate(40);

    EXPECT_EQ(block2 + nf.node_size + 32, block3);
    EXPECT_EQ(nf.free_list().count(), 2);
    EXPECT_EQ(nf.free_list().head()->value, 32);
    EXPECT_EQ(nf.get_cursor(), block3 + 40);
}