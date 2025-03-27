# Spektral Arenas

A simple memory allocation framework to batch allocate/deallocate.

## Classes

* **LinearArena**: A simple memory arena for fast memory allocations. This class provides a linear allocator that allocates memory in a contiguous block. It does not support deallocation of individual allocations but allows resetting the entire arena.

## Usage

### LinearArena

* To create a `LinearArena`, you can specify the size in bytes:

    ```cpp
    Spektral::Arenas::LinearArena arena(1024); // Creates an arena of 1024 bytes
    ```

* To allocate memory:

    ```cpp
    void* ptr = arena.alloc(50); // Allocates 50 bytes
    int* arr = arena.alloc<int>(10); // Allocates an array of 10 integers
    ```

* To allocate and zero-initialize memory:

    ```cpp
    int* arr = arena.calloc<int>(10); // Allocates an array of 10 zero-initialized integers
    ```

* To reset the arena (freeing all allocations):

    ```cpp
    arena.reset();
    ```
