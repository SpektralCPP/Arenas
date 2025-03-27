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

## Benchmark Results
Benchmark availabe [here](tests/perf/main.cpp)

```
-----------------------------------------------------------------------------------------------
Benchmark                                                     Time             CPU   Iterations
-----------------------------------------------------------------------------------------------
malloc_test/iterations:10000/repeats:50_mean             755951 ns       742367 ns           50
malloc_test/iterations:10000/repeats:50_median           749422 ns       736097 ns           50
malloc_test/iterations:10000/repeats:50_stddev            49129 ns        48088 ns           50
malloc_test/iterations:10000/repeats:50_cv                 6.50 %          6.48 %            50
linear_alloc_test/iterations:10000/repeats:50_mean       661096 ns       649307 ns           50
linear_alloc_test/iterations:10000/repeats:50_median     640215 ns       628759 ns           50
linear_alloc_test/iterations:10000/repeats:50_stddev      96940 ns        95213 ns           50
linear_alloc_test/iterations:10000/repeats:50_cv          14.66 %         14.66 %            50
```
### Machine Details
- CPU: Intel Core i9 14900HX
- RAM: 32 GB @ 5600 MT/s
- OS: WSL
