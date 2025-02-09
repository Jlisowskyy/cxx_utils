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
    template <class U, class Callable>
    FAST_CALL_ void Action(Callable &&func, U &&value)
    {
        using RealT = std::remove_cvref_t<U>;

        if constexpr (std::is_same_v<T, RealT>) {
            static_assert(HasType<RealT, T, Args...>(), "Type not found in MultiVector");
            func.template operator()<RealT>(data_, std::forward<U>(value));
            return;
        } else if constexpr (sizeof...(Args) > 0) {
            next_.Action(std::forward<Callable>(func), std::forward<U>(value));
            return;
        }

        std::abort();
    }

    template <class U, class Callable, class... ArgsT>
    FAST_CALL_ U &ReturnAction(Callable &&func, ArgsT &&...args)
    {
        using RealT = std::remove_cvref_t<U>;

        if constexpr (std::is_same_v<T, RealT>) {
            static_assert(HasType<RealT, T, Args...>(), "Type not found in MultiVector");
            return func.template operator()<RealT>(data_, std::forward<ArgsT>(args)...);
        } else if constexpr (sizeof...(Args) > 0) {
            return next_.template ReturnAction<U>(std::forward<Callable>(func), std::forward<ArgsT>(args)...);
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
        Action(
            []<class K>(std::vector<K> &data, const K &value) {
                data.push_back(value);
            },
            value
        );
    }

    template <class U>
    FAST_CALL_ U &front()
    {
        return ReturnAction<U>([]<class K>(std::vector<K> &data) -> K & {
            return data.front();
        });
    }

    template <class U>
    FAST_CALL_ const U &front() const
    {
        return ReturnAction<U>([]<class K>(const std::vector<K> &data) -> const K & {
            return data.front();
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
