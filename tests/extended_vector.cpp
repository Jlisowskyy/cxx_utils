#include <gtest/gtest.h>

#include <CxxUtils/data_types/extended_vector.hpp>
using namespace CxxUtils;

class ExtendedVectorTest : public ::testing::Test
{
    protected:
    ExtendedVector<int> vec_{1, 2, 3};
};

TEST_F(ExtendedVectorTest, InitializationTest)
{
    EXPECT_EQ(vec_.size(), 3);
    EXPECT_EQ(vec_[0], 1);
    EXPECT_EQ(vec_[1], 2);
    EXPECT_EQ(vec_[2], 3);
}

TEST_F(ExtendedVectorTest, GetValuesSafeTest)
{
    auto values = vec_.GetValuesSafe();
    std::vector<int> expected_values{1, 2, 3};
    EXPECT_EQ(values.size(), expected_values.size());
    EXPECT_EQ(values, expected_values);
}

TEST_F(ExtendedVectorTest, ListenerManagementTest)
{
    auto &listeners = vec_.GetListeners();
    const int a     = 1;

    EXPECT_NO_THROW(listeners.NotifyListeners<ContainerEvents::kAdd>(&a));
}

TEST_F(ExtendedVectorTest, MutexLockUnlockTest)
{
    vec_.Lock();
    vec_.Unlock();
    EXPECT_TRUE(true);
}

TEST_F(ExtendedVectorTest, ClearEvent)
{
    bool was_invoked = false;
    auto test_func   = [&](const int *key) {
        was_invoked = true;
    };
    const auto ident = vec_.GetListeners().AddListener<ContainerEvents::kClear>(test_func);

    vec_.Clear();

    EXPECT_TRUE(was_invoked);
}
