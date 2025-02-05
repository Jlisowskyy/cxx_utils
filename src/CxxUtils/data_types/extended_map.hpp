#ifndef CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
#define CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_

#include <CxxUtils/defines.hpp>

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <functional>
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
    using EventFuncType = std::function<void(const Key &)>;
    using std::unordered_map<Key, Value>::unordered_map;

    // ------------------------------
    // Class interactions
    // ------------------------------

    [[nodiscard("Identifier leak")]] uint64_t AddListener(EventFuncType &&listener)
    {
        const std::lock_guard lock(mutex_);
        const uint64_t id = listener_id_++;
        listeners_.emplace_back(id, std::move(listener));

        return id;
    }

    void RemoveListener(const uint64_t identifier)
    {
        const std::lock_guard lock(mutex_);

        const auto it = std::find_if(listeners_.begin(), listeners_.end(), [identifier](const auto &listener) {
            return std::get<0>(listener) == identifier;
        });

        assert(it != listeners_.end());
        listeners_.erase(it);
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

    uint64_t listener_id_ = 0;
    std::vector<std::tuple<int, EventFuncType>> listeners_;
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_EXTENDED_MAP_HPP_
