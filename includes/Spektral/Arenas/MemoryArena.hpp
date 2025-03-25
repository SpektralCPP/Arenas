#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>

namespace Spektral::Arenas {

/**
 * @class MemoryArena
 * @brief A simple memory arena for fast memory allocations.
 *
 * This class provides a linear allocator that allocates memory in a contiguous
 * block. It does not support deallocation of individual allocations but allows
 * resetting the entire arena.
 */
class MemoryArena {
public:
  /**
   * @brief Deleted default constructor.
   */
  MemoryArena() = delete;

  /**
   * @brief Constructs a MemoryArena with a given size.
   * @param size The total size of the memory arena in bytes.
   *
   * Allocates a contiguous block of memory of the specified size. Throws
   * std::bad_alloc if allocation fails.
   */
  MemoryArena(size_t size) : size_(size), current_offset_(0) {
    data = static_cast<char *>(malloc(size));
    if (!data)
      throw std::bad_alloc();
  }

  /**
   * @brief Destructor that frees the allocated memory.
   */
  ~MemoryArena() { free(data); }

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
};

} // namespace Spektral::Arenas
