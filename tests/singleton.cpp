#include <gtest/gtest.h>

#include <CxxUtils/singleton.hpp>
#include <CxxUtils/static_singleton.hpp>

using namespace CxxUtils;

class DSingleton final : public Singleton<DSingleton>
{
    public:
    DSingleton() { x = 3; }
    explicit DSingleton(const int y) { x = y; }

    int x;

    static DSingleton &InitInstance()
    {
        assert(!IsInited());
        Singleton::InitInstance(new DSingleton());
        return GetInstance();
    }
};

class SSingletonInstance final : public StaticSingletonHelper
{
    public:
    SSingletonInstance() { x = 3; }
    explicit SSingletonInstance(const int y) { x = y; }

    ~SSingletonInstance() override = default;

    int x;
};

using SSingleton = StaticSingleton<SSingletonInstance>;

TEST(Singletons, DynamicSingleton)
{
    DSingleton::InitInstance();
    ASSERT_TRUE(DSingleton::IsInited());

    ASSERT_EQ(3, DSingleton::GetInstance().x);

    DSingleton::DeleteInstance();
    ASSERT_FALSE(DSingleton::IsInited());
}

TEST(Singletons, StaticSingleton)
{
    SSingleton::InitInstance();
    ASSERT_TRUE(SSingleton::IsInited());

    ASSERT_EQ(3, SSingleton::GetInstance().x);
    SSingleton::DeleteInstance();
    ASSERT_FALSE(SSingleton::IsInited());

    SSingleton::InitInstance(5);
    ASSERT_TRUE(SSingleton::IsInited());
    ASSERT_EQ(5, SSingleton::GetInstance().x);
    SSingleton::DeleteInstance();
    ASSERT_FALSE(SSingleton::IsInited());
}
