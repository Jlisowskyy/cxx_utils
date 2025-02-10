#include <gtest/gtest.h>

#include <CxxUtils/data_types/multi_vector.hpp>

using namespace CxxUtils;

class MultiVectorTest : public ::testing::Test
{
    protected:
    MultiVector<int, float, double> vec_{};

    void SetUp() override
    {
        vec_.push_back(1);
        vec_.push_back(2.0f);
        vec_.push_back(3.0);
    }
};

TEST_F(MultiVectorTest, SizeTest)
{
    EXPECT_EQ(
        sizeof(vec_),
        (sizeof(std::vector<int>) + sizeof(std::vector<float>) + sizeof(std::vector<double>)) + sizeof(void *)
    );
}

TEST_F(MultiVectorTest, FrontBackTest)
{
    EXPECT_EQ(1, vec_.front<int>());
    EXPECT_EQ(2.0f, vec_.front<float>());
    EXPECT_EQ(3.0, vec_.front<double>());

    EXPECT_EQ(1, vec_.back<int>());
    EXPECT_EQ(2.0f, vec_.back<float>());
    EXPECT_EQ(3.0, vec_.back<double>());

    const auto &const_vec = vec_;

    EXPECT_EQ(1, const_vec.front<int>());
    EXPECT_EQ(2.0f, const_vec.front<float>());
    EXPECT_EQ(3.0, const_vec.front<double>());

    EXPECT_EQ(1, const_vec.back<int>());
    EXPECT_EQ(2.0f, const_vec.back<float>());
    EXPECT_EQ(3.0, const_vec.back<double>());
}

TEST_F(MultiVectorTest, GetSizeTest)
{
    EXPECT_EQ(1, vec_.size<int>());
    EXPECT_EQ(1, vec_.size<float>());
    EXPECT_EQ(1, vec_.size<double>());

    vec_.push_back(4);
    vec_.push_back(5);

    EXPECT_EQ(3, vec_.size<int>());
}

TEST_F(MultiVectorTest, ClearTest)
{
    vec_.clear<int>();
    vec_.clear<float>();
    vec_.clear<double>();

    EXPECT_EQ(0, vec_.size<int>());
    EXPECT_EQ(0, vec_.size<float>());
    EXPECT_EQ(0, vec_.size<double>());

    SetUp();
    vec_.FullClear();

    EXPECT_TRUE(vec_.IsEmpty());
    EXPECT_EQ(0, vec_.size<int>());
    EXPECT_EQ(0, vec_.size<float>());
    EXPECT_EQ(0, vec_.size<double>());

    EXPECT_TRUE(vec_.empty<int>());
    EXPECT_TRUE(vec_.empty<float>());
    EXPECT_TRUE(vec_.empty<double>());
}

TEST_F(MultiVectorTest, SingleTForeach)
{
    static constexpr int kExpectedSum = 1 + 4 + 5 + 3 + 4;

    vec_.push_back(3);
    vec_.push_back(4);
    vec_.push_back(5);

    int sum{};

    vec_.Foreach<int>([&sum](int &value) {
        ++value;
    });
    vec_.Foreach<int>([&sum](const int &value) {
        sum += value;
    });

    EXPECT_EQ(kExpectedSum, sum);

    const auto &const_vec = vec_;

    sum = 0;
    vec_.Foreach<int>([&sum](const int &value) {
        sum += value;
    });

    EXPECT_EQ(kExpectedSum, sum);
}

TEST_F(MultiVectorTest, FullForeach)
{
    double sum{};

    vec_.Foreach([&sum]<class K>(const K &value) {
        sum += static_cast<double>(value);
    });

    EXPECT_EQ(1.0 + 2.0 + 3.0, sum);
}

TEST_F(MultiVectorTest, Copying)
{
    auto vec = vec_;

    EXPECT_EQ(1, vec_.front<int>());
    EXPECT_EQ(2.0f, vec_.front<float>());
    EXPECT_EQ(3.0, vec_.front<double>());
}

TEST_F(MultiVectorTest, Moving)
{
    auto vec = std::move(vec_);

    EXPECT_EQ(1, vec.front<int>());
    EXPECT_EQ(2.0f, vec.front<float>());
    EXPECT_EQ(3.0, vec.front<double>());

    EXPECT_TRUE(vec_.IsEmpty());
}
