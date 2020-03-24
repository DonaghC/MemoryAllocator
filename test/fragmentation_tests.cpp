#include <array>
#include <chrono>
#include <cstddef>
#include <string>
#include <vector>
#include <utility>

#include <gtest/gtest.h>

#include "memory_allocator.h"
#include "FirstFit/first_fit_memory_allocator.h"
#include "NextFit/next_fit_memory_allocator.h"
#include "PoolAllocation/pool_allocation_memory_allocator.h"
#include "BuddySystem/buddy_system_memory_allocator.h"

const std::size_t NODESIZE_FF = FirstFitMemoryAllocator::node_size;
const std::size_t NODESIZE_PA = PoolAllocationMemoryAllocator<0>::node_size;
const std::size_t NODESIZE_BS = BuddySystemMemoryAllocator<0>::node_size;

const std::array<std::string, 6> ROWS = {"FirstFit:\t\t\t", "NextFit:\t\t\t", "PoolAllocation:\t\t\t", "PoolAllocation (scaled buffer):\t", "BuddySystem:\t\t\t", "BuddySystem (scaled buffer):\t"};

TEST(Efficiency, Allocate_NBytes)
{
    const std::array<std::size_t, 3> bytes_alloc = {1, 8, 64+(7*NODESIZE_BS)};

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr3;
    PoolAllocationMemoryAllocator<8> pama(arr3);

    std::array<std::uint8_t, 10000*(8+NODESIZE_PA)> arr3_scaled;
    PoolAllocationMemoryAllocator<8> pama_scaled(arr3_scaled);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr4;
    BuddySystemMemoryAllocator<8> bsma(arr4);

    std::array<std::uint8_t, 10000*(8+NODESIZE_BS)> arr4_scaled;
    BuddySystemMemoryAllocator<8> bsma_scaled(arr4_scaled);

    std::array<MemoryAllocator*, 6> mem_allocs = {&ffma, &nfma, &pama, &pama_scaled, &bsma, &bsma_scaled};

    std::cout << "\t\t\t\t\tAllocations\tBytes allocated\tCapacity\n\t\t\t\t\t\t\t(without nodes)\t(without nodes)\n";
    for (int b=0; b<bytes_alloc.size(); b++)
    {
        std::cout << "N=" << bytes_alloc[b] << "\n";
        for (int m=0; m<mem_allocs.size(); m++)
        {
            if (b == 2 && (m == 2 || m == 3))
            {
                std::cout << "\t" << ROWS[m%6] << "Blocksize too small\n";
                continue;
            }

            std::size_t count = 0;

            void* addr;
            do
            {
                addr = mem_allocs[m]->allocate(bytes_alloc[b]);
                count++;
            }
            while (addr != nullptr);

            std::cout << "\t" << ROWS[m%6] << count << "\t\t";
            std::cout << (count*bytes_alloc[b]) << "B\t\t";
            std::cout << 100*(static_cast<double>((count*bytes_alloc[b]))/static_cast<double>(mem_allocs[m]->length())) << "%\n";

            mem_allocs[m]->reset();
        }
    }
}

TEST(Efficiency, AllocateDeallocateP_NBytes)
{
    const std::array<std::size_t, 3> deallocation_freq = {0, 3, 15};
    const std::array<std::array<std::size_t, 2>, 4> byte_ranges = {
        std::array<std::size_t, 2>{1, 8}, 
        std::array<std::size_t, 2>{9, 16+NODESIZE_BS}, 
        std::array<std::size_t, 2>{32+(3*NODESIZE_BS), 64+(7*NODESIZE_BS)},
        std::array<std::size_t, 2>{1, 64+(7*NODESIZE_BS)}
    };

    const time_t seed = time(NULL);
    std::cout << "Seed: " << seed << "\n";

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr3;
    PoolAllocationMemoryAllocator<8> pama(arr3);

    std::array<std::uint8_t, 10000*(8+NODESIZE_PA)> arr3_scaled;
    PoolAllocationMemoryAllocator<8> pama_scaled(arr3_scaled);

    std::array<std::uint8_t, 10000*(8+NODESIZE_FF)> arr4;
    BuddySystemMemoryAllocator<8> bsma(arr4);

    std::array<std::uint8_t, 10000*(8+NODESIZE_BS)> arr4_scaled;
    BuddySystemMemoryAllocator<8> bsma_scaled(arr4_scaled);

    std::array<MemoryAllocator*, 6> mem_allocs = {&ffma, &nfma, &pama, &pama_scaled, &bsma, &bsma_scaled};

    std::cout << "\t\t\t\t\tAllocations\tBytes allocated\tCapacity\n";
    std::cout << "\t\t\t\t\t\t\t(without nodes)\t(inc nodes)\n";
    for (int b=0; b<byte_ranges.size(); b++)
    {
        const std::size_t lower_bound = byte_ranges[b][0];
        const std::size_t upper_bound = byte_ranges[b][1];

        std::cout << "N=[" << byte_ranges[b][0] << ", " << byte_ranges[b][1] << "]\n";
        for (int p=0; p<deallocation_freq.size(); p++)
        {
            std::cout << "P=" << deallocation_freq[p] << "\n";
            for (int m=0; m<mem_allocs.size(); m++)
            {
                if (b > 0 && (m == 2 || m == 3))
                {
                    std::cout << "\t" << ROWS[m%6] << "Blocksize too small\n";
                    continue;
                }

                srand(seed);

                std::vector<void*> allocs;

                std::size_t count = 0;
                std::size_t bytes_allocated = 0;

                void* addr;
                do
                {
                    std::size_t bytes = (rand()%(upper_bound-lower_bound+1)) + lower_bound;

                    addr = mem_allocs[m]->allocate(bytes);
                    count++;
                    bytes_allocated += bytes;

                    allocs.push_back(addr);

                    if (p != 0 && count > 1 && count%deallocation_freq[p] == 0)
                    {
                        std::size_t pos = rand() % allocs.size();
                        mem_allocs[m]->deallocate(allocs[pos]);
                        allocs.erase(allocs.begin() + pos);
                    }
                }
                while (addr != nullptr);

                std::cout << "\t" << ROWS[m%6] << count << "\t\t";
                std::cout << bytes_allocated << "B\t\t";
                std::cout << 100*(static_cast<double>(mem_allocs[m]->allocated())/static_cast<double>(mem_allocs[m]->length())) << "%\n";
                
                mem_allocs[m]->reset();
            }
        }
        std::cout << "\n";
    }
}

TEST(Segmentation, CapacityC_NBytes)
{
    const std::array<float, 3> cap_limits = {0.7, 0.8, 0.9};
    const std::array<std::array<std::size_t, 2>, 4> byte_ranges = {
        std::array<std::size_t, 2>{1, 8}, 
        std::array<std::size_t, 2>{9, 16+NODESIZE_BS}, 
        std::array<std::size_t, 2>{32+(3*NODESIZE_BS), 64+(7*NODESIZE_BS)},
        std::array<std::size_t, 2>{1, 64+(7*NODESIZE_BS)}
    };

    const time_t seed = time(NULL);
    std::cout << "Seed: " << seed << "\n";

    std::array<std::uint8_t, 1000*(8+NODESIZE_FF)> arr1;
    FirstFitMemoryAllocator ffma(arr1);

    std::array<std::uint8_t, 1000*(8+NODESIZE_FF)> arr2;
    NextFitMemoryAllocator nfma(arr2);

    std::array<MemoryAllocator*, 2> mem_allocs = {&ffma, &nfma};

    std::cout << "\t\t\t\t\tAllocations\tCapacity\n";
    for (int b=0; b<byte_ranges.size(); b++)
    {
        const std::size_t lower_bound = byte_ranges[b][0];
        const std::size_t upper_bound = byte_ranges[b][1];

        std::cout << "N=[" << byte_ranges[b][0] << ", " << byte_ranges[b][1] << "]\n";
        for (int c=0; c<cap_limits.size(); c++)
        {
            const std::size_t limit = ffma.length() * cap_limits[c];

            std::cout << "C=" << cap_limits[c] << "\n";
            for (int m=0; m<mem_allocs.size(); m++)
            {
                if (m >= 2)
                {
                    std::cout << "\t" << ROWS[m%6] << "N/A\n";
                    continue;
                }

                srand(seed);

                std::vector<void*> allocs;

                std::size_t count = 0;

                while (mem_allocs[m]->allocated() <= limit)
                {
                    std::size_t bytes = (rand()%(upper_bound-lower_bound+1)) + lower_bound;
                    allocs.push_back(mem_allocs[m]->allocate(bytes));
                }

                std::size_t pos = rand() % (allocs.size()-1);
                while (allocs.at(pos) != nullptr)
                {
                    mem_allocs[m]->deallocate(allocs.at(pos));
                    allocs.erase(allocs.begin() + pos);

                    std::size_t bytes = (rand()%(upper_bound-lower_bound+1)) + lower_bound;
                    allocs.push_back(mem_allocs[m]->allocate(bytes));

                    pos = rand() % allocs.size();

                    count++;
                }

                std::cout << "\t" << ROWS[m%6] << count << "\t\t";
                std::cout << 100*(static_cast<double>(mem_allocs[m]->allocated()) / static_cast<double>(mem_allocs[m]->length())) << "%\n";

                mem_allocs[m]->reset();
            }
        }
    }
}
