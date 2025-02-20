#include <gtest/gtest.h>
#include <ww_set.h>

using namespace wwstl;

class WWSetTest : public testing::Test
{
public:
    set<int> s;

protected:
    void SetUp() override
    {
        s.insert(1);
        s.insert(2);
        s.insert(3);
        s.insert(4);
    }
};

// 迭代器

TEST_F(WWSetTest, begin)
{
    EXPECT_EQ(*s.begin(), 1);
}

TEST_F(WWSetTest, end)
{
    EXPECT_EQ(*--s.end(), 4);
}

TEST_F(WWSetTest, rbegin)
{
    EXPECT_EQ(*s.rbegin(), 4);
}

TEST_F(WWSetTest, rend)
{
    EXPECT_EQ(*--s.rend(), 1);
}

// 容量

TEST_F(WWSetTest, empty)
{
    EXPECT_EQ(s.empty(), false);
}

TEST_F(WWSetTest, size)
{
    EXPECT_EQ(s.size(), 4);
}

// 修改器

TEST_F(WWSetTest, clear)
{
    s.clear();
    EXPECT_EQ(s.empty(), true);
}

TEST_F(WWSetTest, insert)
{
    auto it = s.insert(1);
    EXPECT_FALSE(it.second);

    s.insert({2, 3, 4, 5, 6, 7, 8});
    EXPECT_EQ(s.size(), 8);
}

TEST_F(WWSetTest, erase)
{
    s.erase(1);
    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(*s.begin(), 2);

    s.erase(4);
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(*--s.end(), 3);

    s.erase(s.begin(), s.end());
    EXPECT_TRUE(s.empty());
}

// 查找

TEST_F(WWSetTest, count)
{
    EXPECT_EQ(s.count(1), 1);
    EXPECT_EQ(s.count(2), 1);
    EXPECT_EQ(s.count(3), 1);
    EXPECT_EQ(s.count(4), 1);
    EXPECT_EQ(s.count(5), 0);
}

TEST_F(WWSetTest, find)
{
    EXPECT_EQ(*s.find(1), 1);
    EXPECT_EQ(*s.find(2), 2);
    EXPECT_EQ(*s.find(3), 3);
    EXPECT_EQ(*s.find(4), 4);
    EXPECT_EQ(s.find(5), s.end());
}

TEST_F(WWSetTest, equal_range)
{
    auto range = s.equal_range(1);
    EXPECT_EQ(*range.first, 1);
    EXPECT_EQ(*range.second, 2);
}

TEST_F(WWSetTest, lower_bound)
{
    EXPECT_EQ(*s.lower_bound(1), 1);
    EXPECT_EQ(*s.lower_bound(2), 2);
    EXPECT_EQ(*s.lower_bound(3), 3);
    EXPECT_EQ(*s.lower_bound(4), 4);
    EXPECT_EQ(s.lower_bound(5), s.end());
}

TEST_F(WWSetTest, upper_bound)
{
    EXPECT_EQ(*s.upper_bound(1), 2);
    EXPECT_EQ(*s.upper_bound(2), 3);
    EXPECT_EQ(*s.upper_bound(3), 4);
    EXPECT_EQ(s.upper_bound(4), s.end());
    EXPECT_EQ(s.upper_bound(5), s.end());
}

// 比较

TEST_F(WWSetTest, compare)
{
    set<int> s2 = {1, 2, 3, 4};
    EXPECT_TRUE(s == s2);

    set<int> s3 = {1, 2, 3};
    EXPECT_FALSE(s == s3);
    EXPECT_TRUE(s > s3);

    set<int> s4 = {1, 2, 4, 7};
    EXPECT_FALSE(s == s4);
    EXPECT_TRUE(s < s4);
}