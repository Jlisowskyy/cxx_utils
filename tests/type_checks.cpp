#include <gtest/gtest.h>

#include <CxxUtils/type_checks.hpp>

using namespace CxxUtils;

TEST(TypeChecks, CountTypes)
{
    EXPECT_EQ(2, (CountType<int, int, int>()));
    EXPECT_EQ(1, (CountType<int, int, float>()));
}

TEST(TypeChecks, HasType)
{
    EXPECT_TRUE((HasType<int, int, int>()));
    EXPECT_TRUE((HasType<int, int, float>()));
    EXPECT_FALSE((HasType<int, float, float>()));
}

TEST(TypeChecks, HasTypeOnce)
{
    EXPECT_TRUE((HasTypeOnce<int, int, float>()));
    EXPECT_FALSE((HasTypeOnce<int, int, int>()));
    EXPECT_FALSE((HasTypeOnce<int, float, float>()));
}

TEST(TypeChecks, HasDuplicateType)
{
    EXPECT_TRUE((HasDuplicateType<int, int, int>()));
    EXPECT_FALSE((HasDuplicateType<int, int, float>()));
    EXPECT_FALSE((HasDuplicateType<int, float, float>()));
}

TEST(TypeChecks, HasDuplicateTypes)
{
    EXPECT_TRUE((HasDuplicateTypes<int, int, int>()));
    EXPECT_TRUE((HasDuplicateTypes<int, int, float>()));
    EXPECT_FALSE((HasDuplicateTypes<int, float, double>()));
}
