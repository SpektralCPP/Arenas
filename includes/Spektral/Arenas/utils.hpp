#include <cmath>
#include <cstddef>
#include <unistd.h>

namespace Spektral::Arenas {
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
  inline size_t optimal_alloc(size_t user_sz) {
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
}
