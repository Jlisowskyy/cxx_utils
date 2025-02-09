#ifndef CXX_UTILS_DATA_TYPES_MULTI_VECTOR_HPP_
#define CXX_UTILS_DATA_TYPES_MULTI_VECTOR_HPP_

#include <CxxUtils/defines.hpp>
#include <CxxUtils/type_checks.hpp>

#include <type_traits>
#include <vector>

CXX_UTILS_DECL_START_

template <class...>
class MultiVector
{
};

template <class T, class... Args>
class MultiVector<T, Args...>
{
    static_assert(!HasDuplicateTypes<T, Args...>(), "Duplicate type in MultiVector");

    // ------------------------------
    // Class creation
    // ------------------------------

    public:
    template <class U, class Callable, class... ArgsT>
    constexpr FAST_CALL_ void Action(Callable &&func, ArgsT &&...args)
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");
            func.template operator()<U>(data_, std::forward<ArgsT>(args)...);
            return;
        } else if constexpr (sizeof...(Args) > 0) {
            next_.template Action<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
            return;
        }

        std::abort();
    }

    template <class U, class Callable, class... ArgsT>
    constexpr FAST_CALL_ void Action(Callable &&func, ArgsT &&...args) const
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");

            const auto &data = data_;
            func.template operator()<U>(data, std::forward<ArgsT>(args)...);
            return;
        } else if constexpr (sizeof...(Args) > 0) {
            next_.template Action<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
            return;
        }

        std::abort();
    }

    template <class U, class ReturnT, class Callable, class... ArgsT>
    constexpr FAST_CALL_ ReturnT ReturnAction(Callable &&func, ArgsT &&...args)
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");
            return func.template operator()<U>(data_, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnAction<U, ReturnT>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    template <class U, class ReturnT, class Callable, class... ArgsT>
    constexpr FAST_CALL_ ReturnT ReturnAction(Callable &&func, ArgsT &&...args) const
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");

            const auto &data = data_;
            return func.template operator()<U>(data, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnAction<U, ReturnT>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    // ------------------------------
    // Class interactions
    // ------------------------------

    template <class U>
    constexpr FAST_CALL_ void push_back(const U &value)
    {
        Action<U>(
            []<class K>(std::vector<K> &data, const K &value) FORCE_INLINE__ {
                data.push_back(value);
            },
            value
        );
    }

    template <class U>
    constexpr FAST_CALL_ U &front()
    {
        return ReturnAction<U, U &>([]<class K>(std::vector<K> &data) FORCE_INLINE__ -> K & {
            return data.front();
        });
    }

    template <class U>
    constexpr FAST_CALL_ const U &front() const
    {
        return ReturnAction<U, const U &>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const K & {
            return data.front();
        });
    }

    template <class U>
    constexpr FAST_CALL_ U &back()
    {
        return ReturnAction<U, U &>([]<class K>(std::vector<K> &data) FORCE_INLINE__ -> K & {
            return data.back();
        });
    }

    template <class U>
    constexpr FAST_CALL_ const U &back() const
    {
        return ReturnAction<U, const U &>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const K & {
            return data.back();
        });
    }

    template <class U>
    NDSCRD_ constexpr FAST_CALL_ size_t size() const
    {
        return ReturnAction<U, size_t>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> size_t {
            return data.size();
        });
    }

    template <class U>
    constexpr FAST_CALL_ void clear() noexcept
    {
        Action<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.clear();
        });
    }

    template <class U>
    constexpr FAST_CALL_ void pop_back()
    {
        Action<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.pop_back();
        });
    }

    template <class U, class Func>
    constexpr FAST_CALL_ void Foreach(Func &&func)
    {
        Action<U>([&func]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            for (auto &elem : data) {
                func(elem);
            }
        });
    }

    template <class U, class Func>
    constexpr FAST_CALL_ void Foreach(Func &&func) const
    {
        Action<U>([&func]<class K>(const std::vector<K> &data) FORCE_INLINE__ {
            for (const auto &elem : data) {
                func(elem);
            }
        });
    }

    template <class Func>
    constexpr FAST_CALL_ void Foreach(Func &&func)
    {
        Foreach<T>([&func](T &elem) FORCE_INLINE__ {
            func.template operator()<T>(elem);
        });

        if constexpr (sizeof...(Args) > 0) {
            next_.Foreach(std::forward<Func>(func));
        }
    }

    template <class U>
    constexpr FAST_CALL_ std::vector<U> &GetUnderlyingData()
    {
        return ReturnAction<U, std::vector<U> &>([]<class K>(std::vector<K> &data) FORCE_INLINE__ -> std::vector<K> & {
            return data;
        });
    }

    template <class U>
    constexpr FAST_CALL_ const std::vector<U> &GetUnderlyingData() const
    {
        return ReturnAction<U, const std::vector<U> &>(
            []<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const std::vector<K> & {
                return data;
            }
        );
    }

    template <class U>
    constexpr FAST_CALL_ void reserve(const size_t size)
    {
        Action<U>([size]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.reserve(size);
        });
    }

    template <class U>
    constexpr FAST_CALL_ bool empty() const
    {
        return ReturnAction<U, bool>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> bool {
            return data.empty();
        });
    }

    NDSCRD_ constexpr FAST_CALL_ bool IsEmpty() const
    {
        bool result = data_.empty();

        if constexpr (sizeof...(Args) > 0) {
            result &= next_.IsEmpty();
        }

        return result;
    }

    constexpr FAST_CALL_ void FullClear() noexcept
    {
        data_.clear();
        if constexpr (sizeof...(Args) > 0) {
            next_.FullClear();
        }
    }

    template <class U>
    constexpr FAST_CALL_ void resize(const size_t size)
    {
        Action<U>([size]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.resize(size);
        });
    }

    template <class U, class... ArgsT>
    constexpr FAST_CALL_ U &emplace_back(ArgsT &&...args)
    {
        return ReturnAction<U, U &>([&args...]<class K>(std::vector<K> &data) FORCE_INLINE__ -> K & {
            return data.emplace_back(std::forward<ArgsT>(args)...);
        });
    }

    template <class U, class... ArgsT>
    constexpr FAST_CALL_ const U &emplace_back(ArgsT &&...args) const
    {
        return ReturnAction<U, U &>([&args...]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const K & {
            return data.emplace_back(std::forward<ArgsT>(args)...);
        });
    }

    // ------------------------------
    // Class fields
    // ------------------------------

    protected:
    std::vector<T> data_{};
    MultiVector<Args...> next_{};
};

CXX_UTILS_DECL_END_

#endif  // CXX_UTILS_DATA_TYPES_MULTI_VECTOR_HPP_
