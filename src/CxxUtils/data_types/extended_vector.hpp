#ifndef CXX_UTILS_DATA_TYPES_EXTENDED_VECTOR_HPP_
#define CXX_UTILS_DATA_TYPES_EXTENDED_VECTOR_HPP_

#include <CxxUtils/data_types/listeners.hpp>
#include <CxxUtils/defines.hpp>

#include <cassert>
#include <mutex>
#include <vector>

CXX_UTILS_DECL_START_

template <typename Value>
class ExtendedVector : public std::vector<Value>
{
    // ------------------------------
    // Internal types
    // ------------------------------

    static constexpr size_t kSizeGrowEstimate = 16;

    public:
    using std::vector<Value>::vector;

    // ------------------------------
    // Class interactions
    // ------------------------------

    NDSCRD_ std::vector<Value> GetValuesSafe()
    {
        const std::lock_guard lock(mutex_);
        return *this;
    }

    NDSCRD_ FAST_CALL_ Listeners<ContainerEvents, const Value *> &GetListeners() { return listeners_; }

    NDSCRD_ FAST_CALL_ std::mutex &GetMutex() { return mutex_; }
    FAST_CALL_ void Lock() { mutex_.lock(); }
    FAST_CALL_ void Unlock() { mutex_.unlock(); }

    FAST_CALL_ void Clear()
    {
        const std::lock_guard lock(mutex_);
        std::vector<Value>::clear();
        listeners_.template NotifyListeners<ContainerEvents::kClear>(nullptr);
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    protected:
    std::mutex mutex_{};
    Listeners<ContainerEvents, const Value *> listeners_{&mutex_};
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_EXTENDED_VECTOR_HPP_
