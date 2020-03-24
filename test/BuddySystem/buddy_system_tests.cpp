#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "BuddySystem/buddy_system_memory_allocator.h"

const std::size_t NODESIZE = BuddySystemMemoryAllocator<0>::node_size;

TEST(Constructor, NoRem)
{
    std::array<std::uint8_t, 80+(80*NODESIZE)> arr;

    BuddySystemMemoryAllocator<1> bs(arr);

    EXPECT_EQ(bs.block_lengths()[0], 1);
    EXPECT_EQ(bs.block_lengths()[1], 2+NODESIZE);
    EXPECT_EQ(bs.block_lengths()[2], 4+(3*NODESIZE));
    EXPECT_EQ(bs.block_lengths()[3], 8+(7*NODESIZE));
    EXPECT_EQ(sizeof(arr)%(bs.block_lengths()[3] + NODESIZE), 0);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, Rem_Blocksize3)
{
    std::array<std::uint8_t, 84+(84*NODESIZE)> arr;

    BuddySystemMemoryAllocator<1> bs(arr);

    EXPECT_EQ(sizeof(arr)%(bs.block_lengths()[3] + NODESIZE), bs.block_lengths()[2] + NODESIZE);
    EXPECT_EQ(bs.allocated(), 11*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, Rem_Blocksize2)
{
    std::array<std::uint8_t, 82+(82*NODESIZE)> arr;

    BuddySystemMemoryAllocator<1> bs(arr);

    EXPECT_EQ(sizeof(arr)%(bs.block_lengths()[3] + NODESIZE), bs.block_lengths()[1] + NODESIZE);
    EXPECT_EQ(bs.allocated(), 11*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, Rem_Blocksize1)
{
    std::array<std::uint8_t, 81+(81*NODESIZE)> arr;

    BuddySystemMemoryAllocator<1> bs(arr);

    EXPECT_EQ(sizeof(arr)%(bs.block_lengths()[3] + NODESIZE), bs.block_lengths()[0] + NODESIZE);
    EXPECT_EQ(bs.allocated(), 11*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 1);
}

TEST(Constructor, Rem_Nodesize)
{
    std::array<std::uint8_t, 640+(81*NODESIZE)> arr;

    BuddySystemMemoryAllocator<8> bs(arr);

    EXPECT_EQ(bs.block_lengths()[0], 8);
    EXPECT_EQ(bs.block_lengths()[1], 16+NODESIZE);
    EXPECT_EQ(bs.block_lengths()[2], 32+(3*NODESIZE));
    EXPECT_EQ(bs.block_lengths()[3], 64+(7*NODESIZE));
    EXPECT_EQ(sizeof(arr)%(bs.block_lengths()[3] + NODESIZE), NODESIZE);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, NoRem_Blocksize4)
{
    std::array<std::uint8_t, 64+(8*NODESIZE)> arr;

    BuddySystemMemoryAllocator<8> bs(arr);

    EXPECT_EQ(sizeof(arr) - (bs.block_lengths()[3] + NODESIZE), 0);
    EXPECT_EQ(bs.allocated(), NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, NoRem_Blocksize3)
{
    std::array<std::uint8_t, 32+(4*NODESIZE)> arr;

    BuddySystemMemoryAllocator<8> bs(arr);

    EXPECT_EQ(sizeof(arr) - (bs.block_lengths()[2] + NODESIZE), 0);
    EXPECT_EQ(bs.allocated(), NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, NoRem_Blocksize2)
{
    std::array<std::uint8_t, 16+(2*NODESIZE)> arr;

    BuddySystemMemoryAllocator<8> bs(arr);

    EXPECT_EQ(sizeof(arr) - (bs.block_lengths()[1] + NODESIZE), 0);
    EXPECT_EQ(bs.allocated(), NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Constructor, NoRem_Blocksize1)
{
    std::array<std::uint8_t, 8+NODESIZE> arr;

    BuddySystemMemoryAllocator<8> bs(arr);

    EXPECT_EQ(sizeof(arr) - (bs.block_lengths()[0] + NODESIZE), 0);
    EXPECT_EQ(bs.allocated(), NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 1);
}

TEST(Allocate, Nothing)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto allocated_bytes = bs.allocated();

    auto addr = bs.allocate(0);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(allocated_bytes, bs.allocated());
}

TEST(Allocate, TooManyBytes)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto allocated_bytes = bs.allocated();

    auto addr = bs.allocate(65+(7*NODESIZE));

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(allocated_bytes, bs.allocated());
}

TEST(Allocate, Blocksize4)
{
    std::array<std::uint8_t, 704+(88*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[3] - 1);
    }

    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.allocated(), (11*NODESIZE) + (10*bs.block_lengths()[3]));
}

TEST(Allocate, Blocksize3)
{
    std::array<std::uint8_t, 352+(44*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[2] - 1);
    }

    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.allocated(), (11*NODESIZE) + (10*bs.block_lengths()[2]));
}

TEST(Allocate, Blocksize2)
{
    std::array<std::uint8_t, 176+(22*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[1] - 1);
    }

    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.allocated(), (11*NODESIZE) + (10*bs.block_lengths()[1]));
}

TEST(Allocate, Blocksize1)
{
    std::array<std::uint8_t, 88+(11*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[0] - 1);
    }

    EXPECT_EQ(bs.free_list1().count(), 1);
    EXPECT_EQ(bs.allocated(), (11*NODESIZE) + (10*bs.block_lengths()[0]));
}

TEST(Allocate, EveryBlocksize10Times_Descending)
{
    std::array<std::uint8_t, 3200+(400*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[3]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[2]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[1]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[0]);
    }

    EXPECT_EQ(bs.allocated(), 10*(bs.block_lengths()[0]+bs.block_lengths()[1]+bs.block_lengths()[2]+bs.block_lengths()[3]) + (72*NODESIZE));
    EXPECT_EQ(bs.free_list4().count(), 31);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Allocate, EveryBlocksize10Times_Ascending)
{
    std::array<std::uint8_t, 3200+(400*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[0]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[1]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[2]);
    }
    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[3]);
    }

    EXPECT_EQ(bs.allocated(), 10*(bs.block_lengths()[0]+bs.block_lengths()[1]+bs.block_lengths()[2]+bs.block_lengths()[3]) + (72*NODESIZE));
    EXPECT_EQ(bs.free_list4().count(), 31);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Allocate, EveryBlocksize10Times_MixedDescending)
{
    std::array<std::uint8_t, 3200+(400*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[3]);
        bs.allocate(bs.block_lengths()[2]);
        bs.allocate(bs.block_lengths()[1]);
        bs.allocate(bs.block_lengths()[0]);
    }

    EXPECT_EQ(bs.allocated(), 10*(bs.block_lengths()[0]+bs.block_lengths()[1]+bs.block_lengths()[2]+bs.block_lengths()[3]) + (72*NODESIZE));
    EXPECT_EQ(bs.free_list4().count(), 31);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Allocate, EveryBlocksize10Times_MixedAscending)
{
    std::array<std::uint8_t, 3200+(400*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[0]);
        bs.allocate(bs.block_lengths()[1]);
        bs.allocate(bs.block_lengths()[2]);
        bs.allocate(bs.block_lengths()[3]);
    }

    EXPECT_EQ(bs.allocated(), 10*(bs.block_lengths()[0]+bs.block_lengths()[1]+bs.block_lengths()[2]+bs.block_lengths()[3]) + (72*NODESIZE));
    EXPECT_EQ(bs.free_list4().count(), 31);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Allocate, NoSpace)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    for(int i=0; i<10; i++)
    {
        bs.allocate(bs.block_lengths()[3]);
    }

    auto addr = bs.allocate(bs.block_lengths()[3]);

    EXPECT_EQ(addr, nullptr);
    EXPECT_EQ(bs.length() - bs.allocated(), 0);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_Blocksize4)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr);

    EXPECT_EQ(addr, reinterpret_cast<void*>(bs.free_list4().head())+NODESIZE);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_Blocksize3)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr);

    EXPECT_EQ(addr, reinterpret_cast<void*>(bs.free_list4().head())+NODESIZE);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_Blocksize2)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr);

    EXPECT_EQ(addr, reinterpret_cast<void*>(bs.free_list4().head())+NODESIZE);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_Blocksize1)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr);

    EXPECT_EQ(addr, reinterpret_cast<void*>(bs.free_list4().head())+NODESIZE);
    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderA)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);
    bs.deallocate(addr3);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderB)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);
    bs.deallocate(addr4);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderC)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);
    bs.deallocate(addr2);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderD)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);
    bs.deallocate(addr4);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderE)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr4);
    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderF)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr4);
    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderG)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);
    bs.deallocate(addr3);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderH)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);
    bs.deallocate(addr4);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderI)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);
    bs.deallocate(addr1);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderJ)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);
    bs.deallocate(addr4);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderK)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr4);
    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderL)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr4);
    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderM)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);
    bs.deallocate(addr2);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderN)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);
    bs.deallocate(addr4);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderO)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);
    bs.deallocate(addr1);
    bs.deallocate(addr4);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderP)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);
    bs.deallocate(addr4);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderQ)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr4);
    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderR)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr4);
    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderS)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr1);
    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderT)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr1);
    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderU)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr2);
    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderV)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr2);
    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderW)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr3);
    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, First_EveryFreeList_OrderX)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);
    auto addr4 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr4);
    bs.deallocate(addr3);
    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), 10*NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 10);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_1stBlock_FreeList4)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 0)
        {
            addr1 = bs.allocate(bs.block_lengths()[3]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[3]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[3]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_6thBlock_FreeList4)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 5)
        {
            addr1 = bs.allocate(bs.block_lengths()[3]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[3]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[3]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_10thBlock_FreeList4)
{
    std::array<std::uint8_t, 640+(80*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 9)
        {
            addr1 = bs.allocate(bs.block_lengths()[3]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[3]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[3]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_1stBlock_FreeList3)
{
    std::array<std::uint8_t, 320+(40*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 0)
        {
            addr1 = bs.allocate(bs.block_lengths()[2]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[2]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[2]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_6thBlock_FreeList3)
{
    std::array<std::uint8_t, 320+(40*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 5)
        {
            addr1 = bs.allocate(bs.block_lengths()[2]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[2]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[2]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_10thBlock_FreeList3)
{
    std::array<std::uint8_t, 320+(40*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 9)
        {
            addr1 = bs.allocate(bs.block_lengths()[2]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[2]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[2]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_1stBlock_FreeList2)
{
    std::array<std::uint8_t, 160+(20*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 0)
        {
            addr1 = bs.allocate(bs.block_lengths()[1]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[1]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[1]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_6thBlock_FreeList2)
{
    std::array<std::uint8_t, 160+(20*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 5)
        {
            addr1 = bs.allocate(bs.block_lengths()[1]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[1]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[1]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_10thBlock_FreeList2)
{
    std::array<std::uint8_t, 160+(20*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 9)
        {
            addr1 = bs.allocate(bs.block_lengths()[1]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[1]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[1]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_1stBlock_FreeList1)
{
    std::array<std::uint8_t, 80+(10*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 0)
        {
            addr1 = bs.allocate(bs.block_lengths()[0]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[0]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[0]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_6thBlock_FreeList1)
{
    std::array<std::uint8_t, 80+(10*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 5)
        {
            addr1 = bs.allocate(bs.block_lengths()[0]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[0]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[0]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Reallocate_10thBlock_FreeList1)
{
    std::array<std::uint8_t, 80+(10*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    void* addr1;
    for (int i=0; i<10; i++)
    {
        if (i == 9)
        {
            addr1 = bs.allocate(bs.block_lengths()[0]);
        }
        else
        {
            bs.allocate(bs.block_lengths()[0]);
        }
    }

    bs.deallocate(addr1);

    auto addr2 = bs.allocate(bs.block_lengths()[0]);

    EXPECT_EQ(addr2, addr1);
    EXPECT_EQ(bs.allocated(), bs.length());
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderA)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr1)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderB)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr1)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderC)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr1)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderD)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderE)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr1)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize4_OrderF)
{
    std::array<std::uint8_t, 192+(24*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[3]);
    auto addr2 = bs.allocate(bs.block_lengths()[3]);
    auto addr3 = bs.allocate(bs.block_lengths()[3]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[3]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 2);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderA)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 2);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderB)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderC)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 2);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderD)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderE)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize3_OrderF)
{
    std::array<std::uint8_t, 96+(12*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[2]);
    auto addr2 = bs.allocate(bs.block_lengths()[2]);
    auto addr3 = bs.allocate(bs.block_lengths()[2]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[2]);
    EXPECT_EQ(bs.free_list4().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 1);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderA)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 2);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderB)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderC)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 2);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderD)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderE)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize2_OrderF)
{
    std::array<std::uint8_t, 48+(6*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[1]);
    auto addr2 = bs.allocate(bs.block_lengths()[1]);
    auto addr3 = bs.allocate(bs.block_lengths()[1]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[1]);
    EXPECT_EQ(bs.free_list3().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 1);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize1_OrderA)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr1);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list1().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 2);
}

TEST(Deallocate, Blocksize1_OrderB)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr1);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize1_OrderC)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr2);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (3*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list1().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 0);
    EXPECT_EQ(bs.free_list1().count(), 2);
}

TEST(Deallocate, Blocksize1_OrderD)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr2);
    bs.deallocate(addr3);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize1_OrderE)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr3);
    bs.deallocate(addr1);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr3)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}

TEST(Deallocate, Blocksize1_OrderF)
{
    std::array<std::uint8_t, 24+(3*NODESIZE)> arr;
    BuddySystemMemoryAllocator<8> bs(arr);

    auto addr1 = bs.allocate(bs.block_lengths()[0]);
    auto addr2 = bs.allocate(bs.block_lengths()[0]);
    auto addr3 = bs.allocate(bs.block_lengths()[0]);

    bs.deallocate(addr3);
    bs.deallocate(addr2);

    EXPECT_EQ(bs.allocated(), (2*NODESIZE) + bs.block_lengths()[0]);
    EXPECT_EQ(bs.free_list2().head(), reinterpret_cast<void*>(addr2)-NODESIZE);
    EXPECT_EQ(bs.free_list4().count(), 0);
    EXPECT_EQ(bs.free_list3().count(), 0);
    EXPECT_EQ(bs.free_list2().count(), 1);
    EXPECT_EQ(bs.free_list1().count(), 0);
}