#include <gtest/gtest.h>
#include <ww_unordered_set.h>

using namespace wwstl;

class WWUnorderedMultiSetTest : public testing::Test
{
public:
    unordered_multiset<std::string> umset;

protected:
    void SetUp() override
    {
        umset.insert("1");
        umset.insert("2");
        umset.insert("3");
        umset.insert("4");
        umset.insert("1");
        umset.insert("2");
        umset.insert("2");
    }
};

// 修改器

TEST_F(WWUnorderedMultiSetTest, clear)
{
    umset.clear();
    EXPECT_TRUE(umset.empty());
    EXPECT_EQ(umset.size(), 0);
}

TEST_F(WWUnorderedMultiSetTest, insert)
{
    umset.insert({"5", "5", "6"});
    EXPECT_EQ(umset.count("5"), 2);
}

TEST_F(WWUnorderedMultiSetTest, erase)
{
    umset.erase(umset.find("2"));
    EXPECT_EQ(umset.count("2"), 2);

    umset.erase("1");
    EXPECT_EQ(umset.count("1"), 0);

    umset.erase(umset.begin(), umset.end());
    EXPECT_TRUE(umset.empty());
}

// 容量

TEST_F(WWUnorderedMultiSetTest, empty)
{
    EXPECT_FALSE(umset.empty());
}

TEST_F(WWUnorderedMultiSetTest, size)
{
    EXPECT_EQ(umset.size(), 7);
}

// 查找

TEST_F(WWUnorderedMultiSetTest, count)
{
    EXPECT_EQ(umset.count("1"), 2);
    EXPECT_EQ(umset.count("2"), 3);
    EXPECT_EQ(umset.count("3"), 1);
    EXPECT_EQ(umset.count("4"), 1);
    EXPECT_EQ(umset.count("5"), 0);
}

TEST_F(WWUnorderedMultiSetTest, find)
{
    EXPECT_EQ(*umset.find("1"), "1");
    EXPECT_EQ(*umset.find("2"), "2");
    EXPECT_EQ(*umset.find("3"), "3");
    EXPECT_EQ(*umset.find("4"), "4");
    EXPECT_EQ(umset.find("5"), umset.end());
}

TEST_F(WWUnorderedMultiSetTest, equal_range)
{
    auto range = umset.equal_range("2");
    for (auto it = range.first; it != range.second; ++it) {
        EXPECT_EQ(*it, "2");
    }
}

// 桶接口

TEST_F(WWUnorderedMultiSetTest, bucket_begin)
{
    EXPECT_EQ(*umset.begin(1), "1");
}

TEST_F(WWUnorderedMultiSetTest, bucket_count)
{
    EXPECT_EQ(umset.bucket_count(), 8);
}

TEST_F(WWUnorderedMultiSetTest, bucket_size)
{
    EXPECT_EQ(umset.bucket_size(1), 2);
}

TEST_F(WWUnorderedMultiSetTest, bucket)
{
    EXPECT_EQ(umset.bucket("1"), 1);
    EXPECT_EQ(umset.bucket("2"), 2);
    EXPECT_EQ(umset.bucket("3"), 3);
}

// 散列策略

TEST_F(WWUnorderedMultiSetTest, load_factor)
{
    EXPECT_EQ(umset.load_factor(), 0.875);
}

TEST_F(WWUnorderedMultiSetTest, max_load_factor)
{
    EXPECT_EQ(umset.max_load_factor(), 1.0);

    umset.max_load_factor(2.0);
    EXPECT_EQ(umset.max_load_factor(), 2.0);
}

TEST_F(WWUnorderedMultiSetTest, rehash)
{
    umset.rehash(16);
    EXPECT_EQ(umset.bucket_count(), 16);
    EXPECT_EQ(umset.load_factor(), 0.4375);
}

TEST_F(WWUnorderedMultiSetTest, reserve)
{
    umset.reserve(16);
    EXPECT_EQ(umset.bucket_count(), 16);
    EXPECT_EQ(umset.load_factor(), 0.4375);
}