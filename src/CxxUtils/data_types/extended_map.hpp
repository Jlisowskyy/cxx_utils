#ifndef CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
#define CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_

#include <CxxUtils/defines.hpp>

#include <algorithm>
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
    using EventFuncType = void (*)(const Key &);

    using std::unordered_map<Key, Value>::unordered_map;

    // ------------------------------
    // Class interactions
    // ------------------------------

    void AddListener(EventFuncType listener) { listeners_.push_back(listener); }
    void RemoveListener(EventFuncType listener)
    {
        assert(std::find(listeners_.begin(), listeners_.end(), listener) != listeners_.end());
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
    }

    template <AccessType type = AccessType::kThreadSafe>
    [[nodiscard]] std::vector<Key> GetKeys()
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
    [[nodiscard]] std::vector<Value> GetValues()
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

    [[nodiscard]] std::mutex &GetMutex() { return mutex_; }
    void Lock() { mutex_.lock(); }
    void Unlock() { mutex_.unlock(); }

    // ------------------------------
    // Class fields
    // ------------------------------

    protected:
    std::mutex mutex_;
    std::vector<EventFuncType> listeners_;
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
