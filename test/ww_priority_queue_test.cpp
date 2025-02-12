#include <gtest/gtest.h>
#include <ww_queue.h>

using namespace wwstl;

class WWPriorityQueueTest : public testing::Test
{
public:
    priority_queue<int> pq;
};

TEST_F(WWPriorityQueueTest, capacity)
{
    EXPECT_TRUE(pq.empty());
    EXPECT_EQ(pq.size(), 0);
}

// 修改器

TEST_F(WWPriorityQueueTest, push)
{
    pq.push(30);
    pq.push(10);
    pq.push(15);
    pq.push(70);
    pq.push(56);
    pq.push(25);
    pq.push(83);

    EXPECT_FALSE(pq.empty());
    EXPECT_EQ(pq.size(), 7);
    EXPECT_EQ(pq.top(), 83);
}

TEST_F(WWPriorityQueueTest, pop)
{
    pq.push(30);
    pq.push(10);
    pq.push(15);
    pq.push(70);
    pq.push(56);
    pq.push(25);
    pq.push(83);

    pq.pop();
    pq.pop();
    pq.pop();
    
    EXPECT_FALSE(pq.empty());
    EXPECT_EQ(pq.size(), 4);
    EXPECT_EQ(pq.top(), 30);
}

// 带比较器的优先队列
TEST_F(WWPriorityQueueTest, compare)
{
    priority_queue<int, vector<int>, std::greater<int>> pq2;
    pq2.push(30);
    pq2.push(10);
    pq2.push(15);
    pq2.push(70);
    pq2.push(56);
    pq2.push(25);
    pq2.push(83);

    EXPECT_EQ(pq2.top(), 10);
    pq2.pop();
    EXPECT_EQ(pq2.top(), 15);
    pq2.pop();
    EXPECT_EQ(pq2.top(), 25);
}