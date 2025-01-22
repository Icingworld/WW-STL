#include <gtest/gtest.h>
#include <ww_map.h>

using namespace wwstl;

class WWMapTest : public testing::Test
{
public:
    map<int, std::string> map;

protected:
    void SetUp() override
    {
        map.insert({1, "1"});
        map.insert({2, "2"});
        map.insert({3, "3"});
        map.insert({4, "4"});
    }
};

// 元素访问

TEST_F(WWMapTest, at)
{
    EXPECT_EQ(map.at(1), "1");
    EXPECT_EQ(map.at(2), "2");
    EXPECT_EQ(map.at(3), "3");
    EXPECT_EQ(map.at(4), "4");
    EXPECT_THROW(map.at(5), std::out_of_range);
}

TEST_F(WWMapTest, operator_access)
{
    EXPECT_EQ(map[1], "1");
    EXPECT_EQ(map[2], "2");
    EXPECT_EQ(map[3], "3");
    EXPECT_EQ(map[4], "4");

    map[5] = "5";
    EXPECT_EQ(map[5], "5");
}

// 迭代器

TEST_F(WWMapTest, begin)
{
    EXPECT_EQ(map.begin()->second, "1");
}

TEST_F(WWMapTest, end)
{
    EXPECT_EQ((--map.end())->second, "4");
}

TEST_F(WWMapTest, rbegin)
{
    EXPECT_EQ(map.rbegin()->second, "4");
}

TEST_F(WWMapTest, rend)
{
    EXPECT_EQ((--map.rend())->second, "1");
}

// 容量

TEST_F(WWMapTest, empty)
{
    EXPECT_EQ(map.empty(), false);
}

TEST_F(WWMapTest, size)
{
    EXPECT_EQ(map.size(), 4);
}

// 修改器

TEST_F(WWMapTest, clear)
{
    map.clear();
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);
}

TEST_F(WWMapTest, insert)
{
    map.insert({
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {5, "55"},
        {6, "66"}
    });
    EXPECT_EQ(map.size(), 9);
}

TEST_F(WWMapTest, erase)
{
    map.erase(1);
    EXPECT_EQ(map.size(), 3);

    map.erase(map.begin(), map.end());
    EXPECT_TRUE(map.empty());
}

// 查找

TEST_F(WWMapTest, count)
{
    EXPECT_EQ(map.count(1), 1);
    EXPECT_EQ(map.count(2), 1);
    EXPECT_EQ(map.count(3), 1);
    EXPECT_EQ(map.count(4), 1);
    EXPECT_EQ(map.count(5), 0);
}

TEST_F(WWMapTest, find)
{
    EXPECT_EQ(map.find(1)->second, "1");
    EXPECT_EQ(map.find(2)->second, "2");
    EXPECT_EQ(map.find(3)->second, "3");
    EXPECT_EQ(map.find(4)->second, "4");
    EXPECT_EQ(map.find(5), map.end());
}

TEST_F(WWMapTest, equal_range)
{
    auto range = map.equal_range(1);
    EXPECT_EQ(range.first->second, "1");
    EXPECT_EQ(range.second->second, "2");
}

TEST_F(WWMapTest, lower_bound)
{
    EXPECT_EQ(map.lower_bound(1)->second, "1");
    EXPECT_EQ(map.lower_bound(2)->second, "2");
    EXPECT_EQ(map.lower_bound(3)->second, "3");
    EXPECT_EQ(map.lower_bound(4)->second, "4");
    EXPECT_EQ(map.lower_bound(5), map.end());
}

TEST_F(WWMapTest, upper_bound)
{
    EXPECT_EQ(map.upper_bound(1)->second, "2");
    EXPECT_EQ(map.upper_bound(2)->second, "3");
    EXPECT_EQ(map.upper_bound(3)->second, "4");
    EXPECT_EQ(map.upper_bound(4), map.end());
    EXPECT_EQ(map.upper_bound(5), map.end());
}