#include "utils.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <unistd.h>
#include <utility>
#include <vector>

namespace Spektral::Arenas {

/**
 * @class LinearArena
 * @brief A simple memory arena for fast memory allocations.
 *
 * This class provides a linear allocator that allocates memory in a contiguous
 * block. It does not support deallocation of individual allocations but allows
 * resetting the entire arena.
 *
 * @note Only use with trivially destroyable types as destructors ARE NOT called
 * when the arena goes out of scope.
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
   * Allocates a contiguous block of memory of atleast the specified size.
   * Throws std::bad_alloc if allocation fails.
   *
   * @note Actual allocated size is based on `optimal_alloc(size)`
   */
  explicit LinearArena(size_t size) : current_offset_(0) {
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
   * @brief Creates and initializes a new object of type `T` on pre-allocated
   * memory.
   *
   * This function allocates raw memory sufficient to hold an instance of type
   * `T`, then constructs an object of that type in-place using the given
   * arguments. It is analogous to using placement `new`.
   *
   * @tparam T The type of the object to be constructed. Must have a constructor
   *           compatible with the provided argument types.
   * @tparam Args Variadic template parameters representing the types of
   * arguments to forward to the constructor of `T`.
   *
   * @param args Arguments to be forwarded to the constructor of `T`.
   *
   * @return A pointer to the newly constructed object of type `T`.
   *
   * @note This pointer is allocated throught the arena so it's life time is
   * tied to the arena. Destructors won't get called.
   */
  template <typename T, typename... Args> T *make(Args... args) {
    // placement new using the arena + forwarded arguments
    return new (static_cast<T *>(alloc(sizeof(T)))) T(std::forward(args)...);
  }

  /**
   * @brief Resets the memory arena.
   *
   * This function resets the arena by setting the allocation offset to zero,
   * effectively making all previously allocated memory available again.
   *
   * Destructors won't be called.
   */
  void reset() { current_offset_ = 0; }

private:
  size_t size_;           ///< The total size of the memory arena.
  size_t current_offset_; ///< The current offset in the memory arena.
  char *data = nullptr;   ///< Pointer to the allocated memory block.
};

} // namespace Spektral::Arenas
