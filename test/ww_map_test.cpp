#include <gtest/gtest.h>
#include <ww_map.h>

using namespace wwstl;

class WWMapTest : public testing::Test
{
public:
    map<int, std::string> m;

protected:
    void SetUp() override
    {
        m.insert({1, "1"});
        m.insert({2, "2"});
        m.insert({3, "3"});
        m.insert({4, "4"});
    }
};

// 元素访问

TEST_F(WWMapTest, at)
{
    EXPECT_EQ(m.at(1), "1");
    EXPECT_EQ(m.at(2), "2");
    EXPECT_EQ(m.at(3), "3");
    EXPECT_EQ(m.at(4), "4");
    EXPECT_THROW(m.at(5), std::out_of_range);
}

TEST_F(WWMapTest, operator_access)
{
    EXPECT_EQ(m[1], "1");
    EXPECT_EQ(m[2], "2");
    EXPECT_EQ(m[3], "3");
    EXPECT_EQ(m[4], "4");

    m[5] = "5";
    EXPECT_EQ(m[5], "5");
}

// 迭代器

TEST_F(WWMapTest, begin)
{
    EXPECT_EQ(m.begin()->second, "1");
}

TEST_F(WWMapTest, end)
{
    EXPECT_EQ((--m.end())->second, "4");
}

TEST_F(WWMapTest, rbegin)
{
    EXPECT_EQ(m.rbegin()->second, "4");
}

TEST_F(WWMapTest, rend)
{
    EXPECT_EQ((--m.rend())->second, "1");
}

// 容量

TEST_F(WWMapTest, empty)
{
    EXPECT_EQ(m.empty(), false);
}

TEST_F(WWMapTest, size)
{
    EXPECT_EQ(m.size(), 4);
}

// 修改器

TEST_F(WWMapTest, clear)
{
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0);
}

TEST_F(WWMapTest, insert)
{
    auto it = m.insert({2, "5"});
    EXPECT_FALSE(it.second);

    m.insert({
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
        {9, "9"},
        {5, "55"},
        {6, "66"}
    });
    EXPECT_EQ(m.size(), 9);
}

TEST_F(WWMapTest, erase)
{
    m.erase(1);
    EXPECT_EQ(m.size(), 3);

    m.erase(m.begin(), m.end());
    EXPECT_TRUE(m.empty());
}

// 查找

TEST_F(WWMapTest, count)
{
    EXPECT_EQ(m.count(1), 1);
    EXPECT_EQ(m.count(2), 1);
    EXPECT_EQ(m.count(3), 1);
    EXPECT_EQ(m.count(4), 1);
    EXPECT_EQ(m.count(5), 0);
}

TEST_F(WWMapTest, find)
{
    EXPECT_EQ(m.find(1)->second, "1");
    EXPECT_EQ(m.find(2)->second, "2");
    EXPECT_EQ(m.find(3)->second, "3");
    EXPECT_EQ(m.find(4)->second, "4");
    EXPECT_EQ(m.find(5), m.end());
}

TEST_F(WWMapTest, equal_range)
{
    auto range = m.equal_range(1);
    EXPECT_EQ(range.first->second, "1");
    EXPECT_EQ(range.second->second, "2");
}

TEST_F(WWMapTest, lower_bound)
{
    EXPECT_EQ(m.lower_bound(1)->second, "1");
    EXPECT_EQ(m.lower_bound(2)->second, "2");
    EXPECT_EQ(m.lower_bound(3)->second, "3");
    EXPECT_EQ(m.lower_bound(4)->second, "4");
    EXPECT_EQ(m.lower_bound(5), m.end());
}

TEST_F(WWMapTest, upper_bound)
{
    EXPECT_EQ(m.upper_bound(1)->second, "2");
    EXPECT_EQ(m.upper_bound(2)->second, "3");
    EXPECT_EQ(m.upper_bound(3)->second, "4");
    EXPECT_EQ(m.upper_bound(4), m.end());
    EXPECT_EQ(m.upper_bound(5), m.end());
}

// 比较

TEST_F(WWMapTest, compare)
{
    map<int, std::string> m2 = {{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}};
    EXPECT_TRUE(m == m2);

    map<int, std::string> m3 = {{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}};
    EXPECT_FALSE(m == m3);
    EXPECT_TRUE(m < m3);

    map<int, std::string> m4 = {{1, "1"}, {2, "2"}, {3, "3"}, {4, "7"}};
    EXPECT_FALSE(m == m4);
    EXPECT_TRUE(m < m4);
}