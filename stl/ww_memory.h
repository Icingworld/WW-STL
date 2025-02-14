#ifndef __WW_MEMORY_H__
#define __WW_MEMORY_H__

#include <limits>
#include <stdexcept>
#include "ww_type_traits.h"

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

/**
 * @brief _replace_first_parameter
 * @details 用于替换第一个模板参数，生成新的模板类
 * @param T 原模板类
 * @param NewFirst 新的模板参数
 */
template <
    class NewFirst,
    class T
> class _replace_first_parameter
{
};

template <
    class NewFirst,
    template <class, class...>
    class T,
    class RawFirst,
    class... Res
> class _replace_first_parameter<NewFirst, T<RawFirst, Res...>>
{
public:
    using type = T<NewFirst, Res...>;
};

template <
    class Alloc,
    class = void
> class _get_pointer_type
{
public:
    using type = typename Alloc::value_type*;
};

template <class Alloc>
class _get_pointer_type<Alloc, wwstl::void_t<typename Alloc::pointer>>
{
public:
    using type = typename Alloc::pointer;
};

template <
    class Alloc,
    class = void
> class _get_const_pointer_type
{
public:
    using _value_type = typename Alloc::value_type;
    using _pointer_type = typename _get_pointer_type<Alloc>::type;
    using type = std::pointer_traits<_pointer_type>::template rebind<const _value_type>;
};

template <class Alloc>
class _get_const_pointer_type<Alloc, wwstl::void_t<typename Alloc::const_pointer>>
{
public:
    using type = typename Alloc::const_pointer;
};

template <
    class Alloc,
    class = void
> class _get_void_pointer_type
{
public:
    using _pointer_type = typename _get_pointer_type<Alloc>::type;
    using type = std::pointer_traits<_pointer_type>::template rebind<void>;
};

template <class Alloc>
class _get_void_pointer_type<Alloc, wwstl::void_t<typename Alloc::void_pointer>>
{
public:
    using type = typename Alloc::void_pointer;
};

template <
    class Alloc,
    class = void
> class _get_const_void_pointer_type
{
public:
    using _pointer_type = typename _get_pointer_type<Alloc>::type;
    using type = std::pointer_traits<_pointer_type>::template rebind<const void>;
};

template <class Alloc>
class _get_const_void_pointer_type<Alloc, wwstl::void_t<typename Alloc::const_void_pointer>>
{
public:
    using type = typename Alloc::const_void_pointer;
};

template <
    class Alloc,
    class = void
> class _get_difference_type
{
public:
    using _pointer_type = typename _get_pointer_type<Alloc>::type;
    using type = std::pointer_traits<_pointer_type>::difference_type;
};

template <class Alloc>
class _get_difference_type<Alloc, wwstl::void_t<typename Alloc::difference_type>>
{
public:
    using type = typename Alloc::difference_type;
};

template <
    class Alloc,
    class = void
> class _get_size_type
{
public:
    using _difference_type = typename _get_difference_type<Alloc>::type;
    using type = typename std::make_unsigned<_difference_type>::type;
};

template <class Alloc>
class _get_size_type<Alloc, wwstl::void_t<typename Alloc::size_type>>
{
public:
    using type = typename Alloc::size_type;
};

template <
    class Alloc,
    class = void
> class _get_propagate_on_container_copy_assignment_type
{
public:
    using type = std::false_type;
};

template <class Alloc>
class _get_propagate_on_container_copy_assignment_type<Alloc, wwstl::void_t<typename Alloc::propagate_on_container_copy_assignment>>
{
public:
    using type = typename Alloc::propagate_on_container_copy_assignment;
};

template <
    class Alloc,
    class = void
> class _get_propagate_on_container_move_assignment_type
{
public:
    using type = std::false_type;
};

template <class Alloc>
class _get_propagate_on_container_move_assignment_type<Alloc, wwstl::void_t<typename Alloc::propagate_on_container_move_assignment>>
{
public:
    using type = typename Alloc::propagate_on_container_move_assignment;
};

template <
    class Alloc,
    class = void
> class _get_propagate_on_container_swap_type
{
public:
    using type = std::false_type;
};

template <class Alloc>
class _get_propagate_on_container_swap_type<Alloc, wwstl::void_t<typename Alloc::propagate_on_container_swap>>
{
public:
    using type = typename Alloc::propagate_on_container_swap;
};

template <
    class Alloc,
    class = void
> class _get_is_always_equal_type
{
public:
    using type = typename std::is_empty<Alloc>::type;
};

template <class Alloc>
class _get_is_always_equal_type<Alloc, wwstl::void_t<typename Alloc::is_always_equal>>
{
public:
    using type = typename Alloc::is_always_equal;
};

template <
    class Alloc,
    class T,
    class = void
> class _get_rebind_alloc_type
{
public:
    using type = typename 
};

template <class Alloc, class T>
class _get_rebind_alloc_type<Alloc, T, wwstl::void_t<typename Alloc::template rebind<T>::other>>
{
public:
    using type = typename Alloc::template rebind<T>::other;
};

template <class Alloc>
class allocator_traits
{
public:
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename _get_pointer_type<Alloc>::type;
    using const_pointer = typename _get_const_pointer_type<Alloc>::type;
    using void_pointer = typename _get_void_pointer_type<Alloc>::type;
    using const_void_pointer = typename _get_const_void_pointer_type<Alloc>::type;
    using difference_type = typename _get_difference_type<Alloc>::type;
    using size_type = typename _get_size_type<Alloc>::type;
    using propagate_on_container_copy_assignment = typename _get_propagate_on_container_copy_assignment_type<Alloc>::type;
    using propagate_on_container_move_assignment = typename _get_propagate_on_container_move_assignment_type<Alloc>::type;
    using propagate_on_container_swap = typename _get_propagate_on_container_swap_type<Alloc>::type;
    using is_always_equal = typename _get_is_always_equal_type<Alloc>::type;

    template <class T>
    using rebind_alloc = typename _get_rebind_alloc_type<Alloc, T>::type;

    template <class T>
    using rebind_traits = allocator_traits<rebind_alloc<T>>;

public:
    /**
     * @brief 用分配器分配未初始化的存储
     */
    static pointer allocate(Alloc & a, size_type n)
    {
        return a.allocate(n);
    };

    /**
     * @brief 用分配器分配未初始化的存储
     */
    static pointer allocate(Alloc & a, size_type n, const_void_pointer hint)
    {
        return a.allocate(n, hint);
    };

    /**
     * @brief 用分配器解分配存储
     */
    static void deallocate(Alloc & a, pointer p, size_type n)
    {
        a.deallocate(p, n);
    }

    /**
     * @brief 在已分配存储中构造对象
     */
    template <
        class T,
        class... Args
    > static void construct(Alloc & a, T * p, Args&&... args)
    {
        a.construct(p, std::forward<Args>(args)...);
    }

    /**
     * @brief 析构储存于已分配存储中的对象
     */
    template <class T>
    static void destroy(Alloc & a, T * p)
    {
        a.destroy(p);
    }

    /**
     * @brief 返回分配器所支持的最大对象大小
     */
    static size_type max_size(const Alloc & a) noexcept
    {
        return a.max_size();
    }

    /**
     * @brief 获得复制标准容器后使用的分配器
     */
    static Alloc select_on_container_copy_construction(const Alloc & a)
    {
        return a.select_on_container_copy_construction();
    }
};

} // namespace wwstl

#endif // __WW_MEMORY_H__