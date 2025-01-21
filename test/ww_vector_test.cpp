#include <gtest/gtest.h>
#include <ww_vector.h>

using namespace wwstl;

class EmplaceTest
{
public:
    const char * name;
    int value;

public:
    EmplaceTest(const char * name, int value)
        : name(name), value(value)
    {
    }
};

class WWVectorTest : public testing::Test
{
public:
    vector<int> vec = {1, 2, 3};
};

// 元素访问
TEST_F(WWVectorTest, ElementAccess)
{
    EXPECT_EQ(vec.front(), 1);
    EXPECT_EQ(vec.back(), 3);
    EXPECT_EQ(vec.at(1), 2);
    EXPECT_EQ(vec.at(2), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec.data()[0], 1);

    EXPECT_THROW(vec.at(3), std::out_of_range);
}

// 迭代器
TEST_F(WWVectorTest, Iterator)
{
    auto it = vec.begin();
    EXPECT_EQ(*(it + 1), 2);

    for (int i = 1; i <= vec.size(); ++i) {
        EXPECT_EQ(*it, i);
        ++it;
    }

    it = vec.end();
    EXPECT_EQ(*(it - 1), 3);

    for (int i = vec.size(); i >= 1; --i) {
        --it;
        EXPECT_EQ(*it, i);
    }

    auto rit = vec.rbegin();
    for (int i = vec.size(); i >= 1; --i) {
        EXPECT_EQ(*rit, i);
        ++rit;
    }

    rit = vec.rend();
    for (int i = 1; i <= vec.size(); ++i) {
        --rit;
        EXPECT_EQ(*rit, i);
    }
}

// 容量
TEST_F(WWVectorTest, Capacity)
{
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 3);

    vector<int> vec2;
    vec2.reserve(11);
    EXPECT_TRUE(vec2.empty());
    EXPECT_EQ(vec2.size(), 0);
    EXPECT_EQ(vec2.capacity(), 11);
    vec2.insert(vec2.begin(), 3, 7);
    vec.shrink_to_fit();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 3);
}

// 修改器
TEST_F(WWVectorTest, clear)
{
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 3);
}

TEST_F(WWVectorTest, insert)
{
    vec.insert(vec.begin() + 1, 7);
    vec.insert(vec.begin() + 2, 8);
    vec.insert(vec.end(), 3, 666);
    vec.insert(vec.begin(), {9, 8, 7});
    EXPECT_EQ(vec, vector<int>({9, 8, 7, 1, 7, 8, 2, 3, 666, 666, 666}));
}

TEST_F(WWVectorTest, erase)
{
    vec.erase(vec.begin() + 1);
    EXPECT_EQ(vec, vector<int>({1, 3}));

    vector<int> vec2 = {1, 2, 3, 4, 5};
    vec2.erase(vec2.begin() + 2, vec2.begin() + 4);
    EXPECT_EQ(vec2, vector<int>({1, 2, 5}));
}

TEST_F(WWVectorTest, push_back)
{
    int a = 4;
    vec.push_back(a);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4}));
    EXPECT_EQ(vec.size(), 4);
    EXPECT_EQ(vec.capacity(), 6);

    vec.push_back(5);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4, 5}));
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 6);
}

TEST_F(WWVectorTest, emplace_back)
{
    vector<EmplaceTest> vec2;
    vec2.emplace_back("emplace_back", 1);
    EmplaceTest b = vec2.back();
    EXPECT_EQ(b.name, "emplace_back");
    EXPECT_EQ(b.value, 1);
    EXPECT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2.capacity(), 1);
}

TEST_F(WWVectorTest, pop_back)
{
    vec.pop_back();
    EXPECT_EQ(vec, vector<int>({1, 2}));
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 3);
}

TEST_F(WWVectorTest, resize)
{
    vec.resize(10, 7);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 7, 7, 7, 7, 7, 7, 7}));
    EXPECT_EQ(vec.size(), 10);
    EXPECT_EQ(vec.capacity(), 10);
}

TEST_F(WWVectorTest, swap)
{
    vector<int> vec2 = {1, 2, 3, 4, 5};
    swap(vec, vec2);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4, 5}));
    EXPECT_EQ(vec2, vector<int>({1, 2, 3}));
}

// 赋值

TEST_F(WWVectorTest, assign)
{
    vec.assign(5, 7);
    EXPECT_EQ(vec, vector<int>({7, 7, 7, 7, 7}));

    vector<int> vec2 = {1, 2, 3, 4, 5};
    vec.assign(vec2.begin(), vec2.end());
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4, 5}));

    vec.assign({6, 6, 6, 6, 6});
    EXPECT_EQ(vec, vector<int>({6, 6, 6, 6, 6}));

    vec = std::move(vec2);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4, 5}));
    EXPECT_EQ(vec2.size(), 0);
}