#include <gtest/gtest.h>
#include <ww_set.h>

using namespace wwstl;

class WWSetTest : public testing::Test
{
public:
    set<int> set;

protected:
    void SetUp() override
    {
        set.insert(1);
        set.insert(2);
        set.insert(3);
        set.insert(4);
    }
};

// 迭代器

TEST_F(WWSetTest, begin)
{
    EXPECT_EQ(*set.begin(), 1);
}

TEST_F(WWSetTest, end)
{
    EXPECT_EQ(*--set.end(), 4);
}

TEST_F(WWSetTest, rbegin)
{
    EXPECT_EQ(*set.rbegin(), 4);
}

TEST_F(WWSetTest, rend)
{
    EXPECT_EQ(*--set.rend(), 1);
}

// 容量

TEST_F(WWSetTest, empty)
{
    EXPECT_EQ(set.empty(), false);
}

TEST_F(WWSetTest, size)
{
    EXPECT_EQ(set.size(), 4);
}

// 修改器

TEST_F(WWSetTest, clear)
{
    set.clear();
    EXPECT_EQ(set.empty(), true);
}

TEST_F(WWSetTest, insert)
{
    auto it = set.insert(1);
    EXPECT_FALSE(it.second);

    set.insert({2, 3, 4, 5, 6, 7, 8});
    EXPECT_EQ(set.size(), 8);
}

TEST_F(WWSetTest, erase)
{
    set.erase(1);
    EXPECT_EQ(set.size(), 3);
    EXPECT_EQ(*set.begin(), 2);

    set.erase(4);
    EXPECT_EQ(set.size(), 2);
    EXPECT_EQ(*--set.end(), 3);

    set.erase(set.begin(), set.end());
    EXPECT_TRUE(set.empty());
}

// 查找

TEST_F(WWSetTest, count)
{
    EXPECT_EQ(set.count(1), 1);
    EXPECT_EQ(set.count(2), 1);
    EXPECT_EQ(set.count(3), 1);
    EXPECT_EQ(set.count(4), 1);
    EXPECT_EQ(set.count(5), 0);
}

TEST_F(WWSetTest, find)
{
    EXPECT_EQ(*set.find(1), 1);
    EXPECT_EQ(*set.find(2), 2);
    EXPECT_EQ(*set.find(3), 3);
    EXPECT_EQ(*set.find(4), 4);
    EXPECT_EQ(set.find(5), set.end());
}

TEST_F(WWSetTest, equal_range)
{
    auto range = set.equal_range(1);
    EXPECT_EQ(*range.first, 1);
    EXPECT_EQ(*range.second, 2);
}

TEST_F(WWSetTest, lower_bound)
{
    EXPECT_EQ(*set.lower_bound(1), 1);
    EXPECT_EQ(*set.lower_bound(2), 2);
    EXPECT_EQ(*set.lower_bound(3), 3);
    EXPECT_EQ(*set.lower_bound(4), 4);
    EXPECT_EQ(set.lower_bound(5), set.end());
}

TEST_F(WWSetTest, upper_bound)
{
    EXPECT_EQ(*set.upper_bound(1), 2);
    EXPECT_EQ(*set.upper_bound(2), 3);
    EXPECT_EQ(*set.upper_bound(3), 4);
    EXPECT_EQ(set.upper_bound(4), set.end());
    EXPECT_EQ(set.upper_bound(5), set.end());
}