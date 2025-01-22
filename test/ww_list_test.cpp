#include <gtest/gtest.h>
#include <ww_list.h>

using namespace wwstl;

class WWListTest : public testing::Test
{
public:
    list<int> wwlist;

protected:
    void SetUp() override
    {
        wwlist.push_back(1);
        wwlist.push_back(2);
        wwlist.push_back(3);
    }
};

// 元素访问

TEST_F(WWListTest, front)
{
    EXPECT_EQ(wwlist.front(), 1);
}

TEST_F(WWListTest, back)
{
    EXPECT_EQ(wwlist.back(), 3);
}

// 迭代器

TEST_F(WWListTest, iterator)
{
    auto it = wwlist.begin();
    for (int i = 1; i <= 3; ++i, ++it) {
        EXPECT_EQ(*it, i);
    }

    it = wwlist.end();
    --it;
    for (int i = 3; i >= 1; --i, --it) {
        EXPECT_EQ(*it, i);
    }

    auto rit = wwlist.rbegin();
    for (int i = 3; i >= 1; --i, ++rit) {
        EXPECT_EQ(*rit, i);
    }

    rit = wwlist.rend();
    --rit;
    for (int i = 1; i <= 3; ++i, --rit) {
        EXPECT_EQ(*rit, i);
    }
}

// 容量

TEST_F(WWListTest, empty)
{
    EXPECT_EQ(wwlist.empty(), false);
}

TEST_F(WWListTest, size)
{
    EXPECT_EQ(wwlist.size(), 3);
}

// 修改器

TEST_F(WWListTest, clear)
{
    wwlist.clear();
    EXPECT_EQ(wwlist.size(), 0);
}

TEST_F(WWListTest, insert)
{
    wwlist.insert(++wwlist.begin(), 0);
    auto it = wwlist.begin();
    EXPECT_EQ(*++it, 0);

    wwlist.insert(++wwlist.begin(), {666, 666, 666});
    it = wwlist.begin();
    ++it;
    for (int i = 0; i < 3; ++i, ++it) {
        EXPECT_EQ(*it, 666);
    }
}

TEST_F(WWListTest, erase)
{
    wwlist.erase(++wwlist.begin());
    auto it = wwlist.begin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*++it, 3);

    wwlist.erase(wwlist.begin(), wwlist.end());
    EXPECT_EQ(wwlist.size(), 0);
}

TEST_F(WWListTest, emplace_back)
{
    wwlist.emplace_back(4);
    EXPECT_EQ(wwlist.back(), 4);
    wwlist.emplace_back(5);
    EXPECT_EQ(wwlist.back(), 5);
}

TEST_F(WWListTest, emplace_front)
{
    wwlist.emplace_front(0);
    EXPECT_EQ(wwlist.front(), 0);
    wwlist.emplace_front(-1);
    EXPECT_EQ(wwlist.front(), -1);
}

TEST_F(WWListTest, pop_back)
{
    wwlist.pop_back();
    EXPECT_EQ(wwlist.back(), 2);
    wwlist.pop_back();
    EXPECT_EQ(wwlist.back(), 1);
}

TEST_F(WWListTest, pop_front)
{
    wwlist.pop_front();
    EXPECT_EQ(wwlist.front(), 2);
    wwlist.pop_front();
    EXPECT_EQ(wwlist.front(), 3);
}

TEST_F(WWListTest, resize)
{
    wwlist.resize(1);
    EXPECT_EQ(wwlist.size(), 1);

    wwlist.resize(5, 666);
    EXPECT_EQ(wwlist.size(), 5);
    auto it = wwlist.begin();
    EXPECT_EQ(*it, 1);
    for (int i = 1; i < 4; ++i) {
        EXPECT_EQ(*++it, 666);
    }
}

// 操作

TEST_F(WWListTest, merge)
{
    list<int> wwlist1 = {0, 1, 6, 7};
    list<int> wwlist2 = {0, 1, 1, 2, 3, 6, 7};
    wwlist.merge(wwlist1);
    EXPECT_EQ(wwlist, wwlist2);
}

TEST_F(WWListTest, splice)
{
    list<int> wwlist1 = {4, 5, 6};
    wwlist.splice(wwlist.end(), wwlist1);
    list<int> expect1 = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(wwlist, expect1);

    list<int> wwlist2 = {7, 8, 9};
    wwlist.splice(wwlist.begin(), wwlist2, ++wwlist2.begin());
    list<int> expect2 = {8, 1, 2, 3, 4, 5, 6};
    EXPECT_EQ(wwlist, expect2);

    list<int> wwlist3 = {10, 11, 12, 13, 14};
    auto first = ++wwlist3.begin();
    auto last = --wwlist3.end();
    wwlist.splice(wwlist.end(), wwlist3, first, last);
    list<int> expect3 = {8, 1, 2, 3, 4, 5, 6, 11, 12, 13};
    EXPECT_EQ(wwlist, expect3);
}

TEST_F(WWListTest, remove)
{
    wwlist.remove(2);
    list<int> expect = {1, 3};
    EXPECT_EQ(wwlist, expect);

    list<int> wwlist1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    wwlist1.remove_if([](int x) {
        return x % 2 == 0;
    });
    list<int> expect1 = {1, 3, 5, 7, 9};
    EXPECT_EQ(wwlist1, expect1);
}

TEST_F(WWListTest, reverse)
{
    wwlist.reverse();
    list<int> expect = {3, 2, 1};
    EXPECT_EQ(wwlist, expect);
}

TEST_F(WWListTest, unique)
{
    list<int> wwlist1 = {1, 2, 2, 2, 3, 3, 4};
    wwlist1.unique();
    list<int> expect = {1, 2, 3, 4};
    EXPECT_EQ(wwlist1, expect);
}

TEST_F(WWListTest, sort)
{
    list<int> wwlist1 = {3, 2, 1, 6, 4, 8, 1, 3};
    wwlist1.sort();
    list<int> expect1 = {1, 1, 2, 3, 3, 4, 6, 8};
    EXPECT_EQ(wwlist1, expect1);
}