#include <gtest/gtest.h>
#include <ww_unordered_map.h>

using namespace wwstl;

class WWUnorderedMultiMapTest : public testing::Test
{
public:
    unordered_multimap<int, std::string> ummap;

protected:
    void SetUp() override
    {
        ummap.insert({1, "1"});
        ummap.insert({2, "2"});
        ummap.insert({3, "3"});
        ummap.insert({4, "4"});
        ummap.insert({1, "11"});
        ummap.insert({2, "22"});
        ummap.insert({2, "222"});
    }
};

// 容量

TEST_F(WWUnorderedMultiMapTest, empty)
{
    EXPECT_FALSE(ummap.empty());
}

TEST_F(WWUnorderedMultiMapTest, size)
{
    EXPECT_EQ(ummap.size(), 7);
}

// 修改器

TEST_F(WWUnorderedMultiMapTest, insert)
{
    ummap.insert(std::pair<const int, std::string>(5, "5"));
    ummap.insert({
        {5, "55"},
        {5, "555"},
    });
    EXPECT_EQ(ummap.count(5), 3);
}

TEST_F(WWUnorderedMultiMapTest, erase)
{
    ummap.erase(2);
    EXPECT_EQ(ummap.count(2), 0);

    ummap.erase(ummap.begin());
    EXPECT_EQ(ummap.count(1), 1);
    EXPECT_EQ(ummap.begin()->second, "11");

    ummap.erase(ummap.begin(), ummap.end());
    EXPECT_TRUE(ummap.empty());
}

// 查找

TEST_F(WWUnorderedMultiMapTest, count)
{
    EXPECT_EQ(ummap.count(1), 2);
    EXPECT_EQ(ummap.count(2), 3);
    EXPECT_EQ(ummap.count(3), 1);
}

TEST_F(WWUnorderedMultiMapTest, find)
{
    // 在本实现中，第一次插入的元素永远会先被找到
    EXPECT_EQ(ummap.find(1)->second, "1");
    EXPECT_EQ(ummap.find(2)->second, "2");
}

TEST_F(WWUnorderedMultiMapTest, equal_range)
{
    auto range = ummap.equal_range(1);
    for (auto it = range.first; it != range.second; ++it) {
        EXPECT_EQ(it->first, 1);
    }
    range = ummap.equal_range(2);
    for (auto it = range.first; it != range.second; ++it) {
        EXPECT_EQ(it->first, 2);
    }
}

// 桶接口

TEST_F(WWUnorderedMultiMapTest, bucket_begin)
{
    EXPECT_EQ(ummap.begin(1)->second, "1");
}

TEST_F(WWUnorderedMultiMapTest, bucket_count)
{
    EXPECT_EQ(ummap.bucket_count(), 8);
}

TEST_F(WWUnorderedMultiMapTest, bucket_size)
{
    EXPECT_EQ(ummap.bucket_size(1), 2);
}

TEST_F(WWUnorderedMultiMapTest, bucket)
{
    EXPECT_EQ(ummap.bucket(1), 1);
    EXPECT_EQ(ummap.bucket(2), 2);
    EXPECT_EQ(ummap.bucket(3), 3);
}

// 散列策略

TEST_F(WWUnorderedMultiMapTest, load_factor)
{
    EXPECT_EQ(ummap.load_factor(), 0.875);
}

TEST_F(WWUnorderedMultiMapTest, max_load_factor)
{
    EXPECT_EQ(ummap.max_load_factor(), 1.0);

    ummap.max_load_factor(2.0);
    EXPECT_EQ(ummap.max_load_factor(), 2.0);
}

TEST_F(WWUnorderedMultiMapTest, rehash)
{
    ummap.rehash(16);
    EXPECT_EQ(ummap.bucket_count(), 16);
    EXPECT_EQ(ummap.load_factor(), 0.4375);
}

TEST_F(WWUnorderedMultiMapTest, reserve)
{
    ummap.reserve(16);
    EXPECT_EQ(ummap.bucket_count(), 16);
    EXPECT_EQ(ummap.load_factor(), 0.4375);
}

// 比较

TEST_F(WWUnorderedMultiMapTest, compare)
{
    unordered_multimap<int, std::string> ummap2 = {
        {1, "1"},
        {2, "2"},
        {3, "3"},
        {4, "4"},
        {1, "11"},
        {2, "22"},
        {2, "222"},
    };
    EXPECT_TRUE(ummap == ummap2);

    unordered_multimap<int, std::string> ummap3 = {
        {1, "1"},
        {2, "2"},
        {3, "3"},
        {1, "11"},
        {2, "22"},
    };
    EXPECT_FALSE(ummap == ummap3);

    unordered_multimap<int, std::string> ummap4 = {
        {1, "1"},
        {2, "2"},
        {3, "666"},
        {4, "4"},
        {1, "11"},
        {2, "22"},
        {2, "666"},
    };
    EXPECT_FALSE(ummap == ummap4);

    unordered_multimap<int, std::string> ummap5 = {
        {1, "1"},
        {3, "3"},
        {2, "2"},
        {1, "11"},
        {2, "22"},
        {2, "222"},
        {4, "4"}
    };
    EXPECT_TRUE(ummap == ummap5);
}