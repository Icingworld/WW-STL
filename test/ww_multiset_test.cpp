#include <gtest/gtest.h>
#include <ww_set.h>

using namespace wwstl;

void printSet(multiset<int> &mset)
{
    for (auto it = mset.begin(); it != mset.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

class WWMultiSetTest : public testing::Test
{
public:
    multiset<int> mset;

protected:
    void SetUp() override
    {
        mset.insert(1);
        mset.insert(2);
        mset.insert(3);
        mset.insert(4);
        mset.insert(1);
        mset.insert(2);
        mset.insert(2);
    }
};

// 迭代器

TEST_F(WWMultiSetTest, begin)
{
    EXPECT_EQ(*mset.begin(), 1);
}

TEST_F(WWMultiSetTest, end)
{
    EXPECT_EQ(*--mset.end(), 4);
}

TEST_F(WWMultiSetTest, rbegin)
{
    EXPECT_EQ(*mset.rbegin(), 4);
}

TEST_F(WWMultiSetTest, rend)
{
    EXPECT_EQ(*--mset.rend(), 1);
}

// 容量

TEST_F(WWMultiSetTest, empty)
{
    EXPECT_EQ(mset.empty(), false);
}

TEST_F(WWMultiSetTest, size)
{
    EXPECT_EQ(mset.size(), 7);
}

// 修改器

TEST_F(WWMultiSetTest, clear)
{
    mset.clear();
    EXPECT_EQ(mset.empty(), true);
}

TEST_F(WWMultiSetTest, insert)
{
    mset.insert({5, 5, 5, 5});
    EXPECT_EQ(mset.size(), 11);
    EXPECT_EQ(mset.count(5), 4);
}

TEST_F(WWMultiSetTest, erase)
{
    mset.erase(mset.find(3));
    EXPECT_EQ(mset.count(3), 0);

    mset.erase(mset.begin(), mset.end());
    EXPECT_TRUE(mset.empty());
}

// 查找

TEST_F(WWMultiSetTest, count)
{
    EXPECT_EQ(mset.count(1), 2);
    EXPECT_EQ(mset.count(2), 3);
    EXPECT_EQ(mset.count(3), 1);
    EXPECT_EQ(mset.count(4), 1);
    EXPECT_EQ(mset.count(5), 0);
}

TEST_F(WWMultiSetTest, find)
{
    EXPECT_EQ(*mset.find(1), 1);
    EXPECT_EQ(*mset.find(2), 2);
    EXPECT_EQ(*mset.find(3), 3);
    EXPECT_EQ(*mset.find(4), 4);
    EXPECT_EQ(mset.find(5), mset.end());
}

TEST_F(WWMultiSetTest, equal_range)
{
    auto range = mset.equal_range(1);
    EXPECT_EQ(*range.first, 1);
    EXPECT_EQ(*range.second, 2);
}

TEST_F(WWMultiSetTest, lower_bound)
{
    EXPECT_EQ(*mset.lower_bound(1), 1);
    EXPECT_EQ(*mset.lower_bound(2), 2);
    EXPECT_EQ(*mset.lower_bound(3), 3);
    EXPECT_EQ(*mset.lower_bound(4), 4);
    EXPECT_EQ(mset.lower_bound(5), mset.end());
}

TEST_F(WWMultiSetTest, upper_bound)
{
    EXPECT_EQ(*mset.upper_bound(1), 2);
    EXPECT_EQ(*mset.upper_bound(2), 3);
    EXPECT_EQ(*mset.upper_bound(3), 4);
    EXPECT_EQ(mset.upper_bound(4), mset.end());
    EXPECT_EQ(mset.upper_bound(5), mset.end());
}