#include <array>
#include <chrono>
#include <cstddef>

#include <gtest/gtest.h>

#include "FirstFit/first_fit_memory_allocator.h"
#include "NextFit/next_fit_memory_allocator.h"
#include "PoolAllocation/pool_allocation_memory_allocator.h"

using FLNode_FF = FirstFitFreeList::DLLNode;
const std::size_t NODESIZE_FF = FirstFitMemoryAllocator::node_size;

TEST(Allocation, First_1000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = ffma.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        ffma.deallocate(b);

        i1++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = nfma.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        nfma.deallocate(b);

        i2++;
    }

    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";

    std::array<std::uint8_t, 256> arr3;
    PoolAllocationMemoryAllocator<8> pama(arr2);

    double time3 = 0;

    int i3=0;
    while (i3<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = pama.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time3 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        nfma.deallocate(b);

        i3++;
    }

    std::cout << "PoolAllocation time:\t" << time3/1000000 << "ms\n";
}

TEST(Allocation, First_10000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        auto b1 = ffma.allocate(1);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

        ffma.deallocate(b1);

        i1++;
    }

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        auto b1 = nfma.allocate(1);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

        nfma.deallocate(b1);

        i2++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, First_50000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        auto b1 = ffma.allocate(1);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

        ffma.deallocate(b1);

        i1++;
    }

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        auto b1 = nfma.allocate(1);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

        nfma.deallocate(b1);

        i2++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 1000Times)
{
    std::array<std::uint8_t, 25024> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    auto start_time1 = std::chrono::high_resolution_clock::now();
    int i1=0;
    while (i1<1000)
    {  
        ffma.allocate(1);
        i1++;
    }
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 25024> arr2;
    NextFitMemoryAllocator nfma(arr2);

    auto start_time2 = std::chrono::high_resolution_clock::now();
    int i2=0;
    while (i2<1000)
    {  
        nfma.allocate(1);
        i2++;
    }
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 10000Times)
{
    std::array<std::uint8_t, 250024> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    auto start_time1 = std::chrono::high_resolution_clock::now();
    int i1=0;
    while (i1<10000)
    {  
        ffma.allocate(1);
        i1++;
    }
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 250024> arr2;
    NextFitMemoryAllocator nfma(arr2);

    auto start_time2 = std::chrono::high_resolution_clock::now();
    int i2=0;
    while (i2<10000)
    {  
        nfma.allocate(1);
        i2++;
    }
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 50000Times)
{
    std::array<std::uint8_t, 1275048> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    auto start_time1 = std::chrono::high_resolution_clock::now();
    int i1=0;
    while (i1<50000)
    {  
        ffma.allocate(1);
        i1++;
    }
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 1275048> arr2;
    NextFitMemoryAllocator nfma(arr2);

    auto start_time2 = std::chrono::high_resolution_clock::now();
    int i2=0;
    while (i2<50000)
    {  
        nfma.allocate(1);
        i2++;
    }
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 1000FreeBlocks)
{
    std::array<std::uint8_t, 50033> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 1000> allocs1;

    auto addr = ffma.allocate(2);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<998 && addr != nullptr)
    {  
        ffma.allocate(1);
        addr = ffma.allocate(1);

        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    auto addr2 = ffma.allocate(8);
    allocs1[999] = ffma.allocate(8);

    ffma.allocate(2);
    
    int i2=0;
    while (i2<1000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 50033> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 1000> allocs2;

    auto addr3 = nfma.allocate(2);
    allocs2[0] = addr3;
    
    int i3=0;
    while (i3<998 && addr3 != nullptr)
    {  
        nfma.allocate(1);
        addr3 = nfma.allocate(1);

        allocs2[i3+1] = addr3;

        i3++;
    }
    if (i3 < 998)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    auto addr4 = nfma.allocate(8);
    allocs2[999] = nfma.allocate(8);

    nfma.allocate(2);
    
    int i4=0;
    while (i4<1000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 10000FreeBlocks)
{
    std::array<std::uint8_t, 9 + (20000*(1+NODESIZE_FF)) + NODESIZE_FF> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 10000> allocs1;

    auto addr = ffma.allocate(2);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<9998 && addr != nullptr)
    {  
        ffma.allocate(1);
        addr = ffma.allocate(1);

        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 9998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    auto addr2 = ffma.allocate(8);
    allocs1[9999] = ffma.allocate(8);

    ffma.allocate(2);
    
    int i2=0;
    while (i2<10000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 9 + (20000*(1+NODESIZE_FF)) + NODESIZE_FF> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 10000> allocs2;

    auto addr3 = nfma.allocate(2);
    allocs2[0] = addr3;
    
    int i3=0;
    while (i3<9998 && addr3 != nullptr)
    {  
        nfma.allocate(1);
        addr3 = nfma.allocate(1);

        allocs2[i3+1] = addr3;

        i3++;
    }
    if (i3 < 9998)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    auto addr4 = nfma.allocate(8);
    allocs2[9999] = nfma.allocate(8);

    nfma.allocate(2);
    
    int i4=0;
    while (i4<10000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, 50000FreeBlocks)
{
    std::array<std::uint8_t, 9 + (100000*(1+NODESIZE_FF)) + NODESIZE_FF> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 50000> allocs1;

    auto addr = ffma.allocate(2);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<49998 && addr != nullptr)
    {  
        ffma.allocate(1);
        addr = ffma.allocate(1);

        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 49998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    auto addr2 = ffma.allocate(8);
    allocs1[49999] = ffma.allocate(8);

    ffma.allocate(2);
    
    int i2=0;
    while (i2<50000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 9 + (100000*(1+NODESIZE_FF)) + NODESIZE_FF> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 50000> allocs2;

    auto addr3 = nfma.allocate(2);
    allocs2[0] = addr3;
    
    int i3=0;
    while (i3<49998 && addr3 != nullptr)
    {  
        nfma.allocate(1);
        addr3 = nfma.allocate(1);

        allocs2[i3+1] = addr3;

        i3++;
    }
    if (i3 < 49998)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    auto addr4 = nfma.allocate(8);
    allocs2[49999] = nfma.allocate(8);

    nfma.allocate(2);
    
    int i4=0;
    while (i4<50000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_NoFreeBlocks)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    auto addr1 = ffma.allocate(8);

    int i1=0;
    while (i1<7 && addr1 != nullptr)
    {
        addr1 = ffma.allocate(8);
        i1++;
    }
    if (i1 < 7)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(64);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    auto addr2 = nfma.allocate(8);

    int i2=0;
    while (i2<7 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(8);
        i2++;
    }
    if (i2 < 7)
    {
        std::cout << "nullptr at i2=" << i2 << "\n";
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = ffma.allocate(64);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_1000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 2000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 1000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<999 && addr1 != nullptr)
    {  
        ffma.allocate(1);
        addr1 = ffma.allocate(1);

        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    int i2=0;
    while (i2<1000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b1, nullptr);

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 2000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 1000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<999 && addr2 != nullptr)
    {  
        nfma.allocate(1);
        addr2 = nfma.allocate(1);

        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    int i4=0;
    while (i4<1000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b2, nullptr);

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_10000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 20000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 10000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<9999 && addr1 != nullptr)
    {  
        ffma.allocate(1);
        addr1 = ffma.allocate(1);

        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    int i2=0;
    while (i2<10000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b1, nullptr);

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 20000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 10000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<9999 && addr2 != nullptr)
    {  
        nfma.allocate(1);
        addr2 = nfma.allocate(1);

        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 9999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    int i4=0;
    while (i4<10000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b2, nullptr);

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_50000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 100000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 50000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<49999 && addr1 != nullptr)
    {  
        ffma.allocate(1);
        addr1 = ffma.allocate(1);

        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ffma.allocate(1);

    int i2=0;
    while (i2<50000)
    {
        ffma.deallocate(allocs1[i2]);
        i2++;
    }

    auto start_time1 = std::chrono::high_resolution_clock::now();
    auto b1 = ffma.allocate(8);
    auto end_time1 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b1, nullptr);

    double time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();

    std::array<std::uint8_t, 100000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 50000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<49999 && addr2 != nullptr)
    {  
        nfma.allocate(1);
        addr2 = nfma.allocate(1);

        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 49999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    nfma.allocate(1);

    int i4=0;
    while (i4<50000)
    {
        nfma.deallocate(allocs2[i4]);
        i4++;
    }

    auto start_time2 = std::chrono::high_resolution_clock::now();
    auto b2 = nfma.allocate(8);
    auto end_time2 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b2, nullptr);

    double time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 - start_time2).count();

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, First_1000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<1000)
    {
        auto b1 = ffma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ffma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i1++;
    }

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<1000)
    {
        auto b1 = nfma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        nfma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i2++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, First_10000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<10000)
    {
        auto b1 = ffma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ffma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i1++;
    }

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<10000)
    {
        auto b1 = nfma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        nfma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i2++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, First_50000Times)
{
    std::array<std::uint8_t, 256> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    double time1 = 0;

    int i1=0;
    while (i1<50000)
    {
        auto b1 = ffma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ffma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i1++;
    }

    std::array<std::uint8_t, 256> arr2;
    NextFitMemoryAllocator nfma(arr2);

    double time2 = 0;

    int i2=0;
    while (i2<50000)
    {
        auto b1 = nfma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        nfma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i2++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_1000Times)
{
    std::array<std::uint8_t, 1000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 1000> allocs1;

    auto addr = ffma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<999 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 1000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 1000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<999 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 999)
    {
        std::cout << "nullptr at i2=" << i2 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_10000Times)
{
    std::array<std::uint8_t, 10000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 10000> allocs1;

    auto addr = ffma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<9999 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 10000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 10000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<9999 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 9999)
    {
        std::cout << "nullptr at i2=" << i2 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_50000Times)
{
    std::array<std::uint8_t, 50000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 50000> allocs1;

    auto addr = ffma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<49999 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 50000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 50000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<49999 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 49999)
    {
        std::cout << "nullptr at i2=" << i2 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_1000Times)
{
    std::array<std::uint8_t, 1000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 1000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<999 && addr1 != nullptr)
    {
        addr1 = ffma.allocate(1);
        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[999-i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 1000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 1000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<999 && addr2 != nullptr)
    {
        addr2 = ffma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[999-i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_10000Times)
{
    std::array<std::uint8_t, 10000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 10000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<9999 && addr1 != nullptr)
    {
        addr1 = ffma.allocate(1);
        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[9999-i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 10000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 10000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<9999 && addr2 != nullptr)
    {
        addr2 = ffma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 9999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[9999-i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_50000Times)
{
    std::array<std::uint8_t, 50000*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 50000> allocs1;

    auto addr1 = ffma.allocate(1);
    allocs1[0] = addr1;
    
    int i1=0;
    while (i1<49999 && addr1 != nullptr)
    {
        addr1 = ffma.allocate(1);
        allocs1[i1+1] = addr1;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs1[49999-i2]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i2++;
    }

    std::array<std::uint8_t, 50000*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 50000> allocs2;

    auto addr2 = nfma.allocate(1);
    allocs2[0] = addr2;
    
    int i3=0;
    while (i3<49999 && addr2 != nullptr)
    {
        addr2 = ffma.allocate(1);
        allocs2[i3+1] = addr2;

        i3++;
    }
    if (i3 < 49999)
    {
        std::cout << "nullptr at i3=" << i3 << "\n";
    }

    double time2 = 0;

    int i4=0;
    while (i4<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs2[49999-i4]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i4++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_1000Times)
{
    std::array<std::uint8_t, 2001*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 1001> allocs11;
    std::array<void*, 1000> allocs12;

    auto addr = ffma.allocate(1);
    allocs11[0] = addr;
    
    int i1=0;
    while (i1<1000 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs12[i1] = addr;

        addr = ffma.allocate(1);
        allocs11[i1+1] = addr;

        i1++;
    }
    if (i1 < 1000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<1000)
    {
        ffma.deallocate(allocs11[i2]);
        i2++;
    }

    int i3=0;
    while (i3<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs12[i3]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i3++;
    }

    std::array<std::uint8_t, 2001*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 1001> allocs21;
    std::array<void*, 1000> allocs22;

    auto addr2 = nfma.allocate(1);
    allocs21[0] = addr2;
    
    int i4=0;
    while (i4<1000 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs22[i4] = addr2;

        addr2 = ffma.allocate(1);
        allocs21[i4+1] = addr2;

        i4++;
    }
    if (i4 < 1000)
    {
        std::cout << "nullptr at i4=" << i4 << "\n";
    }

    double time2 = 0;

    int i5=0;
    while (i5<1000)
    {
        nfma.deallocate(allocs21[i5]);
        i5++;
    }

    int i6=0;
    while (i6<1000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs22[i6]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i6++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_10000Times)
{
    std::array<std::uint8_t, 20001*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 10001> allocs11;
    std::array<void*, 10000> allocs12;

    auto addr = ffma.allocate(1);
    allocs11[0] = addr;
    
    int i1=0;
    while (i1<10000 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs12[i1] = addr;

        addr = ffma.allocate(1);
        allocs11[i1+1] = addr;

        i1++;
    }
    if (i1 < 10000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<10000)
    {
        ffma.deallocate(allocs11[i2]);
        i2++;
    }

    int i3=0;
    while (i3<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs12[i3]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i3++;
    }

    std::array<std::uint8_t, 20001*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 10001> allocs21;
    std::array<void*, 10000> allocs22;

    auto addr2 = nfma.allocate(1);
    allocs21[0] = addr2;
    
    int i4=0;
    while (i4<10000 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs22[i4] = addr2;

        addr2 = ffma.allocate(1);
        allocs21[i4+1] = addr2;

        i4++;
    }
    if (i4 < 10000)
    {
        std::cout << "nullptr at i4=" << i4 << "\n";
    }

    double time2 = 0;

    int i5=0;
    while (i5<10000)
    {
        nfma.deallocate(allocs21[i5]);
        i5++;
    }

    int i6=0;
    while (i6<10000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs22[i6]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i6++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_50000Times)
{
    std::array<std::uint8_t, 100001*(1+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<void*, 50001> allocs11;
    std::array<void*, 50000> allocs12;

    auto addr = ffma.allocate(1);
    allocs11[0] = addr;
    
    int i1=0;
    while (i1<50000 && addr != nullptr)
    {
        addr = ffma.allocate(1);
        allocs12[i1] = addr;

        addr = ffma.allocate(1);
        allocs11[i1+1] = addr;

        i1++;
    }
    if (i1 < 50000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time1 = 0;

    int i2=0;
    while (i2<50000)
    {
        ffma.deallocate(allocs11[i2]);
        i2++;
    }

    int i3=0;
    while (i3<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        ffma.deallocate(allocs12[i3]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time1 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i3++;
    }

    std::array<std::uint8_t, 100001*(1+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<void*, 50001> allocs21;
    std::array<void*, 50000> allocs22;

    auto addr2 = nfma.allocate(1);
    allocs21[0] = addr2;
    
    int i4=0;
    while (i4<50000 && addr2 != nullptr)
    {
        addr2 = nfma.allocate(1);
        allocs22[i4] = addr2;

        addr2 = ffma.allocate(1);
        allocs21[i4+1] = addr2;

        i4++;
    }
    if (i4 < 50000)
    {
        std::cout << "nullptr at i4=" << i4 << "\n";
    }

    double time2 = 0;

    int i5=0;
    while (i5<50000)
    {
        nfma.deallocate(allocs21[i5]);
        i5++;
    }

    int i6=0;
    while (i6<50000)
    {
        auto start_time1 = std::chrono::high_resolution_clock::now();
        nfma.deallocate(allocs22[i6]);
        auto end_time1 = std::chrono::high_resolution_clock::now();
        
        time2 += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time1 - start_time1).count();
        
        i6++;
    }

    std::cout << "FirstFit time:\t" << time1/1000000 << "ms\n";
    std::cout << "NextFit time:\t" << time2/1000000 << "ms\n";
}