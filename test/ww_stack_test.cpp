#include <gtest/gtest.h>
#include <ww_stack.h>

using namespace wwstl;

class WWStackTest : public testing::Test
{
public:
    stack<int> s;
};

// 容量
TEST_F(WWStackTest, Capacity)
{
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

// 修改器

TEST_F(WWStackTest, push)
{
    s.push(1);
    s.push(2);
    s.push(3);
}

TEST_F(WWStackTest, pop)
{
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    s.pop();
    s.pop();
}

// 访问
TEST_F(WWStackTest, top)
{
    s.push(1);
    s.push(2);
    s.push(3);
    EXPECT_EQ(s.top(), 3);
    s.pop();
    EXPECT_EQ(s.top(), 2);
    s.pop();
    EXPECT_EQ(s.top(), 1);
}