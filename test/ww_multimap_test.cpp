#include <gtest/gtest.h>
#include <ww_map.h>

using namespace wwstl;

class WWMultiMapTest : public testing::Test
{
public:
    multimap<int, std::string> mmap;

protected:
    void SetUp() override
    {
        mmap.insert({1, "1"});
        mmap.insert({2, "2"});
        mmap.insert({3, "3"});
        mmap.insert({4, "4"});
        mmap.insert({1, "11"});
        mmap.insert({2, "22"});
        mmap.insert({2, "222"});
    }
};

// 迭代器

TEST_F(WWMultiMapTest, begin)
{
    EXPECT_EQ(mmap.begin()->second, "1");
}

TEST_F(WWMultiMapTest, end)
{
    EXPECT_EQ((--mmap.end())->second, "4");
}

TEST_F(WWMultiMapTest, rbegin)
{
    EXPECT_EQ(mmap.rbegin()->second, "4");
}

TEST_F(WWMultiMapTest, rend)
{
    EXPECT_EQ((--mmap.rend())->second, "1");
}

// 容量

TEST_F(WWMultiMapTest, empty)
{
    EXPECT_EQ(mmap.empty(), false);
}

TEST_F(WWMultiMapTest, size)
{
    EXPECT_EQ(mmap.size(), 7);
}

// 修改器

TEST_F(WWMultiMapTest, clear)
{
    mmap.clear();
    EXPECT_EQ(mmap.size(), 0);
}

TEST_F(WWMultiMapTest, insert)
{
    mmap.insert(std::pair<const int, std::string>(5, "5"));

    mmap.insert({
        {6, "6"},
        {7, "7"},
        {7, "77"},
        {7, "777"}
    });

    EXPECT_EQ(mmap.size(), 12);
}

TEST_F(WWMultiMapTest, erase)
{
    mmap.erase(mmap.find(3));
    EXPECT_EQ(mmap.size(), 6);

    mmap.erase(2);
    EXPECT_EQ(mmap.size(), 3);

    mmap.erase(mmap.begin(), mmap.end());
    EXPECT_TRUE(mmap.empty());
}

// 查找

TEST_F(WWMultiMapTest, count)
{
    EXPECT_EQ(mmap.count(1), 2);
    EXPECT_EQ(mmap.count(2), 3);
    EXPECT_EQ(mmap.count(3), 1);
    EXPECT_EQ(mmap.count(4), 1);
    EXPECT_EQ(mmap.count(5), 0);
}

TEST_F(WWMultiMapTest, find)
{
    EXPECT_EQ(mmap.find(1)->second, "1");
    EXPECT_EQ(mmap.find(2)->second, "2");
    EXPECT_EQ(mmap.find(3)->second, "3");
    EXPECT_EQ(mmap.find(4)->second, "4");
    EXPECT_EQ(mmap.find(5), mmap.end());
}

TEST_F(WWMultiMapTest, equal_range)
{
    auto range = mmap.equal_range(1);
    EXPECT_EQ(range.first->second, "1");
    EXPECT_EQ(range.second->second, "2");
}

TEST_F(WWMultiMapTest, lower_bound)
{
    EXPECT_EQ(mmap.lower_bound(1)->second, "1");
    EXPECT_EQ(mmap.lower_bound(2)->second, "2");
    EXPECT_EQ(mmap.lower_bound(3)->second, "3");
    EXPECT_EQ(mmap.lower_bound(4)->second, "4");
    EXPECT_EQ(mmap.lower_bound(5), mmap.end());
}

TEST_F(WWMultiMapTest, upper_bound)
{
    EXPECT_EQ(mmap.upper_bound(1)->second, "2");
    EXPECT_EQ(mmap.upper_bound(2)->second, "3");
    EXPECT_EQ(mmap.upper_bound(3)->second, "4");
    EXPECT_EQ(mmap.upper_bound(4), mmap.end());
    EXPECT_EQ(mmap.upper_bound(5), mmap.end());
}