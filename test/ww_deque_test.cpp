#include <gtest/gtest.h>
#include <ww_deque.h>

using namespace wwstl;

class WWDequeTest : public testing::Test
{
public:
    deque<int> dq;

protected:
    void SetUp() override
    {
        for (int i = 0; i < 10; i++) {
            dq.emplace_back(i);
        }
    }
};

// 元素访问

TEST_F(WWDequeTest, at)
{
    EXPECT_EQ(dq.at(0), 0);
    EXPECT_EQ(dq.at(3), 3);
    EXPECT_EQ(dq.at(9), 9);

    EXPECT_THROW(dq.at(10), std::out_of_range);
}

TEST_F(WWDequeTest, operator_access)
{
    EXPECT_EQ(dq[0], 0);
    EXPECT_EQ(dq[3], 3);
    EXPECT_EQ(dq[9], 9);
}

TEST_F(WWDequeTest, front)
{
    EXPECT_EQ(dq.front(), 0);

    dq.push_front(100);
    dq.push_front(200);
    EXPECT_EQ(dq.front(), 200);

    dq.pop_front();
    EXPECT_EQ(dq.front(), 100);
}

TEST_F(WWDequeTest, back)
{
    EXPECT_EQ(dq.back(), 9);

    dq.pop_back();
    EXPECT_EQ(dq.back(), 8);
    dq.pop_back();
    EXPECT_EQ(dq.back(), 7);
}

// 迭代器

TEST_F(WWDequeTest, Iterator)
{
    auto it = dq.begin();
    for (int i = 0; it != dq.end(); ++i, ++it) {
        EXPECT_EQ(*it, i);
    }

    auto it2 = dq.rbegin();
    for (int i = 9; it2 != dq.rend(); --i, ++it2) {
        EXPECT_EQ(*it2, i);
    }
}

// 容量

TEST_F(WWDequeTest, empty)
{
    EXPECT_FALSE(dq.empty());
}

TEST_F(WWDequeTest, size)
{
    EXPECT_EQ(dq.size(), 10);
}

// 修改器

TEST_F(WWDequeTest, clear)
{
    dq.clear();
    EXPECT_TRUE(dq.empty());
}

TEST_F(WWDequeTest, insert)
{
    dq.insert(dq.begin() + 1, 100);
    EXPECT_EQ(dq[1], 100);
    dq.insert(dq.begin() + 3, 200);
    EXPECT_EQ(dq[3], 200);

    dq.insert(dq.begin() + 5, 10, 300);
    for (int i = 5; i < 15; i++) {
        EXPECT_EQ(dq[i], 300);
    }

    dq.insert(dq.begin() + 15, 5, 400);
    for (int i = 15; i < 20; i++) {
        EXPECT_EQ(dq[i], 400);
    }
}

TEST_F(WWDequeTest, erase)
{
    dq.erase(dq.begin() + 1);
    EXPECT_EQ(dq[1], 2);
    dq.erase(dq.begin() + 3);
    EXPECT_EQ(dq[3], 5);

    dq.erase(dq.begin() + 1, dq.begin() + 3);
    EXPECT_EQ(dq.size(), 6);
    EXPECT_EQ(dq[0], 0);
    for (int i = 1; i < 6; i++) {
        EXPECT_EQ(dq[i], i + 4);
    }
}

TEST_F(WWDequeTest, emplace_front)
{
    dq.emplace_front(100);
    EXPECT_EQ(dq.front(), 100);
    dq.emplace_front(200);
    EXPECT_EQ(dq.front(), 200);
}

TEST_F(WWDequeTest, emplace_back)
{
    dq.emplace_back(100);
    EXPECT_EQ(dq.back(), 100);
    dq.emplace_back(200);
    EXPECT_EQ(dq.back(), 200);
}

TEST_F(WWDequeTest, pop_front)
{
    dq.pop_front();
    EXPECT_EQ(dq.front(), 1);
    dq.pop_front();
    EXPECT_EQ(dq.front(), 2);
}

TEST_F(WWDequeTest, pop_back)
{
    dq.pop_back();
    EXPECT_EQ(dq.back(), 8);
    dq.pop_back();
    EXPECT_EQ(dq.back(), 7);
}

TEST_F(WWDequeTest, resize)
{
    dq.resize(5);
    EXPECT_EQ(dq.size(), 5);
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(dq[i], i);
    }

    dq.resize(16, 777);
    EXPECT_EQ(dq.size(), 16);
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(dq[i], i < 5 ? i : 777);
    }
}