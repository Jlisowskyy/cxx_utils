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

#endif  // CXX_UTILS_DEFINES_HPP_
