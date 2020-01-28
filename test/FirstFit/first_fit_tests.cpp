#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "FirstFit/first_fit_memory_allocator.h"

TEST(Constructor, ByteArray)
{
    std::array<std::uint8_t, 256> arr;

    FirstFitMemoryAllocator ff(arr);

    EXPECT_EQ(ff.length(), 256);
    EXPECT_EQ(ff.free_list().count(), 1);
}

TEST(Constructor, DWordArray)
{
    std::array<std::uint32_t, 256> arr;

    FirstFitMemoryAllocator ff(arr);

    EXPECT_EQ(ff.length(), 1024);
}

TEST(Allocate, First)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block = ff.allocate(32);

    EXPECT_EQ(block, reinterpret_cast<void*>(arr.data()) + ff.node_size);
    EXPECT_EQ(ff.allocated(), 32 + 2*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
}

TEST(Allocate, Nothing)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    std::size_t before = ff.allocated();

    void* block = ff.allocate(0);

    EXPECT_EQ(block, nullptr);
    EXPECT_EQ(ff.allocated(), before);
}

TEST(Allocate, 3Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    ff.allocate(32);
    ff.allocate(32);
    ff.allocate(32);

    EXPECT_EQ(ff.allocated(), 96 + 4*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
}

TEST(Allocate, NoNodeSpace)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block = ff.allocate(256-ff.node_size);

    EXPECT_NE(block, nullptr);
    EXPECT_EQ(ff.allocated(), ff.length());
    EXPECT_EQ(ff.free_list().count(), 0);
}

TEST(Allocate, HalfNodeSpace)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    ff.allocate(256-(1.5*ff.node_size));

    EXPECT_EQ(ff.allocated(), ff.length()-0.5*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 0);
}

TEST(Allocate, ZeroNode)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    ff.allocate(256-(2*ff.node_size));

    EXPECT_EQ(ff.allocated(), ff.length());
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 0);
}

TEST(Allocate, NoSpace)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    ff.allocate(80);
    ff.allocate(80);
    void* block = ff.allocate(80);

    EXPECT_EQ(block, nullptr);
    EXPECT_EQ(ff.allocated(), 160 + 3*ff.node_size);
}

TEST(Deallocate, OnlyBlock)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block = ff.allocate(32);

    ff.deallocate(block);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderA)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block1);
    ff.deallocate(block2);
    ff.deallocate(block3);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size); // for some reason ff.node_size by itself doesn't work
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderB)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block1);
    ff.deallocate(block3);
    ff.deallocate(block2);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderC)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block2);
    ff.deallocate(block1);
    ff.deallocate(block3);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderD)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block2);
    ff.deallocate(block3);
    ff.deallocate(block1);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderE)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block3);
    ff.deallocate(block1);
    ff.deallocate(block2);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, MultipleBlocks_OrderF)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    void* block3 = ff.allocate(32);

    ff.deallocate(block3);
    ff.deallocate(block2);
    ff.deallocate(block1);

    EXPECT_EQ(ff.allocated(), 1*ff.node_size);
    EXPECT_EQ(ff.free_list().count(), 1);
    EXPECT_EQ(ff.free_list().head()->value, 256-ff.node_size);
}

TEST(Deallocate, Reallocate)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ff(arr);

    void* block1 = ff.allocate(32);
    void* block2 = ff.allocate(32);
    ff.deallocate(block1);

    void* block3 = ff.allocate(40);

    EXPECT_EQ(block2 + ff.node_size + 32, block3);
    EXPECT_EQ(ff.free_list().count(), 2);
    EXPECT_EQ(ff.free_list().head()->value, 32);
}