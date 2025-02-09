#ifndef CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
#define CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_

#include <CxxUtils/data_types/listeners.hpp>
#include <CxxUtils/defines.hpp>

#include <cassert>
#include <mutex>
#include <unordered_map>
#include <vector>

CXX_UTILS_DECL_START_

template <typename Key, typename Value>
class ExtendedMap : public std::unordered_map<Key, Value>
{
    // ------------------------------
    // Internal types
    // ------------------------------

    static constexpr size_t kSizeGrowEstimate = 16;

    public:
    using std::unordered_map<Key, Value>::unordered_map;

    // ------------------------------
    // Class interactions
    // ------------------------------

    template <AccessType type = AccessType::kThreadSafe>
    NDSCRD_ std::vector<Key> GetKeys()
    {
        std::vector<Key> keys;
        keys.reserve(this->size() + kSizeGrowEstimate);

        if constexpr (type == AccessType::kThreadSafe) {
            const std::lock_guard lock(mutex_);
            for (const auto &[key, _] : *this) {
                keys.emplace_back(key);
            }
        } else {
            for (const auto &[key, _] : *this) {
                keys.emplace_back(key);
            }
        }

        return keys;
    }

    template <AccessType type = AccessType::kThreadSafe>
    NDSCRD_ std::vector<Value> GetValues()
    {
        std::vector<Value> values;
        values.reserve(this->size() + kSizeGrowEstimate);

        if constexpr (type == AccessType::kThreadSafe) {
            const std::lock_guard lock(mutex_);
            for (const auto &[_, value] : *this) {
                values.emplace_back(value);
            }
        } else {
            for (const auto &[_, value] : *this) {
                values.emplace_back(value);
            }
        }

        return values;
    }

    NDSCRD_ FAST_CALL_ Listeners<ContainerEvents, const Key *> &GetListeners() { return listeners_; }

    NDSCRD_ FAST_CALL_ std::mutex &GetMutex() { return mutex_; }
    WRAP_CALL_ void Lock() { mutex_.lock(); }
    WRAP_CALL_ void Unlock() { mutex_.unlock(); }

    FAST_CALL_ void Clear()
    {
        const std::lock_guard lock(mutex_);
        std::unordered_map<Key, Value>::clear();
        listeners_.template NotifyListeners<ContainerEvents::kClear>(nullptr);
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    protected:
    std::mutex mutex_{};
    Listeners<ContainerEvents, const Key *> listeners_{&mutex_};
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
