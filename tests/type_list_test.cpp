#include <gtest/gtest.h>

#include <CxxUtils/type_list.hpp>

template <size_t N>
using test_t = CxxUtils::TypeList<N, int, float, double>;
TEST(TypeList, TypeListInit)
{
    ASSERT_TRUE((std::is_same_v<test_t<0>::type, int>));
    ASSERT_TRUE((std::is_same_v<test_t<1>::type, float>));
    ASSERT_TRUE((std::is_same_v<test_t<2>::type, double>));
}

TEST(TypeList, Iterate)
{
    CxxUtils::IterateTypeList<2, test_t>::Apply([]<size_t N, class T> {
        std::cout << N << " " << static_cast<T>(1) / static_cast<T>(2) << std::endl;
    });
}
