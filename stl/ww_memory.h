#ifndef __WW_MEMORY_H__
#define __WW_MEMORY_H__

#include <limits>
#include <stdexcept>

namespace wwstl
{

/**
 * @brief 分配器
 * @link https://zh.cppreference.com/w/cpp/memory/allocator
 */
template <class T>
class allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <typename U>
    class rebind
    {
    public:
        using other = allocator<U>;
    };

public:
    allocator() noexcept = default;

    allocator(const allocator & other) noexcept = default;
    
    template <typename U>
    allocator(const allocator<U> & other) noexcept 
    { // template cannot be default
    };

    ~allocator() = default;

public:
    /**
     * @brief 分配n个元素的内存
     * @param n 元素个数
     * @param hint 会在hint附近分配内存，忽略
     * @return pointer 内存指针
     * @exception std::bad_array_new_length 超出最大尺寸
     * @exception std::bad_alloc 内存分配失败
     */
    pointer allocate(size_type n, const void * hint = nullptr)
    {
        (void)hint;
        if (n > max_size())   // 超出最大尺寸
            throw std::bad_array_new_length();

        if (n == 0)
            return nullptr;

        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    /**
     * @brief 释放由allocate分配的内存
     * @param ptr 要释放的内存指针
     * @param n 元素个数，忽略
     */
    void deallocate(pointer ptr, size_type n = 0)
    {
        (void)n;
        if (ptr == nullptr)
            return;
        
        ::operator delete(ptr);
    }

    /**
     * @brief 构造对象
     * @param ptr 要构造的内存指针
     * @param args 构造函数参数包
     */
    template <typename U, typename... Args>
    void construct(U * ptr, Args&&... args)
    {
        ::new(ptr) U(std::forward<Args>(args)...);
    }

    /**
     * @brief 销毁对象
     * @param ptr 要销毁的内存指针
     */
    template <typename U>
    void destroy(U * ptr)
    {
        ptr->~U();
    }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(value_type);
    }

    pointer address(reference r) const noexcept
    {
        return &r;
    }

    const_pointer address(const_reference r) const noexcept
    {
        return &r;
    }

    bool operator==(const allocator & other) const noexcept
    {
        return true;
    }

    bool operator!=(const allocator & other) const noexcept
    {
        return false;
    }
};

/**
 * @brief void类型特化
 * @details 为void类型的分配器提供类型支持，但不提供实现
 */
template <>
class allocator<void>
{
public:
    using value_type = void;
    using pointer = void*;
    using const_pointer = const void*;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <typename U>
    class rebind
    {
    public:
        using other = allocator<U>;
    };
};

} // namespace wwstl

#endif // __WW_MEMORY_H__