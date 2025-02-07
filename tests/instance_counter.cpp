#include <gtest/gtest.h>

#include <CxxUtils/instance_counter.hpp>

class TestObject : public CxxUtils::InstanceCounter<TestObject>
{
};

TEST(InstanceCounter, Basic)
{
    TestObject obj1;
    TestObject obj2;
    TestObject obj3;

    ASSERT_EQ(obj1.GetInstanceID(), 0);
    ASSERT_EQ(obj2.GetInstanceID(), 1);
    ASSERT_EQ(obj3.GetInstanceID(), 2);

    ASSERT_EQ(TestObject::GetInstanceCount(), 3);

    obj2 = obj1;

    ASSERT_EQ(0, obj2.GetInstanceID());
}
