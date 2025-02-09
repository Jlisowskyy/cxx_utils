#ifndef CXX_UTILS_DATA_TYPES_MULTI_VECTOR_HPP_
#define CXX_UTILS_DATA_TYPES_MULTI_VECTOR_HPP_

#include <CxxUtils/defines.hpp>
#include <CxxUtils/type_checks.hpp>

#include <type_traits>
#include <vector>

CXX_UTILS_DECL_START_

template <class... Args>
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
    FAST_CALL_ void Action(Callable &&func, ArgsT &&...args)
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
    FAST_CALL_ void Action(Callable &&func, ArgsT &&...args) const
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

    template <class U, class Callable, class... ArgsT>
    FAST_CALL_ U &ReturnAction(Callable &&func, ArgsT &&...args)
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");
            return func.template operator()<U>(data_, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnAction<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    template <class U, class Callable, class... ArgsT>
    FAST_CALL_ const U &ReturnAction(Callable &&func, ArgsT &&...args) const
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");

            const auto &data = data_;
            return func.template operator()<U>(data, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnAction<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    template <class U, class Callable, class... ArgsT>
    FAST_CALL_ auto ReturnActionValue(Callable &&func, ArgsT &&...args)
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");
            return func.template operator()<U>(data_, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnActionValue<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    template <class U, class Callable, class... ArgsT>
    FAST_CALL_ auto ReturnActionValue(Callable &&func, ArgsT &&...args) const
    {
        if constexpr (std::is_same_v<T, U>) {
            static_assert(HasType<U, T, Args...>(), "Type not found in MultiVector");

            const auto &data = data_;
            return func.template operator()<U>(data, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnActionValue<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
        }

        std::abort();
    }

    public:
    // ------------------------------
    // Class interactions
    // ------------------------------

    template <class U>
    FAST_CALL_ void push_back(const U &value)
    {
        Action<U>(
            []<class K>(std::vector<K> &data, const K &value) FORCE_INLINE__ {
                data.push_back(value);
            },
            value
        );
    }

    template <class U>
    FAST_CALL_ U &front()
    {
        return ReturnAction<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ -> K & {
            return data.front();
        });
    }

    template <class U>
    FAST_CALL_ const U &front() const
    {
        return ReturnAction<U>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const K & {
            return data.front();
        });
    }

    template <class U>
    FAST_CALL_ U &back()
    {
        return ReturnAction<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ -> K & {
            return data.back();
        });
    }

    template <class U>
    FAST_CALL_ const U &back() const
    {
        return ReturnAction<U>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> const K & {
            return data.back();
        });
    }

    template <class U>
    NDSCRD_ FAST_CALL_ size_t size() const
    {
        return ReturnActionValue<U>([]<class K>(const std::vector<K> &data) FORCE_INLINE__ -> size_t {
            return data.size();
        });
    }

    template <class U>
    FAST_CALL_ void clear() noexcept
    {
        Action<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.clear();
        });
    }

    template <class U>
    FAST_CALL_ void pop_back()
    {
        Action<U>([]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            data.pop_back();
        });
    }

    template <class U, class Func>
    FAST_CALL_ void Foreach(Func &&func)
    {
        Action<U>([&func]<class K>(std::vector<K> &data) FORCE_INLINE__ {
            for (auto &elem : data) {
                func(elem);
            }
        });
    }

    template <class U, class Func>
    FAST_CALL_ void Foreach(Func &&func) const
    {
        Action<U>([&func]<class K>(const std::vector<K> &data) FORCE_INLINE__ {
            for (const auto &elem : data) {
                func(elem);
            }
        });
    }

    template <class Func>
    FAST_CALL_ void Foreach(Func &&func)
    {
        Foreach<T>([&func](T &elem) FORCE_INLINE__ {
            func.template operator()<T>(elem);
        });

        if constexpr (sizeof...(Args) > 0) {
            next_.Foreach(std::forward<Func>(func));
        }
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
