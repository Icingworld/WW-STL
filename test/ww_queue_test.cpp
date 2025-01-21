#include <gtest/gtest.h>
#include <ww_queue.h>

using namespace wwstl;

class WWQueueTest : public testing::Test
{
public:
    queue<int> s;
};

// 容量
TEST_F(WWQueueTest, Capacity)
{
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

// 修改器

TEST_F(WWQueueTest, push)
{
    s.push(1);
    s.push(2);
    s.push(3);
}

TEST_F(WWQueueTest, pop)
{
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    s.pop();
    s.pop();
}

// 访问
TEST_F(WWQueueTest, front)
{
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.front(), 1);
    s.pop();
    EXPECT_EQ(s.front(), 2);
    s.pop();
    EXPECT_EQ(s.front(), 3);
}

TEST_F(WWQueueTest, back)
{
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.back(), 3);
    s.pop();
    EXPECT_EQ(s.back(), 3);
    s.pop();
    EXPECT_EQ(s.back(), 3);
}