#include <gtest/gtest.h>
#include <ww_unordered_map.h>

using namespace wwstl;

class WWUnorderedMapTest : public testing::Test
{
public:
    unordered_map<int, std::string> umap;

protected:
    void SetUp() override
    {
        umap.insert({1, "1"});
        umap.insert({2, "2"});
        umap.insert({3, "3"});
        umap.insert({4, "4"});
    }
};

// 容量

TEST_F(WWUnorderedMapTest, empty)
{
    EXPECT_FALSE(umap.empty());
}

TEST_F(WWUnorderedMapTest, size)
{
    EXPECT_EQ(umap.size(), 4);
}

// 修改器

TEST_F(WWUnorderedMapTest, clear)
{
    umap.clear();
    EXPECT_TRUE(umap.empty());
    EXPECT_EQ(umap.size(), 0);
}

TEST_F(WWUnorderedMapTest, insert)
{
    auto it = umap.insert(std::pair<const int, std::string>(5, "5"));
    EXPECT_TRUE(it.second);
    EXPECT_EQ(it.first->first, 5);
    EXPECT_EQ(it.first->second, "5");
    it = umap.insert({5, "7"});
    EXPECT_FALSE(it.second);

    umap.insert({6, "6"});
}

TEST_F(WWUnorderedMapTest, erase)
{
    umap.insert({
        {5, "5"},
        {6, "6"},
        {7, "7"},
        {8, "8"},
    });

    umap.erase(umap.begin());

    auto it = umap.begin();
    std::advance(it, 3);
    umap.erase(umap.begin(), it);

    int count = umap.erase(7);
    EXPECT_EQ(count, 1);
}

// 查找

TEST_F(WWUnorderedMapTest, at)
{
    EXPECT_EQ(umap.at(1), "1");
    EXPECT_EQ(umap.at(2), "2");
    EXPECT_EQ(umap.at(3), "3");
    EXPECT_EQ(umap.at(4), "4");
    EXPECT_THROW(umap.at(5), std::out_of_range);
}

TEST_F(WWUnorderedMapTest, operator_access)
{
    EXPECT_EQ(umap[1], "1");
    EXPECT_EQ(umap[2], "2");
    EXPECT_EQ(umap[3], "3");
    EXPECT_EQ(umap[4], "4");

    umap[5] = "5";
    EXPECT_EQ(umap[5], "5");
}

TEST_F(WWUnorderedMapTest, count)
{
    EXPECT_EQ(umap.count(1), 1);
    EXPECT_EQ(umap.count(2), 1);
    EXPECT_EQ(umap.count(3), 1);
    EXPECT_EQ(umap.count(4), 1);
    EXPECT_EQ(umap.count(5), 0);
}

TEST_F(WWUnorderedMapTest, find)
{
    auto it = umap.find(1);
    EXPECT_EQ(it->second, "1");
    it = umap.find(2);
    EXPECT_EQ(it->second, "2");
    it = umap.find(3);
    EXPECT_EQ(it->second, "3");
    it = umap.find(4);
    EXPECT_EQ(it->second, "4");
    it = umap.find(5);
    EXPECT_EQ(it, umap.end());
}

TEST_F(WWUnorderedMapTest, equal_range)
{
    auto it = umap.equal_range(1);
    EXPECT_EQ(it.first->second, "1");
    it = umap.equal_range(2);
    EXPECT_EQ(it.first->second, "2");
    it = umap.equal_range(3);
    EXPECT_EQ(it.first->second, "3");
    it = umap.equal_range(4);
    EXPECT_EQ(it.first->second, "4");
    it = umap.equal_range(5);
    EXPECT_EQ(it.first, umap.end());
}

// 桶接口

TEST_F(WWUnorderedMapTest, bucket_begin)
{
    EXPECT_EQ(umap.begin(1)->second, "1");
}

TEST_F(WWUnorderedMapTest, bucket_count)
{
    EXPECT_EQ(umap.bucket_count(), 8);
}

TEST_F(WWUnorderedMapTest, bucket_size)
{
    EXPECT_EQ(umap.bucket_size(1), 1);
}

TEST_F(WWUnorderedMapTest, bucket)
{
    EXPECT_EQ(umap.bucket(1), 1);
    EXPECT_EQ(umap.bucket(2), 2);
    EXPECT_EQ(umap.bucket(3), 3);
}

// 散列策略

TEST_F(WWUnorderedMapTest, load_factor)
{
    EXPECT_EQ(umap.load_factor(), 0.5);
}

TEST_F(WWUnorderedMapTest, max_load_factor)
{
    EXPECT_EQ(umap.max_load_factor(), 1.0);

    umap.max_load_factor(2.0);
    EXPECT_EQ(umap.max_load_factor(), 2.0);
}

TEST_F(WWUnorderedMapTest, rehash)
{
    umap.rehash(16);
    EXPECT_EQ(umap.bucket_count(), 16);
    EXPECT_EQ(umap.load_factor(), 0.25);
}

TEST_F(WWUnorderedMapTest, reserve)
{
    // 就该测试用例而言，和rehash的结果是一样的，但语义不同
    umap.reserve(16);
    EXPECT_EQ(umap.bucket_count(), 16);
    EXPECT_EQ(umap.load_factor(), 0.25);
}