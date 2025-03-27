#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <unistd.h>

namespace Spektral::Arenas {

/**
 * @class LinearArena
 * @brief A simple memory arena for fast memory allocations.
 *
 * This class provides a linear allocator that allocates memory in a contiguous
 * block. It does not support deallocation of individual allocations but allows
 * resetting the entire arena.
 */
class LinearArena {
public:
  /**
   * @brief Deleted default constructor.
   */
  LinearArena() = delete;

  /**
   * @brief Constructs a LinearArena with a given size.
   * @param size The total size of the memory arena in bytes. 4096 by default
   * since that can hold 1024 ints(arbitrary requirement choosen by me).
   *
   * Allocates a contiguous block of memory of the specified size. Throws
   * std::bad_alloc if allocation fails.
   */
  LinearArena(size_t size = 4096) : current_offset_(0) {
    size_ = optimal_alloc(size);
    data = static_cast<char *>(malloc(size));
    if (!data)
      throw std::bad_alloc();
  }

  /**
   * @brief Destructor that frees the allocated memory.
   */
  ~LinearArena() { free(data); }

  /**
   * @brief Allocates a block of memory from the arena.
   * @param size The number of bytes to allocate.
   * @return A pointer to the allocated memory, or nullptr if out of memory.
   *
   * Allocates memory from the arena in a linear fashion. If there is not enough
   * space left, it returns nullptr.
   */
  void *alloc(size_t size) {
    if (current_offset_ + size > size_)
      return nullptr;
    void *ptr = data + current_offset_;
    current_offset_ += size;
    return ptr;
  }

  /**
   * @brief Allocates memory for an array of objects of type T.
   * @tparam T The type of object to allocate.
   * @param count The number of objects to allocate.
   * @return A pointer to the allocated memory, or nullptr if out of memory.
   */
  template <typename T> T *alloc(size_t count) {
    return static_cast<T *>(alloc(sizeof(T) * count));
  }

  /**
   * @brief Allocates and zero-initializes memory for an array of objects of
   * type T.
   * @tparam T The type of object to allocate.
   * @param blocks The number of objects to allocate.
   * @return A pointer to the allocated memory, or nullptr if out of memory.
   *
   * Similar to calloc, this function allocates memory and initializes it to
   * zero.
   */
  template <typename T> T *calloc(size_t blocks) {
    if (current_offset_ + sizeof(T) * blocks > size_)
      return nullptr;
    memset(data + current_offset_, 0, sizeof(T) * blocks);
    void *ptr = data + current_offset_;
    current_offset_ += sizeof(T) * blocks;
    return static_cast<T *>(ptr);
  }

  /**
   * @brief Resets the memory arena.
   *
   * This function resets the arena by setting the allocation offset to zero,
   * effectively making all previously allocated memory available again.
   */
  void reset() { current_offset_ = 0; }

private:
  size_t size_;           ///< The total size of the memory arena.
  size_t current_offset_; ///< The current offset in the memory arena.
  char *data = nullptr;   ///< Pointer to the allocated memory block.

private:
  /**
   * @brief Calculates the optimal allocation size based on user-defined
   * requirements.
   *
   * This function determines the most efficient memory block size for a given
   * request, adhering to specific internal rules defined by malloc. It
   * considers various conditions and constraints to return an appropriate block
   * size:
   * - For sizes less than 1MB, it uses powers of two in the range [2^5, 2^13).
   * - For sizes equal or greater than 1MB, it calculates based on a minimum of
   * 1MB plus multiples of 512KB.
   * - If none of the above conditions apply, it defaults to rounding up to the
   * nearest page size multiple, where the page size is obtained from the system
   * configuration.
   *
   * @param user_sz The requested memory size in bytes.
   * @return A size_t value representing the optimal block size for allocation.
   *
   * @note This function internally uses `ceil` for rounding and `sysconf` to
   * retrieve the system's page size. Make sure that these are available or
   * appropriately defined in your environment. Also, this function internally
   * relies on a for loop that can very easily be unrolled for more perf
   * so try doing that.
   */
  static size_t optimal_alloc(size_t user_sz) {
    // Malloc internally provides the following list of block sizes:
    // 2**N; N in [5, 13) or (32, 4096]
    for (int ii = 5; ii < 13; ++ii)
      if (user_sz < 1 << ii)
        return 1 << ii;
    // If greater than 1MB, it will be 1MB + 512KB * K
    if (user_sz >= 1 << 20) {
      return (1 << 20) + (512 * 1 << 10) *
                             ceil(user_sz - (1 << 20) * 1.0 / (512 * 1 << 10));
    }
    // Or, multiples of the page size
    long page_size = sysconf(_SC_PAGE_SIZE);
    return ceil(user_sz * 1.0 / page_size) * page_size;
  }
};

} // namespace Spektral::Arenas
