#include <gtest/gtest.h>
#include <vector>
#include <ww_memory.h>
#include <memory>

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

/**
 * @brief 自定义最小分配器
 * @details 只包含C++规定的最少的具名要求
 * @link https://zh.cppreference.com/w/cpp/named_req/Allocator
 */
template <class T>
class TestAllocator
{
public:
    using value_type = T;

public:
    TestAllocator() noexcept = default;

    TestAllocator(const TestAllocator & other) noexcept = default;

    template <typename U>
    TestAllocator(const TestAllocator<U> & other) noexcept 
    {
    };

    ~TestAllocator() = default;

public:
    value_type * allocate(std::size_t n, const void * hint = nullptr)
    {
        (void)hint;
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(value_type))   // 超出最大尺寸
            throw std::bad_array_new_length();

        if (n == 0)
            return nullptr;

        return static_cast<value_type *>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(value_type * ptr, std::size_t n = 0)
    {
        (void)n;
        if (ptr == nullptr)
            return;
        
        ::operator delete(ptr);
    }

    // comaparison operators ignored here, which will not be tested
};

// 测试 allocator_traits
TEST_F(WwMemoryTest, AllocatorTraits)
{
    // 测试嵌套类型
    using T = int;
    using Alloc = TestAllocator<T>;
    using Traits = allocator_traits<Alloc>;
    using size_type = typename Traits::size_type;

    // 基本类型检测
    EXPECT_TRUE((std::is_same<typename Traits::value_type, T>::value));
    EXPECT_TRUE((std::is_same<typename Traits::pointer, T*>::value));
    EXPECT_TRUE((std::is_same<typename Traits::const_pointer, const T*>::value));
    EXPECT_TRUE((std::is_same<typename Traits::void_pointer, void*>::value));
    EXPECT_TRUE((std::is_same<typename Traits::const_void_pointer, const void*>::value));

    // 差值类型和大小类型检测
    EXPECT_TRUE((std::is_same<typename Traits::difference_type, std::ptrdiff_t>::value));
    EXPECT_TRUE((std::is_same<typename Traits::size_type, std::size_t>::value));

    // 分配器属性检测
    EXPECT_TRUE((std::is_same<typename Traits::propagate_on_container_copy_assignment, std::false_type>::value));
    EXPECT_TRUE((std::is_same<typename Traits::propagate_on_container_move_assignment, std::false_type>::value));
    EXPECT_TRUE((std::is_same<typename Traits::propagate_on_container_swap, std::false_type>::value));
    EXPECT_TRUE((std::is_same<typename Traits::is_always_equal, std::true_type>::value));

    Alloc alloc;
    T * p = nullptr;

    // allocate
    p = Traits::allocate(alloc, 10);
    EXPECT_NE(p, nullptr);

    // construct
    Traits::construct(alloc, p, 666);
    EXPECT_EQ(*p, 666);

    // destroy
    Traits::destroy(alloc, p);

    // deallocate
    Traits::deallocate(alloc, p, 10);

    // max_size
    size_type m = Traits::max_size(alloc);

    // select
    Alloc tmp = Traits::select_on_container_copy_construction(alloc);

    // allocate_hint
    typename Traits::const_void_pointer hint;
    p = Traits::allocate(alloc, 7, hint);
    EXPECT_NE(p, nullptr);

    Traits::deallocate(alloc, p, 7);
}