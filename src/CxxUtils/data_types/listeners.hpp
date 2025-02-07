#ifndef CXX_UTILS_DATA_TYPES_LISTENERS_HPP_
#define CXX_UTILS_DATA_TYPES_LISTENERS_HPP_

#include <CxxUtils/defines.hpp>

#include <algorithm>
#include <array>
#include <cinttypes>
#include <functional>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <vector>

CXX_UTILS_DECL_START_

// ------------------------------
// Predefined enums
// ------------------------------

enum class ContainerEvents {
    kAdd,
    kRemove,
    kLast,
};

// ------------------------------
// Listeners class
// ------------------------------

template <class EnumT, class... Args>
    requires std::is_enum_v<EnumT>
class Listeners
{
    // ------------------------------
    // Class creation
    // ------------------------------

    public:
    Listeners() = delete;
    explicit Listeners(std::mutex *mutex) : mutex_(mutex) {}
    ~Listeners() = default;

    Listeners(const Listeners &)            = default;
    Listeners &operator=(const Listeners &) = default;

    Listeners(Listeners &&)            = default;
    Listeners &operator=(Listeners &&) = default;

    // ------------------------------
    // Class interaction
    // ------------------------------

    template <EnumT event>
    [[nodiscard("Identifier leak")]] uint64_t AddListener(std::function<void(Args...)> &&listener)
    {
        const std::lock_guard lock(*mutex_);
        const uint64_t id = listener_id_++;
        listeners_[static_cast<size_t>(event)].emplace_back(id, std::move(listener));

        return id;
    }

    template <EnumT event>
    void RemoveListener(const uint64_t identifier)
    {
        const std::lock_guard lock(*mutex_);

        const auto it = std::find_if(
            listeners_[static_cast<size_t>(event)].begin(), listeners_[static_cast<size_t>(event)].end(),
            [identifier](const auto &listener) {
                return std::get<0>(listener) == identifier;
            }
        );

        assert(it != listeners_[static_cast<size_t>(event)].end());
        listeners_[static_cast<size_t>(event)].erase(it);
    }

    template <EnumT event>
    void NotifyListeners(Args... args)
    {
        for (const auto &[_, listener] : listeners_[static_cast<size_t>(event)]) {
            listener(args...);
        }
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    protected:
    std::mutex *mutex_{};
    uint64_t listener_id_ = 0;
    std::array<std::vector<std::tuple<uint64_t, std::function<void(Args...)>>>, static_cast<size_t>(EnumT::kLast)>
        listeners_{};
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_LISTENERS_HPP_
