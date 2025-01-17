#ifndef __WW_ARRAY_H__
#define __WW_ARRAY_H__

#include <stdexcept>
#include "ww_iterator.h"

namespace wwstl
{

/**
 * @brief array常量迭代器类
 */
template <class T>
class _array_const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using self = _array_const_iterator<value_type>;

    using _Ptr = value_type*;   // 底层使用非const指针

public:
    _Ptr _ptr;  // 指向array的指针

public:
    _array_const_iterator()
        : _ptr(nullptr)
    { // 构造一个空迭代器
    }

    explicit _array_const_iterator(pointer ptr)
        : _ptr(const_cast<_Ptr>(ptr))
    { // 用指针ptr构造一个迭代器
    }

public:
    reference operator*() const
    { return *_ptr; }

    pointer operator->() const
    { return &(operator*()); }

    self & operator++()
    {
        ++_ptr;
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self & operator--()
    {
        --_ptr;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self & other) const
    { return _ptr == other._ptr; }

    bool operator!=(const self & other) const
    { return !(*this == other); }

    bool operator<(const self & other) const
    { return _ptr < other._ptr; }

    bool operator>(const self & other) const
    { return other < *this; }

    bool operator<=(const self & other) const
    { return !(other < *this); }

    bool operator>=(const self & other) const
    { return !(*this < other); }

    self & operator+=(const difference_type n)
    {
        _ptr += n;
        return *this;
    }

    self & operator-=(const difference_type n)
    { return *this += -n; }

    self operator+(const difference_type n) const
    {
        self temp = *this;
        return temp += n;
    }

    self operator-(const difference_type n) const
    {
        self temp = *this;
        return temp -= n;
    }

    difference_type operator-(const self & other) const
    { return _ptr - other._ptr; }

    reference operator[](const difference_type n) const
    { return *(*this + n); }
};

/**
 * @brief array非常量迭代器类
 */
template <class T>
class _array_iterator
    : public _array_const_iterator<T>
{
public:
    using base = _array_const_iterator<T>;      // 基类
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using self = _array_iterator<value_type>;

public:
    _array_iterator()
        : base()
    { // 构造一个空迭代器
    }

    explicit _array_iterator(pointer ptr)
        : base(ptr)
    { // 用指针ptr构造一个迭代器
    }

public:
    reference operator*() const
    { return const_cast<reference>(base::operator*()); }

    pointer operator->() const
    { return const_cast<pointer>(base::operator->()); }

    self & operator++()
    {
        ++*(base *)this;
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self & operator--()
    {
        --*(base *)this;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    self & operator+=(difference_type n)
    {
        *(base *)this += n;
        return *this;
    }

    self & operator-=(difference_type n)
    {
        *(base *)this -= n;
        return *this;
    }

    self operator+(difference_type n) const
    {
        self temp = *this;
        return temp += n;
    }

    self operator-(difference_type n) const
    {
        self temp = *this;
        return temp -= n;
    }

    difference_type operator-(const base & other) const
    { return *(base *)this - other; }

    reference operator[](difference_type n) const
    { return *(*this + n); }
};

/**
 * @brief array
 * @link https://zh.cppreference.com/w/cpp/container/array
 */
template <
    class T,
    std::size_t N
> class array
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = _array_iterator<value_type>;
    using const_iterator = _array_const_iterator<value_type>;
    using reverse_iterator = mystl::reverse_iterator<iterator>;
    using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

public:
    value_type _data[N];

public:
    // 元素访问

    /**
     * @brief 带越界检查访问指定的元素
     */
    reference at(size_type pos)
    {
        if (pos >= N) {
            _throw_out_of_range();
        }
        return _data[pos];
    }

    /**
     * @brief 带越界检查访问指定的元素
     */
    const_reference at(size_type pos) const
    {
        if (pos >= N) {
            _throw_out_of_range();
        }
        return _data[pos];
    }

    /**
     * @brief 访问指定的元素
     */
    reference operator[](size_type pos)
    { return _data[pos]; }

    /**
     * @brief 访问指定的元素
     */
    const_reference operator[](size_type pos) const
    { return _data[pos]; }

    /**
     * @brief 访问第一个元素
     */
    reference front()
    { return _data[0]; }

    /**
     * @brief 访问第一个元素
     */
    const_reference front() const
    { return _data[0]; }

    /**
     * @brief 访问最后一个元素
     */
    reference back()
    { return _data[N - 1]; }

    /**
     * @brief 访问最后一个元素
     */
    const_reference back() const
    { return _data[N - 1]; }

    /**
     * @brief 直接访问底层连续存储
     */
    pointer data()
    { return _data; }

    /**
     * @brief 直接访问底层连续存储
     */
    const_pointer data() const
    { return _data; }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    { return iterator(_data); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    { return const_iterator(_data); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    { return begin(); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    { return iterator(_data + N); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    { return const_iterator(_data + N); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    { return end(); }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator crbegin() const noexcept
    { return rbegin(); }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator crend() const noexcept
    { return rend(); }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    constexpr bool empty() const noexcept
    { return false; }

    /**
     * @brief 返回元素数
     */
    constexpr size_type size() const noexcept
    { return N; }

    /**
     * @brief 返回最大容量
     */
    constexpr size_type max_size() const noexcept
    { return size(); }

    // 操作

    /**
     * @brief 以指定值填充容器
     */
    void fill(const value_type & value)
    { std::fill_n(_data, N, value); }

    /**
     * @brief 交换内容
     */
    void swap(array<T, N> & other) noexcept
    { std::swap(_data, other._data); }

public:
    /**
     * @brief 抛出out_of_range异常
     */
    [[noreturn]] void _throw_out_of_range() const
    { throw std::out_of_range("invalid array<T, N> subscript"); }
};

// 非成员函数

template <
    class T,
    std::size_t N
> bool operator==(const array<T, N> & a, const array<T, N> & b)
{ return std::equal(a.begin(), a.end(), b.begin(), b.end()); }

template <
    class T,
    std::size_t N
> bool operator!=(const array<T, N> & a, const array<T, N> & b)
{ return !(a == b); }

template <
    class T,
    std::size_t N
> bool operator<(const array<T, N> & a, const array<T, N> & b)
{ return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end()); }

template <
    class T,
    std::size_t N
> bool operator<=(const array<T, N> & a, const array<T, N> & b)
{ return !(b < a); }

template <
    class T,
    std::size_t N
> bool operator>(const array<T, N> & a, const array<T, N> & b)
{ return b < a; }

template <
    class T,
    std::size_t N
> bool operator>=(const array<T, N> & a, const array<T, N> & b)
{ return !(a < b); }

/**
 * @brief 交换两个array
 */
template <
    class T,
    std::size_t N
> void swap(array<T, N> & a, array<T, N> & b)
{ a.swap(b); }

/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> T & get(array<T, N> & a)
{ return a[I]; }

/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> T && get(array<T, N> && a)
{ return std::move(a[I]); }

/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> const T & get(const array<T, N> & a)
{ return a[I]; }

/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> const T && get(const array<T, N> && a)
{ return std::move(a[I]); }

// 辅助类

template <
    std::size_t I,
    class T
> class tuple_element;

template <
    std::size_t I,
    class T,
    std::size_t N
> class tuple_element<I, array<T, N>>
{
public:
    using type = T;
};

template <class T>
class tuple_size;

template<
    class T,
    std::size_t N
> class tuple_size<array<T, N>> : std::integral_constant<std::size_t, N>
{
};

/**
 * @brief 对空array的特化
 */
template <class T>
class array<T, 0>
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = _array_iterator<value_type>;
    using const_iterator = _array_const_iterator<value_type>;
    using reverse_iterator = mystl::reverse_iterator<iterator>;
    using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

public:
    value_type _data[1];        // 不允许声明一个大小为0的数组

public:
    // 元素访问

    [[noreturn]] reference at(size_type)
    { _throw_out_of_range(); }

    [[noreturn]] const_reference at(size_type) const
    { _throw_out_of_range(); }

    reference operator[](size_type) noexcept
    { return _data[0]; }

    const_reference operator[](size_type) const noexcept
    { return _data[0]; }

    reference front()
    { return _data[0]; }

    const_reference front() const
    { return _data[0]; }

    reference back()
    { return _data[0]; }

    const_reference back() const
    { return _data[0]; }

    pointer data()
    { return nullptr; }

    const_pointer data() const
    { return nullptr; }

    // 迭代器

    iterator begin() noexcept
    { return iterator(); }

    const_iterator begin() const noexcept
    { return const_iterator(); }

    const_iterator cbegin() const noexcept
    { return begin(); }

    iterator end() noexcept
    { return iterator(); }

    const_iterator end() const noexcept
    { return const_iterator(); }

    const_iterator cend() const noexcept
    { return end(); }

    reverse_iterator rbegin() noexcept
    { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept
    { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept
    { return rbegin(); }

    reverse_iterator rend() noexcept
    { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept
    { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept
    { return rend(); }

    // 容量

    constexpr bool empty() const noexcept
    { return true; }

    constexpr size_type size() const noexcept
    { return 0; }

    constexpr size_type max_size() const noexcept
    { return 0; }

    // 操作

    void fill(const value_type &)
    { // do nothing        
    }

    void swap(const array &)
    { // do nothing
    }

public:
    /**
     * @brief 抛出out_of_range异常
     */
    [[noreturn]] void _throw_out_of_range() const
    { throw std::out_of_range("invalid array<T, 0> subscript"); }
};

} // namespace wwstl

#endif // __WW_ARRAY_H__