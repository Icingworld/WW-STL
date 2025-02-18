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

TEST_F(WWVectorTest, Clear)
{
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 3);
}

TEST_F(WWVectorTest, Insert)
{
    vec.insert(vec.begin() + 1, 7);
    vec.insert(vec.begin() + 2, 8);
    vec.insert(vec.end(), 3, 666);
    vec.insert(vec.begin(), {9, 8, 7});
    EXPECT_EQ(vec, vector<int>({9, 8, 7, 1, 7, 8, 2, 3, 666, 666, 666}));
}

TEST_F(WWVectorTest, Erase)
{
    vec.erase(vec.begin() + 1);
    EXPECT_EQ(vec, vector<int>({1, 3}));

    vector<int> vec2 = {1, 2, 3, 4, 5};
    vec2.erase(vec2.begin() + 2, vec2.begin() + 4);
    EXPECT_EQ(vec2, vector<int>({1, 2, 5}));
}

TEST_F(WWVectorTest, PushBack)
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

TEST_F(WWVectorTest, EmplaceBack)
{
    vector<EmplaceTest> vec2;
    vec2.emplace_back("emplace_back", 1);
    EmplaceTest b = vec2.back();
    EXPECT_EQ(b.name, "emplace_back");
    EXPECT_EQ(b.value, 1);
    EXPECT_EQ(vec2.size(), 1);
    EXPECT_EQ(vec2.capacity(), 1);
}

TEST_F(WWVectorTest, PopBack)
{
    vec.pop_back();
    EXPECT_EQ(vec, vector<int>({1, 2}));
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 3);
}

TEST_F(WWVectorTest, Resize)
{
    vec.resize(10, 7);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 7, 7, 7, 7, 7, 7, 7}));
    EXPECT_EQ(vec.size(), 10);
    EXPECT_EQ(vec.capacity(), 10);
}

TEST_F(WWVectorTest, Swap)
{
    vector<int> vec2 = {1, 2, 3, 4, 5};
    swap(vec, vec2);
    EXPECT_EQ(vec, vector<int>({1, 2, 3, 4, 5}));
    EXPECT_EQ(vec2, vector<int>({1, 2, 3}));
}

// 赋值

TEST_F(WWVectorTest, Assign)
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

// vector<bool>

class WWVectorBoolTest : public testing::Test
{
public:
    vector<bool> vec = {true, false, true, false};
};

// 元素访问

TEST_F(WWVectorBoolTest, ElementAccess)
{
    EXPECT_TRUE(vec[0]);
    EXPECT_FALSE(vec[1]);

    EXPECT_TRUE(vec.at(2));
    EXPECT_FALSE(vec.at(1));

    EXPECT_THROW(vec.at(4), std::out_of_range);

    EXPECT_TRUE(vec.front());
    EXPECT_FALSE(vec.back());
}

// 迭代器

TEST_F(WWVectorBoolTest, Iterator)
{
    int i = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it, ++i) {
        if (i % 2 == 0) {
            EXPECT_TRUE(*it);
        } else {
            EXPECT_FALSE(*it);
        }
    }
}

// 容量

TEST_F(WWVectorBoolTest, Capacity)
{
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 4);
}

// 修改器

void printVector(const vector<bool> &vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << (bool)(*it) << " ";
    }
    std::cout << std::endl;
}

TEST_F(WWVectorBoolTest, Clear)
{
    vec.clear();
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}

TEST_F(WWVectorBoolTest, Insert)
{
    vec.insert(vec.begin(), false);
    printVector(vec);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_FALSE(vec[0]);
    EXPECT_TRUE(vec[1]);
    
    vec.insert(vec.begin() + 2, true);
    EXPECT_TRUE(vec[2]);
    EXPECT_FALSE(vec[3]);
    
    vec.insert(vec.end(), 3, true);
    EXPECT_EQ(vec.size(), 9);
    EXPECT_TRUE(vec[8]);
    EXPECT_TRUE(vec[7]);
    EXPECT_TRUE(vec[6]);
    
    std::vector<bool> temp = {false, true, false};
    vec.insert(vec.begin() + 1, temp.begin(), temp.end());
    EXPECT_EQ(vec.size(), 12);
    EXPECT_FALSE(vec[1]);
    EXPECT_TRUE(vec[2]);
    EXPECT_FALSE(vec[3]);
}

TEST_F(WWVectorBoolTest, Erase)
{
    vec.erase(vec.begin());
    EXPECT_EQ(vec.size(), 3);
    EXPECT_FALSE(vec[0]);
    EXPECT_TRUE(vec[1]);
    
    vec.erase(vec.begin() + 1, vec.end());
    EXPECT_EQ(vec.size(), 1);
    EXPECT_FALSE(vec[0]);
}

TEST_F(WWVectorBoolTest, PushBack)
{
    vec.push_back(true);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_TRUE(vec.back());
    
    vec.push_back(false);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_FALSE(vec.back());
}

TEST_F(WWVectorBoolTest, PopBack)
{
    vec.pop_back();
    EXPECT_EQ(vec.size(), 3);
    EXPECT_TRUE(vec.back());
    
    vec.pop_back();
    EXPECT_EQ(vec.size(), 2);
    EXPECT_FALSE(vec.back());
}

TEST_F(WWVectorBoolTest, Resize)
{
    vec.resize(10);
    EXPECT_EQ(vec.size(), 10);
    for (int i = 3; i < 10; ++i) {
        EXPECT_FALSE(vec[i]);
    }

    vec.resize(15, true);
    EXPECT_TRUE(vec[12]);
}

// 翻转

TEST_F(WWVectorBoolTest, Flip)
{
    vec.flip();
    EXPECT_FALSE(vec[0]);
    EXPECT_TRUE(vec[1]);
    EXPECT_FALSE(vec[2]);
    EXPECT_TRUE(vec[3]);
}