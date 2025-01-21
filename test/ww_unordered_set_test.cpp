#include <gtest/gtest.h>
#include <ww_unordered_set.h>

using namespace wwstl;

class WWUnorderedSetTest : public testing::Test
{
public:
    unordered_set<std::string> uset;

protected:
    void SetUp() override
    {
        uset.insert("1");
        uset.insert("2");
        uset.insert("3");
        uset.insert("4");
    }
};

// 修改器

TEST_F(WWUnorderedSetTest, clear)
{
    uset.clear();
    EXPECT_TRUE(uset.empty());
    EXPECT_EQ(uset.size(), 0);
}

TEST_F(WWUnorderedSetTest, insert)
{
    auto it = uset.insert("5");
    EXPECT_TRUE(it.second);
    EXPECT_EQ(*it.first, "5");

    it = uset.insert("5");
    EXPECT_FALSE(it.second);

    uset.insert({"6", "7", "8"});
    EXPECT_EQ(uset.size(), 8);
}

TEST_F(WWUnorderedSetTest, erase)
{
    auto it = uset.erase(uset.begin());
    EXPECT_EQ(*it, "2");
    uset.insert({"6", "7", "8"});
    auto it1 = uset.begin();
    std::advance(it1, 2);
    it = uset.erase(uset.begin(), it1);
    EXPECT_EQ(*it, "3");
    int count = uset.erase("6");
    EXPECT_EQ(count, 1);
}

// 容量

TEST_F(WWUnorderedSetTest, empty)
{
    EXPECT_FALSE(uset.empty());
}

TEST_F(WWUnorderedSetTest, size)
{
    EXPECT_EQ(uset.size(), 4);
}

// 查找

TEST_F(WWUnorderedSetTest, count)
{
    EXPECT_EQ(uset.count("1"), 1);
    EXPECT_EQ(uset.count("2"), 1);
    EXPECT_EQ(uset.count("3"), 1);
    EXPECT_EQ(uset.count("4"), 1);
    EXPECT_EQ(uset.count("5"), 0);
}

TEST_F(WWUnorderedSetTest, find)
{
    EXPECT_EQ(*uset.find("1"), "1");
    EXPECT_EQ(*uset.find("2"), "2");
    EXPECT_EQ(*uset.find("3"), "3");
    EXPECT_EQ(*uset.find("4"), "4");
    EXPECT_EQ(uset.find("5"), uset.end());
}

TEST_F(WWUnorderedSetTest, equal_range)
{
    auto range = uset.equal_range("1");
    EXPECT_EQ(*range.first, "1");
    EXPECT_EQ(*range.second, "2");
}

// 桶接口

TEST_F(WWUnorderedSetTest, bucket_begin)
{
    EXPECT_EQ(*uset.begin(1), "1");
}

TEST_F(WWUnorderedSetTest, bucket_count)
{
    EXPECT_EQ(uset.bucket_count(), 8);
}

TEST_F(WWUnorderedSetTest, bucket_size)
{
    EXPECT_EQ(uset.bucket_size(1), 1);
}

TEST_F(WWUnorderedSetTest, bucket)
{
    EXPECT_EQ(uset.bucket("1"), 1);
    EXPECT_EQ(uset.bucket("2"), 2);
    EXPECT_EQ(uset.bucket("3"), 3);
}

// 散列策略

TEST_F(WWUnorderedSetTest, load_factor)
{
    EXPECT_EQ(uset.load_factor(), 0.5);
}

TEST_F(WWUnorderedSetTest, max_load_factor)
{
    EXPECT_EQ(uset.max_load_factor(), 1.0);

    uset.max_load_factor(2.0);
    EXPECT_EQ(uset.max_load_factor(), 2.0);
}

TEST_F(WWUnorderedSetTest, rehash)
{
    uset.rehash(16);
    EXPECT_EQ(uset.bucket_count(), 16);
    EXPECT_EQ(uset.load_factor(), 0.25);
}

TEST_F(WWUnorderedSetTest, reserve)
{
    // 就该测试用例而言，和rehash的结果是一样的，但语义不同
    uset.reserve(16);
    EXPECT_EQ(uset.bucket_count(), 16);
    EXPECT_EQ(uset.load_factor(), 0.25);
}