#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <cstddef>

namespace wwstl
{

// 这些在设计algorithm时会使用到，对不同的迭代器类型有不同的行为
// 本实现中未使用，而是使用标准库中的定义
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/**
 * @brief iterator
 * @details 迭代器的基类，定义了迭代器的五种属性
 */
template<
    class Category,
    class T,
    class Distance = std::ptrdiff_t,
    class Pointer = T*,
    class Reference = T&
> class iterator
{
public:
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

/**
 * @brief reverse_iterator
 * @details 逆向迭代器，是对正向迭代器的封装
 * @link https://zh.cppreference.com/w/cpp/iterator/reverse_iterator
 */
template <class Iter>
class reverse_iterator
{
public:
    using iterator_type = Iter;     // 迭代器类型
    using iterator_category = typename Iter::iterator_category;
    using value_type = typename Iter::value_type;
    using difference_type = typename Iter::difference_type;
    using pointer = typename Iter::pointer;
    using reference = typename Iter::reference;

    using self = reverse_iterator<Iter>;

public:
    iterator_type _current;  // 底层迭代器

public:
    reverse_iterator()
        : _current()
    {}

    explicit reverse_iterator(iterator_type x)
        : _current(x)
    {}

    template <class U>
    reverse_iterator(const reverse_iterator<U> & u)
        : _current(u._current)
    {}

public:
    template <class U>
    self & operator=(const reverse_iterator<U> & u)
    {
        _current = u._current;
        return *this;
    }

    iterator_type base() const
    { return _current; }

    reference operator*() const
    {
        // 先退后一个位置，再取值
        iterator_type tmp = _current;
        return *--tmp;
    }

    pointer operator->() const
    { return &(operator*()); }

    reference operator[](difference_type n) const
    { return _current[-n - 1]; }

    self & operator++()
    {
        --_current;
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        --_current;
        return tmp;
    }

    self & operator--()
    {
        ++_current;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        ++_current;
        return tmp;
    }

    self operator+(difference_type n) const
    { return self(_current - n); }

    self & operator+=(difference_type n)
    {
        _current -= n;
        return *this;
    }

    self operator-(difference_type n) const
    { return self(_current + n); }

    self& operator-=(difference_type n)
    {
        _current += n;
        return *this;
    }
};

/**
 * 与reverse_iterator相关的非成员函数
 */

template <
    class Iter1,
    class Iter2
> bool operator==(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() == rhs.base(); }

template <
    class Iter1,
    class Iter2
> bool operator!=(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() != rhs.base(); }

template <
    class Iter1,
    class Iter2
> bool operator<(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() > rhs.base(); }

template <
    class Iter1,
    class Iter2
> bool operator<=(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() >= rhs.base(); }

template <
    class Iter1,
    class Iter2
> bool operator>(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() < rhs.base(); }

template <
    class Iter1,
    class Iter2
> bool operator>=(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs)
{ return lhs.base() <= rhs.base(); }

/**
 * @brief 令迭代器前进
 */
template <class Iter>
reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter> & it)
{ return reverse_iterator<Iter>(it.base() - n); }

/**
 * @brief 计算两个迭代器之间的距禈
 */
template <
    class Iter1,
    class Iter2
> auto operator-(const reverse_iterator<Iter1> & lhs, const reverse_iterator<Iter2> & rhs) -> decltype(rhs.base() - lhs.base())
{ return rhs.base() - lhs.base(); }

} // namespace wwstl

#endif