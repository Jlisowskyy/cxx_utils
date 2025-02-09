#ifndef CXX_UTILS_TEMPLATE_HELPERS_HPP_
#define CXX_UTILS_TEMPLATE_HELPERS_HPP_

#include <CxxUtils/defines.hpp>

#include <cstdint>
#include <cstdlib>
#include <iostream>

CXX_UTILS_DECL_START_

template <class ExprT, uint64_t kMaxValue, uint64_t kStep = 1, class FuncT, class... Args>
void RolledSwitch(FuncT&& func, const uint64_t value, Args&&... args)
{
    if (value == kMaxValue) {
        func.template operator()<static_cast<ExprT>(kMaxValue)>(std::forward<Args>(args)...);
        return;
    }

    if constexpr (kMaxValue >= kStep) {
        RolledSwitch<ExprT, kMaxValue - kStep, kStep>(std::forward<FuncT>(func), value, std::forward<Args>(args)...);
        return;
    }

    std::cerr << "Error: RolledSwitch: value " << value << " is out of range" << std::endl;
    std::abort();
}

template <class ExprT, uint64_t kMaxValue, uint64_t kStep = 1, class FuncT, class... Args>
auto RolledSwitchReturnable(FuncT&& func, const uint64_t value, Args&&... args)
{
    if (value == kMaxValue) {
        return func.template operator()<static_cast<ExprT>(kMaxValue)>(std::forward<Args>(args)...);
    }

    if constexpr (kMaxValue >= kStep) {
        return RolledSwitchReturnable<ExprT, kMaxValue - kStep, kStep>(
            std::forward<FuncT>(func), value, std::forward<Args>(args)...
        );
    }

    std::cerr << "Error: RolledSwitch: value " << value << " is out of range" << std::endl;
    std::abort();
}

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_TEMPLATE_HELPERS_HPP_
