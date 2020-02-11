#include <array>
#include <chrono>
#include <cstddef>

#include <gtest/gtest.h>

#include "FirstFit/first_fit_memory_allocator.h"

using FLNode = FreeList::DLLNode;
const std::size_t NODESIZE = FirstFitMemoryAllocator::node_size;

TEST(Allocation, First_1000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b1 = ma.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        ma.deallocate(b1);

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, First_10000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<10000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b1 = ma.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        ma.deallocate(b1);

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, First_50000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<50000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        auto b1 = ma.allocate(1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        ma.deallocate(b1);

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 1000Times)
{
    std::array<std::uint8_t, 25024> arr;
    FirstFitMemoryAllocator ma(arr);

    auto start_time = std::chrono::high_resolution_clock::now();
    int i=0;
    while (i<1000)
    {  
        ma.allocate(1);
        i++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 10000Times)
{
    std::array<std::uint8_t, 250024> arr;
    FirstFitMemoryAllocator ma(arr);

    auto start_time = std::chrono::high_resolution_clock::now();
    int i=0;
    while (i<10000)
    {  
        ma.allocate(1);
        i++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 50000Times)
{
    std::array<std::uint8_t, 1275048> arr;
    FirstFitMemoryAllocator ma(arr);

    auto start_time = std::chrono::high_resolution_clock::now();
    int i=0;
    while (i<50000)
    {  
        ma.allocate(1);
        i++;
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 1000FreeBlocks)
{
    std::array<std::uint8_t, 50033> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 1000> allocs;

    auto addr = ma.allocate(2);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<998 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);

    auto addr2 = ma.allocate(8);
    allocs[999] = ma.allocate(8);

    ma.allocate(2);
    
    int i2=0;
    while (i2<1000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 10000FreeBlocks)
{
    std::array<std::uint8_t, 9 + (20000*(1+NODESIZE)) + NODESIZE> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 10000> allocs;

    auto addr = ma.allocate(2);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<9998 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 9998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);
    
    auto addr2 = ma.allocate(8);
    allocs[9999] = ma.allocate(8);

    ma.allocate(2);
    
    int i2=0;
    while (i2<10000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, 50000FreeBlocks)
{
    std::array<std::uint8_t, 9 + (100000*(1+NODESIZE)) + NODESIZE> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 50000> allocs;

    auto addr = ma.allocate(2);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<49998 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 49998)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);
    
    auto addr2 = ma.allocate(8);
    allocs[49999] = ma.allocate(8);

    ma.allocate(2);
    
    int i2=0;
    while (i2<50000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_NoFreeBlocks)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    auto addr = ma.allocate(8);

    int i=0;
    while (i<7 && addr != nullptr)
    {
        addr = ma.allocate(8);
        i++;
    }
    if (i < 7)
    {
        std::cout << "nullptr at i=" << i << "\n";
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(64);
    auto end_time = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_1000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 2000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 1000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<999 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);

    int i2=0;
    while (i2<1000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b, nullptr);

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_10000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 20000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 10000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<9999 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);

    int i2=0;
    while (i2<10000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b, nullptr);

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Allocation, NoSpace_50000FreeBlocksTooSmall)
{
    std::array<std::uint8_t, 100000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 50000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<49999 && addr != nullptr)
    {  
        ma.allocate(1);
        addr = ma.allocate(1);

        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    ma.allocate(1);

    int i2=0;
    while (i2<50000)
    {
        ma.deallocate(allocs[i2]);
        i2++;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto b = ma.allocate(8);
    auto end_time = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(b, nullptr);

    double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, First_1000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<1000)
    {
        auto b1 = ma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, First_10000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<10000)
    {
        auto b1 = ma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, First_50000Times)
{
    std::array<std::uint8_t, 256> arr;
    FirstFitMemoryAllocator ma(arr);

    double time = 0;

    int i=0;
    while (i<50000)
    {
        auto b1 = ma.allocate(1);

        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(b1);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        i++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_1000Times)
{
    std::array<std::uint8_t, 1000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 1000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_10000Times)
{
    std::array<std::uint8_t, 10000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 10000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<9999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<10000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrev_50000Times)
{
    std::array<std::uint8_t, 50000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 50000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<49999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<50000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_1000Times)
{
    std::array<std::uint8_t, 1000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 1000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[999-i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_10000Times)
{
    std::array<std::uint8_t, 10000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 10000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<9999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 9999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<10000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[9999-i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergeNext_50000Times)
{
    std::array<std::uint8_t, 50000*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 50000> allocs;

    auto addr = ma.allocate(1);
    allocs[0] = addr;
    
    int i1=0;
    while (i1<49999 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs[i1+1] = addr;

        i1++;
    }
    if (i1 < 49999)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<50000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs[49999-i2]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i2++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_1000Times)
{
    std::array<std::uint8_t, 2001*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 1001> allocs1;
    std::array<void*, 1000> allocs2;

    auto addr = ma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<1000 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs2[i1] = addr;

        addr = ma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 1000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<1000)
    {
        ma.deallocate(allocs1[i2]);
        i2++;
    }

    int i3=0;
    while (i3<1000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs2[i3]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i3++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_10000Times)
{
    std::array<std::uint8_t, 20001*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 10001> allocs1;
    std::array<void*, 10000> allocs2;

    auto addr = ma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<10000 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs2[i1] = addr;

        addr = ma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 10000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<10000)
    {
        ma.deallocate(allocs1[i2]);
        i2++;
    }

    int i3=0;
    while (i3<10000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs2[i3]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i3++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}

TEST(Deallocation, MergePrevNext_50000Times)
{
    std::array<std::uint8_t, 100001*(1+NODESIZE)> arr;
    FirstFitMemoryAllocator ma(arr);

    std::array<void*, 50001> allocs1;
    std::array<void*, 50000> allocs2;

    auto addr = ma.allocate(1);
    allocs1[0] = addr;
    
    int i1=0;
    while (i1<50000 && addr != nullptr)
    {
        addr = ma.allocate(1);
        allocs2[i1] = addr;

        addr = ma.allocate(1);
        allocs1[i1+1] = addr;

        i1++;
    }
    if (i1 < 50000)
    {
        std::cout << "nullptr at i1=" << i1 << "\n";
    }

    double time = 0;

    int i2=0;
    while (i2<50000)
    {
        ma.deallocate(allocs1[i2]);
        i2++;
    }

    int i3=0;
    while (i3<50000)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        ma.deallocate(allocs2[i3]);
        auto end_time = std::chrono::high_resolution_clock::now();
        
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        i3++;
    }

    std::cout << "Time: " << time/1000000 << "ms\n";
}