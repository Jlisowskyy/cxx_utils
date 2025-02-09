#ifndef CXX_UTILS_TYPE_CHECKS_HPP_
#define CXX_UTILS_TYPE_CHECKS_HPP_

#include <CxxUtils/defines.hpp>

#include <cstdlib>
#include <type_traits>

CXX_UTILS_DECL_START_

template <typename T, typename... Args>
constexpr size_t CountType()
{
    return (static_cast<size_t>(std::is_same_v<T, Args>) + ...);
}

template <typename T, typename... Args>
constexpr bool HasType()
{
    return (std::is_same_v<T, Args> || ...);
}

template <typename T, typename... Args>
constexpr bool HasTypeOnce()
{
    return CountType<T, Args...>() == 1;
}

template <typename T, typename... Args>
constexpr bool HasDuplicateType()
{
    return CountType<T, Args...>() > 1;
}

template <typename... Args>
constexpr bool HasDuplicateTypes()
{
    return (HasDuplicateType<Args, Args...>() || ...);
}

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_TYPE_CHECKS_HPP_
