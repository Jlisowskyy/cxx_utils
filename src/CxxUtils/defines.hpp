#ifndef CXX_UTILS_DEFINES_HPP_
#define CXX_UTILS_DEFINES_HPP_

#include <cstdint>

#define CXX_UTILS_DECL_START_ \
    namespace CxxUtils        \
    {
#define CXX_UTILS_DECL_END_ }

enum class AccessType : uint8_t {
    kThreadSafe,
    kNonThreadSafe,
};

#ifdef __GNUC__
#define FORCE_INLINE__ __attribute__((always_inline))
#elif defined(_MSC_VER)
#define FORCE_INLINE__ __forceinline
#elif defined(__clang__)
#define FORCE_INLINE__ __attribute__((always_inline))
#else
#warning "Unknown compile, force inline not supported"
#define FORCE_INLINE__
#endif

#define FAST_CALL_ inline FORCE_INLINE__
#define WRAP_CALL_ inline FORCE_INLINE__

#define NDSCRD_ [[nodiscard]]
#define UNUSED_ [[maybe_unused]]

#endif  // CXX_UTILS_DEFINES_HPP_
