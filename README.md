# Memory Allocator

This project contains implementations for 4 different memory allocators:
- FirstFitMemoryAllocator
- NextFitMemoryAllocator
- PoolAllocationMemoryAllocator
- BuddySystemMemoryAllocator

Each memory allocator implements the abstract class MemoryAllocator. This ensures that each class implements the following:
- allocate
- deallocate
- allocated
- length
- reset

Having a common design for all the memory allocators makes them easy to use as they all operate in the same way.

## Methods
### allocate

This method allocates a number of bytes in the memory buffer and returns the address of the allocated block of memory. It is where the memory allocators differ the most, i.e. this is where the first fit, pool allocation, etc. algorithms are implemented.

The signature for this method must be as follows:
> virtual void* allocate(std::size_t bytes) = 0;
- The method must return a void pointer to the allocated memory. A void pointer is used as the memory can be used to store any type.
- The parameter is 'bytes' and is of type 'std::size_t', which is simply an unsigned integer. This parameter specifies how many bytes to allocate in the memory buffer.

### deallocate

This method deallocates a previously allocated block of memory, freeing it up to be allocated again. It is very similar in all the memory allocators since all it's doing is adding another block to the free list (list of blocks of memory available for allocation). Each memory allocator maintains their free list differently though so the deallocate method varies slightly.

The signature for this method must be as follows:
> virtual void deallocate(void* addr) = 0;
- The parameter 'addr' is of type 'void*', which is a void pointer to the address of the memory, i.e. the address that would've been returned when allocate was initially called to get this block of memory.

### allocated

This method simply returns the number of bytes allocated/used in the memory buffer. Since all the memory allocators use a free list to keep track of free blocks, the space used by these free blocks is also factored into the value returned by this method. For example, the free blocks used for first fit store more information than those used in pool allocation, so they take up more space. It is the same for every memory allocator.

The signature for this method must be as follows:
> virtual std::size_t allocated() const = 0;
- The method returns an unsigned integer, which is the number of bytes being used in the memory buffer.
- The method is also marked as 'const', which means that it cannot alter any variables within it's body.

### length

This method simply returns the total number of bytes in the memory buffer. It is the same for every memory allocator.

The signature for this method must be as follows:
> virtual std::size_t length() const = 0;
- The method returns an unsigned integer, which is the total number of bytes in the memory buffer.
- The method is also marked as 'const', which means that it cannot alter any variables within it's body.

### reset

This method marks deallocates all blocks and sets all member variables to their initial values. This method varies slightly for each memory allocator as they all have different member variables and they all maintain their free lists differently.

The signature for this method must be as follows:
> virtual void reset() = 0;

## How to build

To pull from GitHub:

```
git clone https://github.com/deniscasey/MemoryAllocator.git
cd MemoryAllocator
git submodule update --init --recursive
```

This project is built using CMake as follows:

```
mkdir tmp
cd tmp
cmake ..
make
```

Note: All the warnings are simply from my use of void pointers.

From here you can run any of the test files:

```
./TEST_FILE
```