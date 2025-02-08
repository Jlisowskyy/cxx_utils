#include <gtest/gtest.h>

#include <CxxUtils/data_types/extended_map.hpp>

using namespace CxxUtils;

class ExtendedMapTest : public ::testing::Test
{
    protected:
    ExtendedMap<int, std::string> map_{
        {1,   "one"},
        {2,   "two"},
        {3, "three"}
    };
};

TEST_F(ExtendedMapTest, InitializationTest)
{
    EXPECT_EQ(map_.size(), 3);
    EXPECT_EQ(map_[1], "one");
    EXPECT_EQ(map_[2], "two");
    EXPECT_EQ(map_[3], "three");
}

TEST_F(ExtendedMapTest, GetKeysThreadSafeTest)
{
    auto keys = map_.GetKeys<AccessType::kThreadSafe>();
    std::vector expected_keys{1, 2, 3};
    EXPECT_EQ(keys.size(), expected_keys.size());
    for (int key : expected_keys) {
        EXPECT_NE(std::find(keys.begin(), keys.end(), key), keys.end());
    }
}

TEST_F(ExtendedMapTest, GetValuesThreadSafeTest)
{
    auto values = map_.GetValues<AccessType::kThreadSafe>();
    std::vector<std::string> expected_values{"one", "two", "three"};
    EXPECT_EQ(values.size(), expected_values.size());
    for (const auto &value : expected_values) {
        EXPECT_NE(std::find(values.begin(), values.end(), value), values.end());
    }
}

void TestListenerFunction(const int *key) {}

TEST_F(ExtendedMapTest, ListenerManagementTest)
{
    const auto ident = map_.GetListeners().AddListener<ContainerEvents::kAdd>(TestListenerFunction);
    EXPECT_NO_THROW(map_.GetListeners().RemoveListener<ContainerEvents::kAdd>(ident));
}

TEST_F(ExtendedMapTest, MutexLockUnlockTest)
{
    map_.Lock();
    map_.Unlock();
    EXPECT_TRUE(true);
}

TEST_F(ExtendedMapTest, ClearEvent)
{
    bool was_invoked = false;
    auto test_func   = [&](const int *key) {
        was_invoked = true;
    };
    const auto ident = map_.GetListeners().AddListener<ContainerEvents::kClear>(test_func);

    map_.Clear();

    EXPECT_TRUE(was_invoked);
}
