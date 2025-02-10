#include <gtest/gtest.h>
#include <ww_forward_list.h>

using namespace wwstl;

class WWForwardListTest : public ::testing::Test
{
public:
    forward_list<int> flist;

protected:
    void SetUp() override
    {
        flist.push_front(1);
        flist.push_front(2);
        flist.push_front(3);
    }
};

// 元素访问

TEST_F(WWForwardListTest, front)
{
    EXPECT_EQ(flist.front(), 3);
}

// 迭代器

TEST_F(WWForwardListTest, Iterator)
{
    EXPECT_EQ(++flist.before_begin(), flist.begin());

    auto it = flist.begin();
    for (int i = 3; i >= 1; --i) {
        EXPECT_EQ(*it++, i);
    }
}

// 容量

TEST_F(WWForwardListTest, empty)
{
    EXPECT_FALSE(flist.empty());
}

// 修改器

TEST_F(WWForwardListTest, clear)
{
    flist.clear();
    EXPECT_TRUE(flist.empty());
}

TEST_F(WWForwardListTest, insert)
{
    flist.insert_after(flist.begin(), 4);
    forward_list<int> expect1 = {3, 4, 2, 1};
    EXPECT_EQ(flist, expect1);

    flist.insert_after(flist.begin(), {5, 6});
    forward_list<int> expect2 = {3, 5, 6, 4, 2, 1};
    EXPECT_EQ(flist, expect2);

    flist.insert_after(flist.begin(), 3, 666);
    forward_list<int> expect3 = {3, 666, 666, 666, 5, 6, 4, 2, 1};
    EXPECT_EQ(flist, expect3);
}

TEST_F(WWForwardListTest, emplace_after)
{
    flist.emplace_after(flist.begin(), 4);
    forward_list<int> expect1 = {3, 4, 2, 1};
    EXPECT_EQ(flist, expect1);
}

TEST_F(WWForwardListTest, erase_after)
{
    flist.erase_after(flist.begin());
    forward_list<int> expect1 = {3, 1};
    EXPECT_EQ(flist, expect1);

    forward_list<int> flist1 = {1, 2, 3, 4, 5, 6};
    flist1.erase_after(flist1.begin(), std::next(flist1.begin(), 3));
    forward_list<int> expect2 = {1, 4, 5, 6};
    EXPECT_EQ(flist1, expect2);
}

TEST_F(WWForwardListTest, emplace_front)
{
    flist.emplace_front(4);
    forward_list<int> expect1 = {4, 3, 2, 1};
    EXPECT_EQ(flist, expect1);
}

TEST_F(WWForwardListTest, pop_front)
{
    flist.pop_front();
    forward_list<int> expect1 = {2, 1};
    EXPECT_EQ(flist, expect1);
}

TEST_F(WWForwardListTest, resize)
{
    flist.resize(1);
    forward_list<int> expect1 = {3};
    EXPECT_EQ(flist, expect1);

    flist.resize(5, 666);
    forward_list<int> expect2 = {3, 666, 666, 666, 666};
    EXPECT_EQ(flist, expect2);
}

// 操作

TEST_F(WWForwardListTest, merge)
{
    forward_list<int> flist2 = {8, 5, 4, 1, 0};
    flist.merge(flist2, std::greater<int>());
    forward_list<int> expect1 = {8, 5, 4, 3, 2, 1, 1, 0};
    EXPECT_EQ(flist, expect1);
}

TEST_F(WWForwardListTest, splice_after)
{
    forward_list<int> flist1 = {1, 2, 3};
    flist.splice_after(flist.begin(), flist1);
    forward_list<int> expect1 = {3, 1, 2, 3, 2, 1};
    EXPECT_EQ(flist, expect1);

    forward_list<int> flist2 = {8, 5, 4, 1, 0};
    auto it = ++flist2.begin();
    flist.splice_after(flist.begin(), flist2, it);
    forward_list<int> expect2 = {3, 4, 1, 2, 3, 2, 1};
    EXPECT_EQ(flist, expect2);

    forward_list<int> flist3 = {4, 5, 1, 7, 9};
    auto first = flist3.begin();
    auto last = flist3.end();
    flist.splice_after(flist.begin(), flist3, first, last);
    forward_list<int> expect3 = {3, 5, 1, 7, 9, 4, 1, 2, 3, 2, 1};
    EXPECT_EQ(flist, expect3);
}

TEST_F(WWForwardListTest, remove)
{
    flist.remove(3);
    forward_list<int> expect1 = {2, 1};
    EXPECT_EQ(flist, expect1);

    forward_list<int> flist1 = {8, 5, 5, 5, 4, 4, 1, 0, 0};
    flist1.remove_if([](int x) {
        return x >= 1 && x <= 5;
    });
    forward_list<int> expect2 = {8, 0, 0};
    EXPECT_EQ(flist1, expect2);
}

TEST_F(WWForwardListTest, reverse)
{
    flist.reverse();
    forward_list<int> expect1 = {1, 2, 3};
    EXPECT_EQ(flist, expect1);
}

TEST_F(WWForwardListTest, unique)
{
    forward_list<int> flist1 = {8, 5, 5, 5, 4, 4, 1, 0, 0};
    flist1.unique();
    forward_list<int> expect1 = {8, 5, 4, 1, 0};
    EXPECT_EQ(flist1, expect1);
}

TEST_F(WWForwardListTest, sort)
{
    flist.sort();
    forward_list<int> expect1 = {1, 2, 3};
    EXPECT_EQ(flist, expect1);
}