#include <gtest/gtest.h>
#include <vector>
#include <ww_memory.h>

using namespace wwstl;

class WwMemoryTest : public testing::Test
{
public:
    using int_allocator_type = allocator<int>;

    int_allocator_type alloc;   // int类型分配器
};

// 测试allocate和deallocate
TEST_F(WwMemoryTest, AllocateAndDeallocate)
{
    int * p = alloc.allocate(10);
    EXPECT_NE(p, nullptr);

    alloc.deallocate(p, 10);
}

// 测试construct和destroy
TEST_F(WwMemoryTest, ConstructAndDestroy)
{
    int * p = alloc.allocate(10);

    for (int i = 0; i < 10; ++i) {
        alloc.construct(p + i, i);
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(p[i], i);
    }

    for (int i = 0; i < 10; ++i) {
        alloc.destroy(p + i);
    }

    alloc.deallocate(p, 10);
}

// 测试rebind
TEST_F(WwMemoryTest, Rebind)
{
    using char_allocator_type = typename int_allocator_type:: template rebind<char>::other;
    char_allocator_type char_alloc;

    char * p = char_alloc.allocate(10);

    for (int i = 0; i < 10; ++i) {
        char_alloc.construct(p + i, 'a' + i);
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(p[i], 'a' + i);
    }

    for (int i = 0; i < 10; ++i) {
        char_alloc.destroy(p + i);
    }

    char_alloc.deallocate(p, 10);
}

// 测试exception
TEST_F(WwMemoryTest, Exception)
{
    EXPECT_THROW(alloc.allocate(alloc.max_size() + 1), std::bad_array_new_length);
}

// 传入标准容器
TEST_F(WwMemoryTest, Vector)
{
    std::vector<int, int_allocator_type> vec(alloc);

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vec[i], i);
    }

    std::vector<int, int_allocator_type>().swap(vec);
}