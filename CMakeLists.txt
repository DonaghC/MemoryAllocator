cmake_minimum_required(VERSION 3.5)

project("MemoryAllocator")

add_subdirectory("external/googletest")
enable_testing()

include_directories("include/")
include_directories("external/googletest/googletest/include/")

#add_executable(firstfit_freelist_test test/FirstFit/free_list_tests.cpp)
#target_link_libraries(firstfit_freelist_test gtest gtest_main)
#add_test(firstfit_freelist_test firstfit_freelist_test)

add_executable(firstfit_test test/FirstFit/first_fit_tests.cpp)
target_link_libraries(firstfit_test gtest gtest_main)
add_test(firstfit_test firstfit_test)