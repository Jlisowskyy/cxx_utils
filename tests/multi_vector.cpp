#include <gtest/gtest.h>

#include <CxxUtils/data_types/multi_vector.hpp>

using namespace CxxUtils;

class MultiVectorTest : public ::testing::Test
{
    protected:
    MultiVector<int, float, double> vec_{};
};

TEST_F(MultiVectorTest, SizeTest)
{
    EXPECT_EQ(
        sizeof(vec_),
        (sizeof(std::vector<int>) + sizeof(std::vector<float>) + sizeof(std::vector<double>)) + sizeof(void *)
    );
}

TEST_F(MultiVectorTest, PushTest)
{
    vec_.push_back(1);
    vec_.push_back(2.0f);
    vec_.push_back(3.0);

    EXPECT_EQ(vec_.front<int>(), 1);
    EXPECT_EQ(vec_.front<float>(), 2.0f);
    EXPECT_EQ(vec_.front<double>(), 3.0);

    const auto &const_vec = vec_;
    //
    // EXPECT_EQ(const_vec.front<int>(), 1);
    // EXPECT_EQ(const_vec.front<float>(), 2.0f);
    // EXPECT_EQ(const_vec.front<double>(), 3.0);
}
