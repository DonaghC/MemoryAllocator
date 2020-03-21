#include <array>
#include <chrono>
#include <cstddef>
#include <string>

#include <gtest/gtest.h>

#include "memory_allocator.h"
#include "FirstFit/first_fit_memory_allocator.h"
#include "NextFit/next_fit_memory_allocator.h"
#include "PoolAllocation/pool_allocation_memory_allocator.h"
#include "BuddySystem/buddy_system_memory_allocator.h"

using FLNode_FF = FirstFitFreeList::DLLNode;
const std::size_t NODESIZE_FF = FirstFitMemoryAllocator::node_size;
const std::size_t NODESIZE_PA = PoolAllocationMemoryAllocator<0>::node_size;
const std::size_t NODESIZE_BS = BuddySystemMemoryAllocator<0>::node_size;

const std::array<std::string, 5> STRINGS = {"FirstFit:\t\t\t", "NextFit:\t\t\t", "PoolAllocation:\t\t\t", "BuddySystem:\t\t\t", "BuddySystem (large alloc):\t"};

TEST(Allocation, First_NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_PA)> arr3;
    PoolAllocationMemoryAllocator<8> pama(arr3);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_BS)> arr4;
    BuddySystemMemoryAllocator<8> bsma(arr4);

    std::array<std::uint8_t, 10*(bytes_alloc_large+NODESIZE_BS)> arr5;
    BuddySystemMemoryAllocator<8> bsma_large(arr5);

    std::array<MemoryAllocator*, 5> mem_allocs = {&ffma, &nfma, &pama, &bsma, &bsma_large};

    for (int n=0; n<sizeN.size(); n++)
    {
        std::cout << "N=" << sizeN[n] << "\n";
        for (int m=0; m<mem_allocs.size(); m++)
        {
            std::size_t bytes_number = bytes_alloc;
            if (m == 4)
            {
                bytes_number = bytes_alloc_large;
            }

            double time = 0;

            int i=0;
            while (i<sizeN[n])
            {
                auto start_time = std::chrono::high_resolution_clock::now();
                auto b = mem_allocs[m]->allocate(bytes_number);
                auto end_time = std::chrono::high_resolution_clock::now();
                
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

                mem_allocs[m]->deallocate(b);

                i++;
            }

            std::cout << "\t" << STRINGS[m] << time/1000000 << "ms\n";
        }
    }
}

TEST(Allocation, NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_PA)> arr3_1000;
    PoolAllocationMemoryAllocator<8> pama_1000(arr3_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_BS)> arr4_1000;
    BuddySystemMemoryAllocator<8> bsma_1000(arr4_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc_large+NODESIZE_BS)> arr5_1000;
    BuddySystemMemoryAllocator<8> bsma_large_1000(arr5_1000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_PA)> arr3_10000;
    PoolAllocationMemoryAllocator<8> pama_10000(arr3_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_BS)> arr4_10000;
    BuddySystemMemoryAllocator<8> bsma_10000(arr4_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc_large+NODESIZE_BS)> arr5_10000;
    BuddySystemMemoryAllocator<8> bsma_large_10000(arr5_10000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_PA)> arr3_50000;
    PoolAllocationMemoryAllocator<8> pama_50000(arr3_50000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_BS)> arr4_50000;
    BuddySystemMemoryAllocator<8> bsma_50000(arr4_50000);

    std::array<MemoryAllocator*, 14> mem_allocs = {
        &ffma_1000, &nfma_1000, &pama_1000, &bsma_1000, &bsma_large_1000,
        &ffma_10000, &nfma_10000, &pama_10000, &bsma_10000, &bsma_large_10000,
        &ffma_50000, &nfma_50000, &pama_50000, &bsma_50000
    };

    for (int n=0; n<sizeN.size(); n++)
    {
        std::cout << "N=" << sizeN[n] << "\n";
        for (int m=0; m<5; m++)
        {
            std::size_t bytes_number = bytes_alloc;
            if (m == 4)
            {
                if (n == 2)
                {
                    std::cout << "\t" << STRINGS[4] << "Buffer too large\n";
                    break;
                }
                bytes_number = bytes_alloc_large;
            }

            auto start_time = std::chrono::high_resolution_clock::now();
            int i=0;
            while (i<1000)
            {  
                mem_allocs[(5*n)+m]->allocate(bytes_number);
                i++;
            }
            auto end_time = std::chrono::high_resolution_clock::now();

            double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

            std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
        }
    }
}

TEST(Allocation, NFreeBlocks)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 57+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};
    
    std::array<std::uint8_t, 10 + (1999*bytes_alloc) + (2001*NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 10 + (1999*bytes_alloc) + (2001*NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 16008 + (2001*NODESIZE_PA)> arr3_1000;
    PoolAllocationMemoryAllocator<8> pama_1000(arr3_1000);

    std::array<std::uint8_t, 16008 + (2001*NODESIZE_BS)> arr4_1000;
    BuddySystemMemoryAllocator<8> bsma_1000(arr4_1000);

    std::array<std::uint8_t, 128072 + (16008*NODESIZE_BS)> arr5_1000;
    BuddySystemMemoryAllocator<8> bsma_large_1000(arr5_1000);

    std::array<std::uint8_t, 9 + (20000*(1+NODESIZE_FF)) + NODESIZE_FF> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 9 + (20000*(1+NODESIZE_FF)) + NODESIZE_FF> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 160008 + (20001*NODESIZE_PA)> arr3_10000;
    PoolAllocationMemoryAllocator<8> pama_10000(arr3_10000);

    std::array<std::uint8_t, 9 + (100000*(1+NODESIZE_FF)) + NODESIZE_FF> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 9 + (100000*(1+NODESIZE_FF)) + NODESIZE_FF> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<MemoryAllocator*, 10> mem_allocs = {
        &ffma_1000, &nfma_1000, &pama_1000, &bsma_1000, &bsma_large_1000,
        &ffma_10000, &nfma_10000, &pama_10000,
        &ffma_50000, &nfma_50000
    };

    std::cout << "N=" << sizeN[0] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }

        std::array<void*, 1000> allocs;

        auto addr = mem_allocs[m]->allocate(bytes_number+1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[0]-2) && addr != nullptr)
        {  
            mem_allocs[m]->allocate(bytes_number);
            addr = mem_allocs[m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[0]-2))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[m]->allocate(bytes_number);

        auto addr2 = mem_allocs[m]->allocate(bytes_number+7);
        allocs[(sizeN[0]-1)] = mem_allocs[m]->allocate(bytes_number+7);

        mem_allocs[m]->allocate(bytes_number+1);
        
        int i2=0;
        while (i2<(sizeN[0]-2))
        {
            mem_allocs[m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[m]->allocate(8);
        auto end_time = std::chrono::high_resolution_clock::now();

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[1] << "\n";
    for (int m=0; m<3; m++)
    {
        const std::size_t bytes_number = bytes_alloc;
        
        std::array<void*, 10000> allocs;

        auto addr = mem_allocs[5+m]->allocate(bytes_number+1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[1]-2) && addr != nullptr)
        {  
            mem_allocs[5+m]->allocate(bytes_number);
            addr = mem_allocs[5+m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[1]-2))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[5+m]->allocate(bytes_number);

        auto addr2 = mem_allocs[5+m]->allocate(bytes_number+7);
        allocs[(sizeN[1]-1)] = mem_allocs[5+m]->allocate(bytes_number+7);

        mem_allocs[5+m]->allocate(bytes_number+1);
        
        int i2=0;
        while (i2<(sizeN[1]-2))
        {
            mem_allocs[5+m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[5+m]->allocate(8);
        auto end_time = std::chrono::high_resolution_clock::now();

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[3] << "Buffer too large\n";
    std::cout << "\t" << STRINGS[4] << "Buffer too large\n";

    std::cout << "N=" << sizeN[2] << "\n";
    for (int m=0; m<2; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        std::array<void*, 50000> allocs;

        auto addr = mem_allocs[8+m]->allocate(bytes_number+1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[2]-2) && addr != nullptr)
        {  
            mem_allocs[8+m]->allocate(bytes_number);
            addr = mem_allocs[8+m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[2]-2))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[8+m]->allocate(bytes_number);

        auto addr2 = mem_allocs[8+m]->allocate(bytes_number+7);
        allocs[(sizeN[2]-1)] = mem_allocs[8+m]->allocate(bytes_number+7);

        mem_allocs[8+m]->allocate(bytes_number+1);
        
        int i2=0;
        while (i2<(sizeN[2]-2))
        {
            mem_allocs[8+m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[8+m]->allocate(8);
        auto end_time = std::chrono::high_resolution_clock::now();

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[2] << "Buffer too large\n";
    std::cout << "\t" << STRINGS[3] << "Buffer too large\n";
    std::cout << "\t" << STRINGS[4] << "Buffer too large\n";
}

TEST(Allocation, NoSpace_NFreeBlocks_TooSmall)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 57+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};
    
    std::array<std::uint8_t, 2000*(1+NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 2000*(1+NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 20000*(1+NODESIZE_FF)> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 20000*(1+NODESIZE_FF)> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 100000*(1+NODESIZE_FF)> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 100000*(1+NODESIZE_FF)> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<MemoryAllocator*, 6> mem_allocs = {
        &ffma_1000, &nfma_1000,
        &ffma_10000, &nfma_10000,
        &ffma_50000, &nfma_50000
    };

    std::cout << "N=" << sizeN[0] << "\n";
    for (int m=0; m<2; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        std::array<void*, 1000> allocs;

        auto addr = mem_allocs[m]->allocate(1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[0]-1) && addr != nullptr)
        {  
            mem_allocs[m]->allocate(bytes_number);
            addr = mem_allocs[m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[0]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[m]->allocate(bytes_number);

        int i2=0;
        while (i2<sizeN[0])
        {
            mem_allocs[m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[m]->allocate(8+(7*NODESIZE_BS));
        auto end_time = std::chrono::high_resolution_clock::now();

        EXPECT_EQ(b, nullptr);

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[2] << "N/A\n";
    std::cout << "\t" << STRINGS[3] << "N/A\n";
    std::cout << "\t" << STRINGS[4] << "N/A\n";

    std::cout << "N=" << sizeN[1] << "\n";
    for (int m=0; m<2; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        std::array<void*, 10000> allocs;

        auto addr = mem_allocs[2+m]->allocate(1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[1]-1) && addr != nullptr)
        {  
            mem_allocs[2+m]->allocate(bytes_number);
            addr = mem_allocs[2+m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[1]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[2+m]->allocate(bytes_number);

        int i2=0;
        while (i2<sizeN[1])
        {
            mem_allocs[2+m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[2+m]->allocate(8+(7*NODESIZE_BS));
        auto end_time = std::chrono::high_resolution_clock::now();

        EXPECT_EQ(b, nullptr);

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[2] << "N/A\n";
    std::cout << "\t" << STRINGS[3] << "N/A\n";
    std::cout << "\t" << STRINGS[4] << "N/A\n";

    std::cout << "N=" << sizeN[2] << "\n";
    for (int m=0; m<2; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        std::array<void*, 50000> allocs;

        auto addr = mem_allocs[4+m]->allocate(1);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[2]-1) && addr != nullptr)
        {  
            mem_allocs[4+m]->allocate(bytes_number);
            addr = mem_allocs[4+m]->allocate(bytes_number);

            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[2]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        mem_allocs[4+m]->allocate(bytes_number);

        int i2=0;
        while (i2<sizeN[2])
        {
            mem_allocs[4+m]->deallocate(allocs[i2]);
            i2++;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto b = mem_allocs[4+m]->allocate(8+(7*NODESIZE_BS));
        auto end_time = std::chrono::high_resolution_clock::now();

        EXPECT_EQ(b, nullptr);

        double time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[2] << "N/A\n";
    std::cout << "\t" << STRINGS[3] << "N/A\n";
    std::cout << "\t" << STRINGS[4] << "N/A\n";
}

TEST(Deallocation, First_NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_PA)> arr3;
    PoolAllocationMemoryAllocator<8> pama(arr3);

    std::array<std::uint8_t, 10*(bytes_alloc+NODESIZE_BS)> arr4;
    BuddySystemMemoryAllocator<8> bsma(arr4);

    std::array<std::uint8_t, 10*(bytes_alloc_large+NODESIZE_BS)> arr5;
    BuddySystemMemoryAllocator<8> bsma_large(arr5);

    std::array<MemoryAllocator*, 5> mem_allocs = {&ffma, &nfma, &pama, &bsma, &bsma_large};

    for (int n=0; n<sizeN.size(); n++)
    {
        std::cout << "N=" << sizeN[n] << "\n";
        for (int m=0; m<mem_allocs.size(); m++)
        {
            std::size_t bytes_number = bytes_alloc;
            if (m == 4)
            {
                bytes_number = bytes_alloc_large;
            }

            double time = 0;

            int i=0;
            while (i<sizeN[n])
            {
                auto b = mem_allocs[m]->allocate(bytes_number);

                auto start_time = std::chrono::high_resolution_clock::now();
                mem_allocs[m]->deallocate(b);
                auto end_time = std::chrono::high_resolution_clock::now();
                
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

                i++;
            }

            std::cout << "\t" << STRINGS[m] << time/1000000 << "ms\n";
        }
    }
}

TEST(Deallocation, MergePrev_NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};
    
    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 1000*(8+NODESIZE_BS)> arr4_1000;
    BuddySystemMemoryAllocator<8> bsma_1000(arr4_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc_large+NODESIZE_BS)> arr5_1000;
    BuddySystemMemoryAllocator<8> bsma_large_1000(arr5_1000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 10000*(8+NODESIZE_BS)> arr4_10000;
    BuddySystemMemoryAllocator<8> bsma_10000(arr4_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc_large+NODESIZE_BS)> arr5_10000;
    BuddySystemMemoryAllocator<8> bsma_large_10000(arr5_10000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<std::uint8_t, 50000*(8+NODESIZE_BS)> arr4_50000;
    BuddySystemMemoryAllocator<8> bsma_50000(arr4_50000);

    std::array<MemoryAllocator*, 14> mem_allocs = {
        &ffma_1000, &nfma_1000, nullptr, &bsma_1000, &bsma_large_1000,
        &ffma_10000, &nfma_10000, nullptr, &bsma_10000, &bsma_large_10000,
        &ffma_50000, &nfma_50000, nullptr, &bsma_50000
    };

    std::cout << "N=" << sizeN[0] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 1000> allocs;

        auto addr = mem_allocs[m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[0]-1) && addr != nullptr)
        {
            addr = mem_allocs[m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[0]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[0])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[m]->deallocate(allocs[i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[1] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 10000> allocs;

        auto addr = mem_allocs[5+m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[1]-1) && addr != nullptr)
        {
            addr = mem_allocs[5+m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[1]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[1])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[5+m]->deallocate(allocs[i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[2] << "\n";
    for (int m=0; m<5; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        if (m == 2)
        {
            std::cout << "\t" << STRINGS[m] << "N/A\n";
            continue;
        }
        else if (m == 4)
        {
            std::cout << "\t" << STRINGS[m] << "Buffer too large\n";
            break;
        }
        
        std::array<void*, 50000> allocs;

        auto addr = mem_allocs[10+m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[2]-1) && addr != nullptr)
        {
            addr = mem_allocs[10+m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[2]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[2])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[10+m]->deallocate(allocs[i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
}

TEST(Deallocation, MergeNext_NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};
    
    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc+NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 1000*(8+NODESIZE_BS)> arr4_1000;
    BuddySystemMemoryAllocator<8> bsma_1000(arr4_1000);

    std::array<std::uint8_t, 1000*(bytes_alloc_large+NODESIZE_BS)> arr5_1000;
    BuddySystemMemoryAllocator<8> bsma_large_1000(arr5_1000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc+NODESIZE_FF)> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 10000*(8+NODESIZE_BS)> arr4_10000;
    BuddySystemMemoryAllocator<8> bsma_10000(arr4_10000);

    std::array<std::uint8_t, 10000*(bytes_alloc_large+NODESIZE_BS)> arr5_10000;
    BuddySystemMemoryAllocator<8> bsma_large_10000(arr5_10000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 50000*(bytes_alloc+NODESIZE_FF)> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<std::uint8_t, 50000*(8+NODESIZE_BS)> arr4_50000;
    BuddySystemMemoryAllocator<8> bsma_50000(arr4_50000);

    std::array<MemoryAllocator*, 14> mem_allocs = {
        &ffma_1000, &nfma_1000, nullptr, &bsma_1000, &bsma_large_1000,
        &ffma_10000, &nfma_10000, nullptr, &bsma_10000, &bsma_large_10000,
        &ffma_50000, &nfma_50000, nullptr, &bsma_50000
    };

    std::cout << "N=" << sizeN[0] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 1000> allocs;

        auto addr = mem_allocs[m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[0]-1) && addr != nullptr)
        {
            addr = mem_allocs[m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[0]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[0])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[m]->deallocate(allocs[sizeN[0]-1-i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[1] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 10000> allocs;

        auto addr = mem_allocs[5+m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[1]-1) && addr != nullptr)
        {
            addr = mem_allocs[5+m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[1]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[1])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[5+m]->deallocate(allocs[sizeN[1]-1-i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[2] << "\n";
    for (int m=0; m<5; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        if (m == 2)
        {
            std::cout << "\t" << STRINGS[m] << "N/A\n";
            continue;
        }
        else if (m == 4)
        {
            std::cout << "\t" << STRINGS[m] << "Buffer too large\n";
            break;
        }
        
        std::array<void*, 50000> allocs;

        auto addr = mem_allocs[10+m]->allocate(bytes_number);
        allocs[0] = addr;
        
        int i1=0;
        while (i1<(sizeN[2]-1) && addr != nullptr)
        {
            addr = mem_allocs[10+m]->allocate(bytes_number);
            allocs[i1+1] = addr;

            i1++;
        }
        if (i1 < (sizeN[2]-1))
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[2])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[10+m]->deallocate(allocs[sizeN[2]-1-i2]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i2++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
}

TEST(Deallocation, MergePrevNext_NTimes)
{
    const std::size_t bytes_alloc = 1;
    const std::size_t bytes_alloc_large = 64+(7*NODESIZE_BS);

    const std::array<std::size_t, 3> sizeN = {1000, 10000, 50000};
    
    std::array<std::uint8_t, 2001*(bytes_alloc+NODESIZE_FF)> arr1_1000;
    FirstFitMemoryAllocator ffma_1000(arr1_1000);

    std::array<std::uint8_t, 2001*(bytes_alloc+NODESIZE_FF)> arr2_1000;
    NextFitMemoryAllocator nfma_1000(arr2_1000);

    std::array<std::uint8_t, 2001*(8+NODESIZE_BS)> arr4_1000;
    BuddySystemMemoryAllocator<8> bsma_1000(arr4_1000);

    std::array<std::uint8_t, 2001*(bytes_alloc_large+NODESIZE_BS)> arr5_1000;
    BuddySystemMemoryAllocator<8> bsma_large_1000(arr5_1000);

    std::array<std::uint8_t, 20001*(bytes_alloc+NODESIZE_FF)> arr1_10000;
    FirstFitMemoryAllocator ffma_10000(arr1_10000);

    std::array<std::uint8_t, 20001*(bytes_alloc+NODESIZE_FF)> arr2_10000;
    NextFitMemoryAllocator nfma_10000(arr2_10000);

    std::array<std::uint8_t, 20001*(8+NODESIZE_BS)> arr4_10000;
    BuddySystemMemoryAllocator<8> bsma_10000(arr4_10000);

    std::array<std::uint8_t, 100001*(bytes_alloc+NODESIZE_FF)> arr1_50000;
    FirstFitMemoryAllocator ffma_50000(arr1_50000);

    std::array<std::uint8_t, 100001*(bytes_alloc+NODESIZE_FF)> arr2_50000;
    NextFitMemoryAllocator nfma_50000(arr2_50000);

    std::array<MemoryAllocator*, 12> mem_allocs = {
        &ffma_1000, &nfma_1000, nullptr, &bsma_1000, &bsma_large_1000,
        &ffma_10000, &nfma_10000, nullptr, &bsma_10000, nullptr,
        &ffma_50000, &nfma_50000
    };

    std::cout << "N=" << sizeN[0] << "\n";
    for (int m=0; m<5; m++)
    {
        std::size_t bytes_number = bytes_alloc;
        if (m == 4)
        {
            bytes_number = bytes_alloc_large;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 1001> allocs1;
        std::array<void*, 1000> allocs2;

        auto addr = mem_allocs[m]->allocate(1);
        allocs1[0] = addr;
        
        int i1=0;
        while (i1<sizeN[0] && addr != nullptr)
        {
            addr = mem_allocs[m]->allocate(bytes_number);
            allocs2[i1] = addr;

            addr = mem_allocs[m]->allocate(bytes_number);
            allocs1[i1+1] = addr;

            i1++;
        }
        if (i1 < sizeN[0])
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[0])
        {
            mem_allocs[m]->deallocate(allocs1[i2]);
            i2++;
        }

        int i3=0;
        while (i3<sizeN[0])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[m]->deallocate(allocs2[i3]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i3++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[1] << "\n";
    for (int m=0; m<5; m++)
    {
        const std::size_t bytes_number = bytes_alloc;

        if (m == 4)
        {
            std::cout << "\t" << STRINGS[4] << "Buffer too large\n";
            break;
        }
        else if (m == 2)
        {
            std::cout << "\t" << STRINGS[2] << "N/A\n";
            continue;
        }
        
        std::array<void*, 10001> allocs1;
        std::array<void*, 10000> allocs2;

        auto addr = mem_allocs[5+m]->allocate(1);
        allocs1[0] = addr;
        
        int i1=0;
        while (i1<sizeN[1] && addr != nullptr)
        {
            addr = mem_allocs[5+m]->allocate(bytes_number);
            allocs2[i1] = addr;

            addr = mem_allocs[5+m]->allocate(bytes_number);
            allocs1[i1+1] = addr;

            i1++;
        }
        if (i1 < sizeN[1])
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[1])
        {
            mem_allocs[5+m]->deallocate(allocs1[i2]);
            i2++;
        }

        int i3=0;
        while (i3<sizeN[1])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[5+m]->deallocate(allocs2[i3]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i3++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }

    std::cout << "N=" << sizeN[2] << "\n";
    for (int m=0; m<2; m++)
    {
        const std::size_t bytes_number = bytes_alloc;
        
        std::array<void*, 50001> allocs1;
        std::array<void*, 50000> allocs2;

        auto addr = mem_allocs[10+m]->allocate(1);
        allocs1[0] = addr;
        
        int i1=0;
        while (i1<sizeN[2] && addr != nullptr)
        {
            addr = mem_allocs[10+m]->allocate(bytes_number);
            allocs2[i1] = addr;

            addr = mem_allocs[10+m]->allocate(bytes_number);
            allocs1[i1+1] = addr;

            i1++;
        }
        if (i1 < sizeN[2])
        {
            std::cout << "nullptr at i1=" << i1 << "\n";
        }

        double time = 0;

        int i2=0;
        while (i2<sizeN[2])
        {
            mem_allocs[10+m]->deallocate(allocs1[i2]);
            i2++;
        }

        int i3=0;
        while (i3<sizeN[2])
        {
            auto start_time = std::chrono::high_resolution_clock::now();
            mem_allocs[10+m]->deallocate(allocs2[i3]);
            auto end_time = std::chrono::high_resolution_clock::now();
            
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
            
            i3++;
        }
        
        std::cout << "\t" << STRINGS[m%5] << time/1000000 << "ms\n";
    }
    std::cout << "\t" << STRINGS[2] << "N/A\n";
    std::cout << "\t" << STRINGS[3] << "Buffer too large\n";
    std::cout << "\t" << STRINGS[4] << "Buffer too large\n";
}