#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "FirstFit/first_fit_memory_allocator.h"

TEST(eg, eg)
{
    std::array<std::uint8_t, 32> arr;

    FirstFitMemoryAllocator ff(arr);
}