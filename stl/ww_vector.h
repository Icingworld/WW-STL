#ifndef __WW_VECTOR_H__
#define __WW_VECTOR_H__

#include <initializer_list>
#include <memory>
#include "ww_iterator.h"
#include "ww_memory.h"

namespace wwstl
{

/**
 * @brief vector常量迭代器
 */
template <class T>
class _vector_const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    using self = _vector_const_iterator<value_type>;
    using _Ptr = value_type*;   // 底层使用非const指针

public:
    _Ptr _ptr;  // 指向array的指针

public:
    _vector_const_iterator()
        : _ptr(nullptr)
    { // 构造一个空迭代器
    }
    explicit _vector_const_iterator(pointer ptr)
        : _ptr(const_cast<_Ptr>(ptr))
    { // 用指针ptr构造一个迭代器
    }

public:
    reference operator*() const
    {
        return *_ptr;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

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
    {
        return _ptr == other._ptr;
    }

    bool operator!=(const self & other) const
    {
        return !(*this == other);
    }

    bool operator<(const self & other) const
    {
        return _ptr < other._ptr;
    }

    bool operator>(const self & other) const
    {
        return other < *this;
    }

    bool operator<=(const self & other) const
    {
        return !(other < *this);
    }

    bool operator>=(const self & other) const
    {
        return !(*this < other);
    }

    self & operator+=(const difference_type n)
    {
        _ptr += n;
        return *this;
    }

    self & operator-=(const difference_type n)
    {
        return *this += -n;
    }

    self operator+(const difference_type n) const
    {
        self tmp = *this;
        return tmp += n;
    }

    self operator-(const difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const self & other) const
    {
        return _ptr - other._ptr;
    }

    reference operator[](const difference_type n) const
    {
        return *(*this + n);
    }
};

/**
 * @brief vector迭代器
 */
template <class T>
class _vector_iterator
    : public _vector_const_iterator<T>
{
public:
    using base = _vector_const_iterator<T>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    using self = _vector_iterator<value_type>;

public:
    _vector_iterator()
        : base()
    { // 构造一个空迭代器
    }

    _vector_iterator(pointer ptr)
        : base(ptr)
    { // 用指针ptr构造一个迭代器
    }

public:
    reference operator*() const
    {
        return const_cast<reference>(base::operator*());
    }

    pointer operator->() const
    {
        return const_cast<pointer>(base::operator->());
    }

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
        self tmp = *this;
        return tmp += n;
    }

    self operator-(difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const base & other) const
    {
        return *(base *)this - other;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }
};

/**
 * @brief vector
 * @link https://zh.cppreference.com/w/cpp/container/vector
 */
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class vector
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = _vector_iterator<value_type>;
    using const_iterator = _vector_const_iterator<value_type>;
    using reverse_iterator = wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = wwstl::reverse_iterator<const_iterator>;

public:
    pointer _start;             // 起始元素指针
    pointer _finish;            // 结束元素的下一位指针
    pointer _end_of_storage;    // 空间末尾指针
    allocator_type _allocator;  // 分配器

public:
    vector()
        : vector(Allocator())
    {
    }

    explicit vector(const Allocator & alloc)
        : _start(nullptr)
        , _finish(nullptr)
        , _end_of_storage(nullptr)
        , _allocator(alloc)
    {
    }

    explicit vector(size_type count, const Allocator & alloc = Allocator())
        : vector(alloc)
    {
        assign(count, value_type());
    }

    vector(size_type count, const value_type & value, const Allocator & alloc = Allocator())
        : vector(alloc)
    {
        assign(count, value);
    }

    template <
        class InputIt,
        class = typename std::enable_if<wwstl::is_iterator<InputIt>::value>::type
    > vector(InputIt first, InputIt last, const Allocator & alloc = Allocator())
        : vector(alloc)
    {
        assign(first, last);
    }

    vector(const vector & other)
        : vector(allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator()))
    {
        assign(other.begin(), other.end());
    }

    vector(vector && other)
        : _start(other._start)
        , _finish(other._finish)
        , _end_of_storage(other._end_of_storage)
        , _allocator(std::move(other.get_allocator()))
    {
        other._set_new_space(nullptr, nullptr, nullptr);
    }

    vector(const vector & other, const Allocator & alloc)
        : vector(alloc)
    {
        assign(other.begin(), other.end());
    }

    vector(vector && other, const Allocator & alloc)
        : _allocator(alloc)
    {
        if (_allocator == other._allocator) {
            _set_new_space(other._start, other._finish, other._end_of_storage);
            other._set_new_space(nullptr, nullptr, nullptr);
        } else {
            _set_new_space(nullptr, nullptr, nullptr);
            assign(other.begin(), other.end());
        }
    }

    vector(std::initializer_list<value_type> init, const Allocator & alloc = Allocator())
        : vector(init.begin(), init.end(), alloc)
    {
    }

    ~vector()
    {
        _clean();
    }

public:
    // 成员函数

    /**
     * @brief 将值赋给容器
     */
    vector & operator=(const vector & other)
    {
        if (this != &other) {
            assign(other.begin(), other.end());
            if (allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                _allocator = other._allocator;
            }
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    vector & operator=(vector && other)
    {
        if (this != &other) {
            // 销毁原有的资源
            _clean();
            // 移动新的资源
            _set_new_space(other._start, other._finish, other._end_of_storage);
            if (allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                _allocator = std::move(other._allocator);
            }
            // 清空原来的资源
            other._set_new_space(nullptr, nullptr, nullptr);
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    vector & operator=(std::initializer_list<value_type> ilist)
    {
        assign(ilist);
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(size_type count, const value_type & value)
    {
        size_type old_size = size();
        size_type old_capacity = capacity();
        if (count > old_capacity) {
            // 超出了最大空间，需要重新分配
            pointer new_start = _allocator.allocate(count);
            std::uninitialized_fill_n(new_start, count, value);
            // 销毁原来的空间
            _clean();
            // 设置新的空间
            _set_new_space(new_start, count, count);
        } else if (count > old_size) {
            // 超过了原来的大小，但不需要重新分配
            // 原来的区间直接赋值
            std::fill_n(_start, old_size, value);
            // 多余区间直接构造新的值
            std::uninitialized_fill_n(_start + old_size, count - old_size, value);
            _finish = _start + count;
        } else {
            // 不足原来的大小
            // 目标区间直接赋值
            std::fill_n(_start, count, value);
            // 多余区间直接销毁
            _destroy_n(_start + count, old_size - count);
            _finish = _start + count;
        }
    }

    /**
     * @brief 将值赋给容器
     */
    template <
        class InputIterator,
        class = typename std::enable_if<wwstl::is_iterator<InputIterator>::value>::type
    > void assign(InputIterator first, InputIterator last)
    {
        size_type count =  std::distance(first, last);
        size_type old_size = size();
        size_type old_capacity = capacity();
        if (count > old_capacity) {
            // 超出了最大空间，需要重新分配
            pointer new_start = _allocator.allocate(count);
            // 按迭代器顺序赋值
            std::uninitialized_copy(first, last, new_start);
            // 销毁原来的空间
            _clean();
            // 设置新的空间
            _set_new_space(new_start, count, count);
        } else if (count > old_size) {
            // 超过了原来的大小，但不需要重新分配
            // 原来的区间
            InputIterator it = first;
            for (size_type i=0; i<old_size; ++i) {
                *(_start + i) = *it;
                ++it;
            }
            // 多余区间直接构造新的值
            for (size_type i=old_size; i<count; ++i) {
                _allocator.construct(_start + i, *it);
                ++it;
            }
            _finish = _start + count;
        } else {
            // 不足原来的大小
            // 原来的区间直接赋值
            std::copy(first, last, _start);
            // 多余区间直接销毁
            _destroy_n(_start + count, old_size - count);
            _finish = _start + count;
        }
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(std::initializer_list<value_type> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const noexcept
    {
        return _allocator;
    }

    // 元素访问

    /**
     * @brief 带越界检查访问指定的元素
     */
    reference at(size_type pos)
    {
        if (pos >= size()) {
            _throw_out_of_range();
        }
        return *(_start + pos);
    }

    /**
     * @brief 带越界检查访问指定的元素
     */
    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            _throw_out_of_range();
        }
        return *(_start + pos);
    }

    /**
     * @brief 访问指定的元素
     */
    reference operator[](size_type pos)
    {
        return *(_start + pos);
    }

    /**
     * @brief 访问指定的元素
     */
    const_reference operator[](size_type pos) const
    {
        return *(_start + pos);
    }

    /**
     * @brief 访问第一个元素
     */
    reference front()
    {
        return *_start;
    }

    /**
     * @brief 访问第一个元素
     */
    const_reference front() const
    {
        return *_start;
    }

    /**
     * @brief 访问最后一个元素
     */
    reference back()
    {
        return *(_finish - 1);
    }

    /**
     * @brief 访问最后一个元素
     */
    const_reference back() const
    {
        return *(_finish - 1);
    }

    /**
     * @brief 直接访问底层连续存储
     */
    pointer data()
    {
        return _start;
    }

    /**
     * @brief 直接访问底层连续存储
     */
    const_pointer data() const
    {
        return _start;
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return iterator(_start);
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return const_iterator(_start);
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return iterator(_finish);
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return const_iterator(_finish);
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return end();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _start == _finish;
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _finish - _start;
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }

    /**
     * @brief 预留存储空间
     */
    void reserve(size_type new_cap)
    {
        if (new_cap > capacity()) {
            if (new_cap > max_size()) {
                _throw_length_error();
            }
            size_type growth = std::max(new_cap, capacity() * 2);
            _reallocate(growth);
        }
    }

    /**
     * @brief 返回当前存储空间能够容纳的元素数
     */
    size_type capacity() const noexcept
    {
        return _end_of_storage - _start;
    }

    /**
     * @brief 通过释放未使用的内存减少内存的使用
     */
    void shrink_to_fit()
    {
        if (size() < capacity())
            _reallocate(size());
    }

    // 修改器

    /**
     * @brief 清除内容
     */
    void clear() noexcept
    {
        _destroy_n(_start, size());
        _finish = _start;
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, const value_type & value)
    {
        return _emplace_n(pos, 1, value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, value_type && value)
    {
        return _emplace_n(pos, 1, std::move(value));
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, size_type count, const value_type & value)
    {
        return _emplace_n(pos, count, value);
    }

    /**
     * @brief 插入元素
     */
    template <
        class InputIterator,
        class = typename std::enable_if<wwstl::is_iterator<InputIterator>::value>::type
    > iterator insert(const_iterator pos, InputIterator first, InputIterator last)
    {
        size_type n = std::distance(cbegin(), pos);
        size_type count = std::distance(first, last);
        _check_newsize(count);
        std::move_backward(_start + n, _finish, _finish + count);
        std::copy(first, last, _start + n);
        _finish += count;
        return iterator(_start + n);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief 原位构造元素
     */
    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        return _emplace_n(pos, 1, std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        pointer p = pos._ptr;
        std::move(p + 1, _finish, p);
        _allocator.destroy(_finish);
        --_finish;
        return iterator(p);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last)
            return iterator(last._ptr);

        pointer p = first._ptr;
        std::move(last._ptr, _finish, p);
        for (pointer q = last._ptr; q != _finish; ++q) {
            _allocator.destroy(q);
        }
        _finish = _finish - (last._ptr - first._ptr);
        return iterator(p);
    }

    /**
     * @brief 将元素添加到容器末尾
     */
    void push_back(const value_type & value)
    {
        emplace_back(value);
    }

    /**
     * @brief 将元素添加到容器末尾
     */
    void push_back(value_type && value)
    {
        emplace_back(std::move(value));
    }

    /**
     * @brief 在容器末尾原位构造元素
     */
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        emplace(end(), std::forward<Args>(args)...);
    }

    /**
     * @brief 改变存储元素的个数
     */
    void resize(size_type count) 
    {
        resize(count, value_type());
    }

    /**
     * @brief 改变存储元素的个数
     */
    void resize(size_type count, const value_type & value)
    {
        if (count == size())
            return;
        if (count < size()) {
            erase(begin() + count, end());
        } else {
            insert(end(), count - size(), value);
        }
    }

    /**
     * @brief 移除末元素
     */
    void pop_back()
    {
        erase(end() - 1);
    }

    /**
     * @brief 交换内容
     */
    void swap(vector & other)
    {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_end_of_storage, other._end_of_storage);
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_allocator, other._allocator);
        }
    }

public:
    /**
     * @brief 销毁从某个位置开始的count个元素
     */
    void _destroy_n(pointer start, size_type count)
    {
        for (size_type i = 0; i < count; ++i) {
            _allocator.destroy(start + i);
        }
    }

    /**
     * @brief 设置新空间指针
     */
    void _set_new_space(pointer new_start, size_type new_size, size_type new_cap) noexcept
    {
        _start = new_start;
        _finish = new_start + new_size;
        _end_of_storage = new_start + new_cap;
    }

    /**
     * @brief 设置新空间指针
     */
    void _set_new_space(pointer new_start, pointer new_finish, pointer new_end_of_storage) noexcept
    {
        _start = new_start;
        _finish = new_finish;
        _end_of_storage = new_end_of_storage;
    }

    /**
     * @brief 重新分配内存
     * @details 不支持移动
     */
    void _reallocate_aux(std::false_type, size_type new_cap)
    {
        size_type old_size = size();
        pointer new_start = _allocator.allocate(new_cap);
        std::uninitialized_copy(begin(), end(), iterator(new_start));
        _clean();
        _set_new_space(new_start, old_size, new_cap);
    }

    /**
     * @brief 重新分配内存
     * @details 支持移动
     */
    void _reallocate_aux(std::true_type, size_type new_cap)
    {
        size_type old_size = size();
        pointer new_start = _allocator.allocate(new_cap);
        for (size_type i = 0; i < old_size; ++i) {
            _allocator.construct(new_start + i, std::move(*(_start + i)));
        }
        _clean();
        _set_new_space(new_start, old_size, new_cap);
    }

    /**
     * @brief 重新分配内存
     * @details 需要保证 new_cap > size
     */
    void _reallocate(size_type new_cap)
    {
        _reallocate_aux(std::is_move_constructible<value_type>(), new_cap);
    }

    /**
     * @brief 判断插入count个元素是否需要重新分配内存
     */
    void _check_newsize(size_type count)
    {
        if (count > 0) {
            size_type target_size = size() + count;
            if (target_size > max_size()) {
                _throw_length_error();
            }
            if (target_size > capacity()) {
                // 需要扩容
                if (count == 1) {
                    // 只需要插入一个元素，两倍扩容
                    if (capacity() == 0) {
                        reserve(1);
                    } else {
                        reserve(capacity() * 2);
                    }
                } else {
                    // 需要插入多个元素，直接扩容
                    reserve(target_size);
                }
            }
        }
    }

    /**
     * @brief 在pos位置插入count个元素
     * @details TODO 其逻辑仍需完善
     */
    template <class... Args>
    iterator _emplace_n(const_iterator pos, size_type count, Args&&... args)
    {
        difference_type offset = pos - begin();
        _check_newsize(count);     // 判断是否需要扩容
        std::move_backward(begin() + offset, end(), end() + count);
        for (size_type i = 0; i < count; ++i) {
            _allocator.construct(_start + offset + i, std::forward<Args>(args)...);
        }
        _finish += count;
        return begin() + offset;
    }

    /**
     * @brief 清空vector为初始状态
     */
    void _clean()
    {
        if (_start) {
            for (pointer p = _finish; p != _start;) {
                _allocator.destroy(--p);
            }
            _allocator.deallocate(_start, _end_of_storage - _start);
            _start = _finish = _end_of_storage = nullptr;
        }
    }

    [[noreturn]] void _throw_out_of_range() const
    {
        throw std::out_of_range("invalid vector<T, Allocator> subscript");
    }

    [[noreturn]] void _throw_length_error() const
    {
        throw std::length_error("vector<T, Allocator> too long");
    }
};

// 非成员函数

template <
    class T,
    class Alloc
> bool operator==(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <
    class T,
    class Alloc
> bool operator!=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(lhs == rhs);
}

template <
    class T,
    class Alloc
> bool operator<(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <
    class T,
    class Alloc
> bool operator>(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return rhs < lhs;
}

template <
    class T,
    class Alloc
> bool operator<=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(rhs < lhs);
}

template <
    class T,
    class Alloc
> bool operator>=(const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
    return !(lhs < rhs);
}

template <
    class T,
    class Alloc
> void swap(vector<T, Alloc> & lhs, vector<T, Alloc> & rhs)
{
    lhs.swap(rhs);
}

/**
 * @brief _bit_proxy
 * @details 位代理，提供访问位的能力
 */
class _bit_proxy
{
public:
    using bitset = unsigned long;
    using size_type = std::size_t;

public:
    bitset * _word;      // 位所在的 word
    size_type _index;         // 位在 word 中的下标

public:
    _bit_proxy(bitset * word, size_type index)
        : _word(word)
        , _index(index)
    {
    }

public:
    /**
     * @brief 写入 bool
     */
    _bit_proxy & operator=(bool value)
    {
        if (value) {
            *_word |= 1UL << _index;
        } else {
            *_word &= ~(1UL << _index);
        }
        return *this;
    }

    /**
     * @brief 读取 bool
     */
    operator bool() const
    {
        return (*_word >> _index) & 1UL;
    }
};

/**
 * @brief _vector_bool_const_iterator
 */
class _vector_bool_const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = bool;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = void;                   // 不允许返回 bool*
    using reference = const _bit_proxy;     // 返回位代理

    using bitset = unsigned long;
    using self = _vector_bool_const_iterator;

public:
    bitset * _data;
    size_type _index;

public:
    _vector_bool_const_iterator(bitset * data, size_type index)
        : _data(data)
        , _index(index)
    {
    }

public:
    reference operator*() const
    {
        return _bit_proxy(_data, _index);
    }

    self & operator++()
    {
        ++_index;
        if (_index >= sizeof(bitset) * 8) {
            ++_data;
            _index = 0;
        }
        return *this;
    }

    self & operator++(int n)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self & operator--()
    {
        if (_index == 0) {
            --_data;
            _index = sizeof(bitset) * 8 - 1;
        } else {
            --_index;
        }
        return *this;
    }

    self & operator--(int n)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self & rhs) const
    {
        return _data == rhs._data && _index == rhs._index;
    }

    bool operator!=(const self & rhs) const
    {
        return !(*this == rhs);
    }

    self & operator+=(const difference_type n)
    {
        size_t total_bits = _index + n;
        _data += total_bits / (sizeof(bitset) * 8);     // 更新数据指针
        _index = total_bits % (sizeof(bitset) * 8);     // 更新位索引
        return *this;
    }

    self & operator-=(const difference_type n)
    {
        return *this += -n;
    }

    self operator+(const difference_type n) const
    {
        self tmp = *this;
        return tmp += n;
    }

    self operator-(const difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const self & other) const
    {
        return (_data - other._data) * sizeof(bitset) * 8 + (_index - other._index);
    }

    reference operator[](const difference_type n) const
    {
        return *(*this + n);
    }
};

/**
 * @brief _vector_bool_iterator
 */
class _vector_bool_iterator
    : public _vector_bool_const_iterator
{
public:
    using base = _vector_bool_const_iterator;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = bool;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = void;                   // 不允许返回 bool*
    using reference = _bit_proxy;           // 返回位代理

    using bitset = unsigned long;
    using self = _vector_bool_iterator;

public:
    _vector_bool_iterator(bitset * data, size_type index)
        : base(data, index)
    {
    }

public:
    reference operator*() const
    {
        return _bit_proxy(_data, _index);
    }

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
        self tmp = *this;
        return tmp += n;
    }

    self operator-(difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const base & other) const
    {
        return *(base *)this - other;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }
};

} // namespace wwstl

#endif // __WW_VECTOR_H__