#ifndef CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
#define CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_

#include <CxxUtils/defines.hpp>

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <mutex>
#include <unordered_map>
#include <vector>

CXX_UTILS_DECL_START_

template <typename Key, typename Value>
class ExtendedMap
{
    // ------------------------------
    // Internal types
    // ------------------------------

    static constexpr size_t kSizeGrowEstimate = 16;

    public:
    using EventFuncType = void (*)(const Key &);

    // ------------------------------
    // Class creation
    // ------------------------------

    ExtendedMap(std::initializer_list<std::pair<Key, Value>> list)
    {
        for (const auto &[key, value] : list) {
            map_[key] = value;
        }
    }

    // ------------------------------
    // Class interactions
    // ------------------------------

    void AddListener(EventFuncType listener) { listeners_.push_back(listener); }
    void RemoveListener(EventFuncType listener)
    {
        assert(std::find(listeners_.begin(), listeners_.end(), listener) != listeners_.end());
        listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
    }

    [[nodiscard]] const std::unordered_map<Key, Value> &GetMap() const { return map_; }
    [[nodiscard]] std::unordered_map<Key, Value> &GetMap() { return map_; }

    template <AccessType type>
    [[nodiscard]] std::vector<Key> GetKeysThread() const
    {
        std::vector<Key> keys;
        keys.reserve(map_.size() + kSizeGrowEstimate);

        if constexpr (type == AccessType::kThreadSafe) {
            std::lock_guard lock(mutex_);
            for (const auto &[key, _] : map_) {
                keys.push_back(key);
            }
        } else {
            for (const auto &[key, _] : map_) {
                keys.push_back(key);
            }
        }

        return keys;
    }

    template <AccessType type>
    [[nodiscard]] std::vector<Value> GetValuesThreadSafe() const
    {
        std::vector<Value> values;
        values.reserve(map_.size() + kSizeGrowEstimate);

        if constexpr (type == AccessType::kThreadSafe) {
            std::lock_guard lock(mutex_);
            for (const auto &[_, value] : map_) {
                values.push_back(value);
            }
        } else {
            for (const auto &[_, value] : map_) {
                values.push_back(value);
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
    std::unordered_map<Key, Value> map_;
    std::mutex mutex_;
    std::vector<EventFuncType> listeners_;
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
