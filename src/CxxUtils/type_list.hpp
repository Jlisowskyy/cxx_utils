#ifndef TYPE_LIST_HPP_
#define TYPE_LIST_HPP_

#include <CxxUtils/defines.hpp>

CXX_UTILS_DECL_START_

template <size_t N, class T, class... Ts>
struct TypeList {
    static constexpr size_t size = sizeof...(Ts) + 1;

    static_assert(N < size, "Index out of range");
    using type = typename TypeList<N - 1, Ts...>::type;
};

template <class T, class... Ts>
struct TypeList<0, T, Ts...> {
    static constexpr size_t size = sizeof...(Ts) + 1;

    using type = T;
};

template <size_t N, template <size_t> class TypeIter>
struct IterateTypeList {
    template <class Callable>
    static void Apply(Callable&& func)
    {
        func.template operator()<N, typename TypeIter<N>::type>();
        IterateTypeList<N - 1, TypeIter>::Apply(std::forward<Callable>(func));
    }
};

template <template <size_t> class TypeIter>
struct IterateTypeList<0, TypeIter> {
    template <class Callable>
    static void Apply(Callable&& func)
    {
        func.template operator()<0, typename TypeIter<0>::type>();
    }
};

CXX_UTILS_DECL_END_

#endif  // TYPE_LIST_HPP_
