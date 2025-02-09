#include <gtest/gtest.h>

#include <CxxUtils/template_helpers.hpp>

using namespace CxxUtils;

TEST(TemplateHelpers, Switch)
{
    bool is_called  = false;
    const auto func = [&is_called]<size_t value>(const bool b) -> int {
        is_called = true;
        return 9;
    };

    is_called = false;
    RolledSwitch<size_t, 10, 1>(func, 3, true);
    ASSERT_TRUE(is_called);

    is_called = false;
    RolledSwitch<size_t, 10, 1>(func, 10, true);
    ASSERT_TRUE(is_called);

    is_called = false;
    RolledSwitch<size_t, 10, 2>(func, 4, true);
    ASSERT_TRUE(is_called);

    is_called        = false;
    const int result = RolledSwitchReturnable<size_t, 10, 1>(func, 0, true);
    ASSERT_EQ(9, result);
    ASSERT_TRUE(is_called);
}
