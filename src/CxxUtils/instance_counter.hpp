#ifndef CXX_UTILS_INSTANCE_COUNTER_HPP_
#define CXX_UTILS_INSTANCE_COUNTER_HPP_

#include <CxxUtils/defines.hpp>

#include <atomic>
#include <cstdlib>

CXX_UTILS_DECL_START_

template <typename>
class InstanceCounter
{
    static std::atomic<size_t> instance_count_;
    size_t instance_id_;

    public:
    InstanceCounter() : instance_id_(instance_count_++) {}

    [[nodiscard]] static size_t GetInstanceCount() noexcept { return instance_count_; }
    [[nodiscard]] size_t GetInstanceID() const noexcept { return instance_id_; }
};

template <typename T>
std::atomic<size_t> InstanceCounter<T>::instance_count_{0};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_INSTANCE_COUNTER_HPP_
