#include <gtest/gtest.h>
#include <ww_array.h>

using namespace wwstl;

class WwArrayTest : public ::testing::Test
{
public:
    array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
};

// 元素访问
TEST_F(WwArrayTest, ElementAccess)
{
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 10);
    EXPECT_EQ(arr.at(3), 4);
    EXPECT_EQ(arr[6], 7);
    EXPECT_EQ(arr.data()[0], 1);

    EXPECT_THROW(arr.at(10), std::out_of_range);
}

// 迭代器
TEST_F(WwArrayTest, Iterator)
{
    auto it = arr.begin();
    EXPECT_EQ(*(it + 4), 5);

    for (int i = 1; i <= 10; ++i) {
        EXPECT_EQ(*it, i);
        ++it;
    }

    it = arr.end();
    EXPECT_EQ(*(it - 4), 7);

    for (int i = 10; i >= 1; --i) {
        --it;
        EXPECT_EQ(*it, i);
    }

    auto rit = arr.rbegin();
    for (int i = 10; i >= 1; --i) {
        EXPECT_EQ(*rit, i);
        ++rit;
    }

    rit = arr.rend();
    for (int i = 1; i <= 10; ++i) {
        --rit;
        EXPECT_EQ(*rit, i);
    }
}

// 容量
TEST_F(WwArrayTest, Capacity)
{
    EXPECT_FALSE(arr.empty());
    EXPECT_EQ(arr.size(), 10);
    EXPECT_EQ(arr.max_size(), 10);
}

// 操作
TEST_F(WwArrayTest, Operation)
{
    auto it = arr.begin() + 2;
    EXPECT_EQ(*it, 3);

    arr.fill(0);
    for (int i = 1; i <= 10; ++i) {
        EXPECT_EQ(arr[i - 1], 0);
    }

    EXPECT_EQ(*it, 0);      // 迭代器不失效

    array<int, 10> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    swap(arr, arr2);

    for (int i = 1; i <= 10; ++i) {
        EXPECT_EQ(arr[i - 1], i);
        EXPECT_EQ(arr2[i - 1], 0);
    }
}