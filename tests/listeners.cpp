#include <gtest/gtest.h>
#include <CxxUtils/data_types/listeners.hpp>

using namespace CxxUtils;

#include <mutex>

class ListenersTest : public ::testing::Test
{
    protected:
    std::mutex mutex_;
    Listeners<ContainerEvents, int> listeners_{&mutex_};
};

TEST_F(ListenersTest, AddRemoveListenerTest)
{
    auto callback = [](int value) {
    };
    uint64_t id = listeners_.AddListener<ContainerEvents::kAdd>(callback);
    EXPECT_NO_THROW(listeners_.RemoveListener<ContainerEvents::kAdd>(id));
}

TEST_F(ListenersTest, NotifyListenersTest)
{
    bool called   = false;
    auto callback = [&called](int value) {
        called = true;
    };
    listeners_.AddListener<ContainerEvents::kAdd>(callback);
    listeners_.NotifyListeners<ContainerEvents::kAdd>(42);
    EXPECT_TRUE(called);
}
