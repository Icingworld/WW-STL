#ifndef __WW_DEQUE_H__
#define __WW_DEQUE_H__

#include <initializer_list>
#include "ww_type_traits.h"
#include "ww_memory.h"
#include "ww_iterator.h"
#include <iostream>

namespace wwstl
{

/**
 * @brief deque常量迭代器
 */
template <class T>
class _deque_const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    using map_pointer = value_type**;           // 指向中控器的指针类型
    using self = _deque_const_iterator<T>;
    using _Ptr = value_type*;                   // 底层使用非const指针

public:
    _Ptr _cur;          // 当前指向的元素
    _Ptr _first;        // 当前缓冲区的第一个元素
    _Ptr _last;         // 当前缓冲区的最后一个元素的后一个位置
    map_pointer _node;  // 指向当前缓冲区指针

public:
    _deque_const_iterator()
        : _cur(nullptr)
        , _first(nullptr)
        , _last(nullptr)
        , _node(nullptr)
    { // 空迭代器
    }

    _deque_const_iterator(map_pointer node)
        : _deque_const_iterator()
    {
        // 初始化指向缓冲区的迭代器
        _set_node(node);
        _cur = _first;
    }

public:
    reference operator*() const
    {
       return *_cur;
    }

    pointer operator->() const
    {
       return &(operator*());
    }

    self & operator++()
    {
        ++_cur;
        if (_cur == _last) {
            // 已经到达尾部，需要移动到下一个缓冲区
            _set_node(_node + 1);
            _cur = _first;
        }
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
        if (_cur == _first) {
            // 已经位于头部，需要移动到上一个缓冲区
            _set_node(_node - 1);
            _cur = _last;
        }
        --_cur;
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
       return _cur == other._cur;
    }

    bool operator!=(const self & other) const
    {
       return !(*this == other);
    }

    bool operator<(const self & other) const
    {
        if (_node == other._node) {
            return _cur < other._cur;
        }
        return _node < other._node;
    }

    bool operator>(const self & other) const
    {
       return other < *this;
    }

    bool operator<=(const self & other) const
    {
       return !(*this > other);
    }

    bool operator>=(const self & other) const
    {
       return !(*this < other);
    }

    self & operator+=(difference_type n)
    {
        difference_type offset = n + (_cur - _first);
        difference_type buffer_size = static_cast<difference_type>(_buffer_size());
        if (offset >= 0 && offset < buffer_size) {
            // 在当前的缓冲区中，直接移动
            _cur += n;
        } else {
            // 不在当前的缓冲区中，需要跳转
            // 计算需要移动的缓冲区数
            difference_type node_offset;
            if (offset > 0) {
                node_offset = offset / buffer_size;
            } else {
                node_offset = -((-offset - 1) / buffer_size) - 1;
            }
            _set_node(_node + node_offset);
            _cur = _first + (offset - node_offset * buffer_size);
        }
        return *this;
    }

    self & operator-=(difference_type n)
    {
        return *this += -n;
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

    difference_type operator-(const self & other) const
    {
       return static_cast<difference_type>((_node - other._node) * _buffer_size() + (_cur - _first) - (other._cur - other._first));
    }

    reference operator[](difference_type n) const
    {
       return *(*this + n);
    }

public:
    /**
     * @brief 设置当前迭代器指向的缓冲区
     */
    void _set_node(map_pointer new_node)
    {
        _node = new_node;
        _first = *_node;
        _last = _first + _buffer_size();
    }

    /**
     * @brief 获取缓冲区的大小
     * @details 在本实现中设置为8个元素大小
     */
    size_type _buffer_size() const noexcept
    {
       return 8 * sizeof(value_type);
    }

    /**
     * @brief 清空迭代器
     */
    void _clear() noexcept
    {
        _cur = _first = _last = nullptr;
        _node = nullptr;
    }
};

/**
 * @brief deque迭代器
 */
template <class T>
class _deque_iterator
    : public _deque_const_iterator<T>
{
public:
    using base = _deque_const_iterator<T>;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    using map_pointer = value_type**;
    using self = _deque_iterator<T>;

public:
    _deque_iterator()
        : base()
    { // 空迭代器
    }

    _deque_iterator(map_pointer node)
        : base(node)
    { // 节点初始化迭代器
    }

    _deque_iterator(const base & x)
        : base(x)
    { // 用于将const_iterator转换为iterator
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
        base::operator++();
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
        base::operator--();
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
        base::operator+=(n);
        return *this;
    }

    self & operator-=(difference_type n)
    {
        base::operator-=(n);
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
    {
       return base::operator-(other);
    }

    reference operator[](difference_type n) const
    {
       return *(*this + n);
    }
};

/**
 * @brief deque
 * @link https://zh.cppreference.com/w/cpp/container/deque
 */
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class deque
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
    using iterator = _deque_iterator<value_type>;
    using const_iterator = _deque_const_iterator<value_type>;
    using reverse_iterator = wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = wwstl::reverse_iterator<const_iterator>;

    using map_pointer = value_type**;   // 中控器指针类型
    using map_allocator_type = typename allocator_type::template rebind<pointer>::other;    // 中控器分配器类型

public:
    iterator _start;                    // 起始位置迭代器，指向头部
    iterator _finish;                   // 结束位置迭代器，指向尾部的下一个位置
    map_pointer _map;                   // 中控器指针
    size_type _map_size;                // 中控器大小
    allocator_type _allocator;          // 元素分配器
    map_allocator_type  _map_allocator; // 中控器分配器

public:
    deque()
        : deque(Allocator())
    {
    }

    explicit deque(const Allocator & alloc)
        : _start(), _finish(), _map(nullptr), _map_size(0), _allocator(alloc), _map_allocator()
    {
        _initialize_map(0);
    }

    explicit deque(size_type count, const Allocator & alloc = Allocator())
        : deque(alloc)
    {
        _initialize_map(count);
        assign(count, value_type());
    }

    deque(size_type count, const value_type & value, const Allocator & alloc = Allocator())
        : deque(alloc)
    {
        _initialize_map(count);
        assign(count, value);
    }

    template <class InputIterator>
    deque(InputIterator first, InputIterator last, const Allocator & alloc = Allocator())
        : deque(alloc)
    {
        _initialize_map(0);
        assign(first, last);
    }

    deque(const deque & other)
        : deque(other._allocator)
    {
        _initialize_map(other.size());
        assign(other.begin(), other.end());
    }

    deque(deque && other)
        : _start(std::move(other._start))
        , _finish(std::move(other._finish))
        , _map(other._map)
        , _map_size(other._map_size)
        , _allocator(std::move(other._allocator))
        , _map_allocator(std::move(other._map_allocator))
    {
        other._start._clear();
        other._finish._clear();
        other._map = nullptr;
        other._map_size = 0;
    }

    deque(const deque & other, const Allocator & alloc)
        : deque(alloc)
    {
        _initialize_map(other.size());
        assign(other.begin(), other.end());
    }

    deque(deque && other, const Allocator & alloc)
        : _start(std::move(other._start))
        , _finish(std::move(other._finish))
        , _map(other._map)
        , _map_size(other._map_size)
        , _allocator(alloc)
        , _map_allocator()
    {
        other._start._clear();
        other._finish._clear();
        other._map = nullptr;
        other._map_size = 0;
    }

    deque(std::initializer_list<value_type> ilist, const Allocator & alloc = Allocator())
        : deque(alloc)
    {
        _initialize_map(ilist.size());
        assign(ilist);
    }

    ~deque()
    {
        if (_map != nullptr) {
            // 暂存迭代器位置
            map_pointer old_start = _start._node;
            map_pointer old_finish = _finish._node;
            // 删除所有元素
            while (!empty()) {
                pop_back();
            }
            // 释放所有的缓冲区
            _deallocate_nodes(old_start, old_finish + 1);
            // 释放中控器
            _deallocate_map();
        }
    }

public:
    deque & operator=(const deque & other)
    {
        if (this != &other) {
            assign(other.begin(), other.end());
            if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                _allocator = other._allocator;
            }
            if (std::allocator_traits<map_allocator_type>::propagate_on_container_copy_assignment::value) {
                _map_allocator = other._map_allocator;
            }
        }
        return *this;
    }

    deque & operator=(deque && other)
    {
        if (this != &other) {
            map_pointer old_start = _start._node;
            map_pointer old_finish = _finish._node;
            // 删除所有元素
            clear();
            // 释放所有的缓冲区
            _deallocate_nodes(old_start, old_finish + 1);
            // 释放中控器
            _deallocate_map();

            _start = std::move(other._start);
            _finish = std::move(other._finish);
            _map = other._map;
            _map_size = other._map_size;
            if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                _allocator = std::move(other._allocator);
            }
            if (std::allocator_traits<map_allocator_type>::propagate_on_container_move_assignment::value) {
                _map_allocator = std::move(other._map_allocator);
            }
            
            // 重置other
            other._start._clear();
            other._finish._clear();
            other._map = nullptr;
            other._map_size = 0;
        }
        return *this;
    }

    deque & operator=(std::initializer_list<value_type> ilist)
    {
        assign(ilist);
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(size_type count, const value_type & value)
    {
        clear();
        insert(end(), count, value);
    }

    /**
     * @brief 将值赋给容器
     */
    template <
        class InputIt,
        class = typename std::enable_if<wwstl::is_iterator<InputIt>::value>::type
    > void assign(InputIt first, InputIt last)
    {
        // 清空原有元素
        clear();
        // 插入新区间元素
        for (; first != last; ++first) {
            push_back(*first);
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
        return *(begin() + pos);
    }

    /**
     * @brief 带越界检查访问指定的元素
     */
    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            _throw_out_of_range();
        }
        return *(begin() + pos);
    }

    /**
     * @brief 访问指定的元素
     */
    reference operator[](size_type pos)
    {
       return begin()[static_cast<difference_type>(pos)];
    }

    /**
     * @brief 访问指定的元素
     */
    const_reference operator[](size_type pos) const
    {
       return begin()[static_cast<difference_type>(pos)];
    }

    /**
     * @brief 访问第一个元素
     */
    reference front()
    {
       return *begin();
    }

    /**
     * @brief 访问第一个元素
     */
    const_reference front() const
    {
       return *begin();
    }

    /**
     * @brief 访问最后一个元素
     */
    reference back()
    {
       return *(--end());
    }

    /**
     * @brief 访问最后一个元素
     */
    const_reference back() const
    {
       return *(--end());
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
       return _start;
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
       return _start;
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
       return _finish;
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
       return _finish;
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
       return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /**
     * @brief 通过释放未使用的内存减少内存的使用
     */
    void shrink_to_fit() noexcept
    { // TODO
    }

    // 修改器

    /**
     * @brief 清除内容
     * @details 清除所有元素，但不释放空间
     */
    void clear() noexcept
    {
        while(!empty()) {
            pop_back();
        }
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, const value_type & value)
    {
       return emplace(pos, value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, value_type && value)
    {
       return emplace(pos, std::move(value));
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
        class InputIt,
        class = typename std::enable_if<wwstl::is_iterator<InputIt>::value>::type
    > iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
       return _emplace_it(pos, first, last);
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
        if (pos == begin()) {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        } else if (pos == end()) {
            emplace_back(std::forward<Args>(args)...);
            return --end();
        } else {
            return _emplace(pos, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        iterator p = pos;
        iterator next = pos;
        ++next;
        // 判断移动前面的元素还是后面的元素
        difference_type nums_before = p - _start;
        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 移动前面的元素
            std::copy_backward(_start, p, next);
            pop_front();
        } else {
            // 移动后面的元素
            std::copy(next, _finish, p);
            pop_back();
        }
        // 所有迭代器都会失效，需要重新计算
        return _start + nums_before;
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        iterator f = first;
        iterator l = last;
        // 判断移动前面的元素还是后面的元素
        difference_type n = l - f;
        difference_type nums_before = f - _start;       // 前面的元素个数
        if (nums_before < static_cast<difference_type>(size() - n / 2)) {
            // 移动前面的元素
            std::copy_backward(_start, f, l);
            // 销毁前面的元素
            for (size_type i = 0; i < n; ++i) {
                _allocator.destroy((_start + i)._cur);
            }
            // 释放前面的缓冲区
            _deallocate_nodes(_start._node, (_start + n)._node);  // 注意_start + n是新的起点缓冲区，不能被释放
            // 设置新的起点
            _start = _start + n;
        } else {
            // 移动后面的元素
            std::copy(l, _finish, f);
            // 销毁后面的元素
            for (size_type i = 0; i < n; ++i) {
                _allocator.destroy((_finish - i - 1)._cur);
            }
            // 释放后面的缓冲区
            _deallocate_nodes((_finish - n + 1)._node, (_finish + 1)._node);
            // 设置新的终点
            _finish = _finish - n;
        }
        return _start + nums_before;
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
        push_back(value);
    }

    /**
     * @brief 在容器末尾原位构造元素
     */
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        if (_finish._cur != _finish._last - 1) {
            _allocator.construct(_finish._cur++, std::forward<Args>(args)...);
        } else {
            _emplace_back(std::forward<Args>(args)...);
        }
    }

    /**
     * @brief 移除末元素
     */
    void pop_back()
    {
        if (_finish._cur != _finish._first) {
            // 当前位置不在缓冲区起始位置
            _allocator.destroy(--_finish._cur);
        } else {
            // 在起始位置，先移动到上一个缓冲区
            _finish._set_node(_finish._node - 1);
            _finish._cur = _finish._last - 1;
            // 再销毁
            _allocator.destroy(_finish._cur);
        }
    }

    /**
     * @brief 插入元素到容器起始
     */
    void push_front(const value_type & value)
    {
        emplace_front(value);
    }

    /**
     * @brief 插入元素到容器起始
     */
    void push_front(value_type && value)
    {
        push_front(value);
    }

    /**
     * @brief 在容器头部原位构造元素
     */
    template <class... Args>
    void emplace_front(Args&&... args)
    {
        if (_start._cur != _start._first) {
            _allocator.construct(--_start._cur, std::forward<Args>(args)...);
        } else {
            _emplace_front(std::forward<Args>(args)...);
        }
    }

    /**
     * @brief 移除首元素
     */
    void pop_front()
    {
        if (_start._cur != _start._last - 1) {
            // 当前位置不在缓冲区末尾位置
            _allocator.destroy(_start._cur++);
        } else {
            // 在末尾位置，先销毁
            _allocator.destroy(_start._cur);
            // 再移动到下一个缓冲区
            _start._set_node(_start._node + 1);
            _start._cur = _start._first;
        }
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
     * @brief 交换内容
     */
    void swap(deque & other)
    {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_map, other._map);
        std::swap(_map_size, other._map_size);
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_allocator, other._allocator);
        }
    }

public:
    /**
     * @brief 申请中控器所需内存
     * @param n 缓冲区个数
     */
    void _allocate_map(size_type n)
    {
        _map = _map_allocator.allocate(n);
    }

    /**
     * @brief 释放中控器内存
     */
    void _deallocate_map()
    {
        _map_allocator.deallocate(_map, _map_size);
    }

    /**
     * @brief 获取缓冲区大小
     */
    static size_type _deque_buffer_size()
    {
        // 设计为使用对象大小的8倍
        return 8 * sizeof(value_type);
    }

    /**
     * @brief 申请一个缓冲区内存
     */
    pointer _allocate_node()
    {
        // 申请一个buffer_size()大小的内存
        return _allocator.allocate(_deque_buffer_size());
    }

    /**
     * @brief 释放一个缓冲区内存
     */
    void _deallocate_node(pointer p)
    {
        _allocator.deallocate(p, _deque_buffer_size());
    }

    /**
     * @brief 在中控器的某个范围内申请缓冲区内存
     */
    void _allocate_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            *cur = _allocate_node();
        }
    }

    /**
     * @brief 在中控器的某个范围内释放缓冲区内存
     */
    void _deallocate_nodes(map_pointer first, map_pointer last)
    {
        for (map_pointer cur = first; cur != last; ++cur) {
            _deallocate_node(*cur);
        }
    }

    /**
     * @brief 初始化中控器
     * @param n 元素个数
     */
    void _initialize_map(size_type n)
    {
        // 计算需要的缓冲区数量
        size_type node_count = n / _deque_buffer_size() + 1;
        // 额外申请2个缓冲区，用于冗余，如果小于最小大小则设置为最小(8)
        _map_size = std::max(static_cast<size_type>(8), static_cast<size_type>(node_count + 2));
        _allocate_map(_map_size);
        // 初始化两个迭代器，选择中间是因为双端队列需要在两端操作
        // 经过这样设置，前端有一半的缓冲区，后端至少有冗余的2个缓冲区
        map_pointer start_node = _map + (_map_size - node_count) / 2;
        map_pointer finish_node = start_node + node_count;
        // 初始化范围内所有缓冲区
        // 除了范围内的缓冲区，其他冗余缓冲区都没有申请内存
        _allocate_nodes(start_node, finish_node);
        // 设置中控器的起始和结束迭代器
        _start._set_node(start_node);
        _start._cur = _start._first;
        _finish._set_node(finish_node - 1);
        _finish._cur = _finish._first + (n % _deque_buffer_size());
    }

    /**
     * @brief 重新分配中控器
     * @details 如果预留的缓冲区足够，则只移动中控器中缓冲区的位置，否则需要重新分配中控器内存
     * @param n 需要添加的缓冲区个数
     * @param is_front 是否是在头部扩容
     */
    void _reallocate_map(size_type n, bool is_front)
    {
        const size_type old_num_nodes = _finish._node - _start._node + 1;
        const size_type new_num_nodes = old_num_nodes + n;
        map_pointer new_start;

        if (_map_size > 2 * new_num_nodes) {
            // 预留的缓冲区足够，则不需要重新分配内存，计算新的起始位置
            if (is_front) {
                // 在头部扩容，则额外需要留出n个缓冲区
                new_start = _map + (_map_size - new_num_nodes) / 2 + n;
            } else {
                new_start = _map + (_map_size - new_num_nodes) / 2;
            }

            if (new_start < _start._node) {
                // 需要向前移动
                std::copy(_start._node, _finish._node + 1, new_start);
            } else {
                // 向后移动
                std::copy_backward(_start._node, _finish._node + 1, new_start + old_num_nodes); // 注意第三个参数是结尾，因为要从后向前拷贝
            }
        } else {
            // 预留的缓冲区不够，需要重新申请内存，并拷贝数据到新的中控器
            size_type new_map_size = _map_size + std::max(_map_size, n) + 2;    // 新空间至少是原来的两倍 + 2
            // 申请一块新内存供新中控器使用
            map_pointer new_map = _map_allocator.allocate(new_map_size);
            if (is_front) {
                new_start = new_map + (new_map_size - new_num_nodes) / 2 + n;
            } else {
                new_start = new_map + (new_map_size - new_num_nodes) / 2;
            }
            // 拷贝数据
            std::copy(_start._node, _finish._node + 1, new_start);
            // 释放旧中控器内存
            _map_allocator.deallocate(_map, _map_size);
            // 设置新的中控器
            _map = new_map;
            _map_size = new_map_size;
        }

        // 设置新的起始和末尾迭代器
        _start._set_node(new_start);
        _finish._set_node(new_start + old_num_nodes - 1);
    }

    /**
     * @brief 在需要时于尾部扩展中控器
     */
    void _reserve_map_at_back(size_type n = 1)
    {
        // 如果后备的缓冲区不够，则需要重新分配中控器
        if (n + 1 > _map_size - (_finish._node - _map)) {
            _reallocate_map(n, false);
        }
    }

    /**
     * @brief 在需要时于头部扩展中控器
     */
    void _reserve_map_at_front(size_type n = 1)
    {
        if (n > _start._node - _map) {
            _reallocate_map(n, true);
        }
    }

    /**
     * @brief 在尾部插入元素的辅助函数
     * @details 当需要跳跃缓冲区时调用该函数
     */
    template <class... Args>
    void _emplace_back(Args&&... args)
    {
        // 判断是否需要重新扩展中控器，如果还有剩余缓冲区则直接构造并移动到下一个缓冲区
        _reserve_map_at_back();
        // 不管是否扩展，下一个缓冲区都是未申请内存的
        *(_finish._node + 1) = _allocate_node();
        // 在当前缓冲区的最后一个位置构造元素
        _allocator.construct(_finish._cur, std::forward<Args>(args)...);
        // 移动到下一个位置
        _finish._set_node(_finish._node + 1);
        _finish._cur = _finish._first;
    }

    /**
     * @brief 在头部插入元素的辅助函数
     * @details 当需要跳跃缓冲区时调用该函数
     */
    template <class... Args>
    void _emplace_front(Args&&... args)
    {
        _reserve_map_at_front();
        // 申请内存
        *(_start._node - 1) = _allocate_node();
        // 需要先移动到上一个缓冲区的最后一个位置，再构造元素
        _start._set_node(_start._node - 1);
        _start._cur = _start._last - 1;
        _allocator.construct(_start._cur, std::forward<Args>(args)...);
    }

    /**
     * @brief 在中间位置插入元素
     */
    template <class... Args>
    iterator _emplace(const_iterator pos, Args&&... args)
    {
        iterator p = pos;
        value_type value(std::forward<Args>(args)...);
        // 判断移动前面的元素还是后面的元素
        difference_type nums_before = p - _start;

        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 移动前面的元素，先在头部插入一个和当前头部一样的元素
            push_front(std::move(front()));
            // 设置标号
            iterator front1 = _start;
            ++front1;
            iterator front2 = front1;
            ++front2;
            p = _start + nums_before;
            iterator pos1 = p;
            ++pos1;
            // 拷贝内存
            std::copy(front2, pos1, front1);
        } else {
            // 移动后面的元素
            push_back(std::move(back()));
            // 设置标号
            iterator back1 = _finish;
            --back1;
            iterator back2 = back1;
            --back2;
            p = _start + nums_before;
            // 拷贝内存
            std::copy_backward(p, back2, back1);
        }
        // 移动元素
        *p = std::move(value);
        return pos;
    }

    /**
     * @brief 在pos位置前插入n个元素
     */
    template <class... Args>
    iterator _emplace_n(const_iterator pos, size_type count, Args&&... args)
    {
        iterator p = pos;
        value_type value(std::forward<Args>(args)...);
        difference_type nums_before = p - _start;

        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 将pos前面的元素向前移动，先判断是否需要扩容
            _reserve_map_at_front(count / _deque_buffer_size() + 1);
            iterator old_start = _start;                                        // 记录起始位置
            p = old_start + nums_before;                                        // 更新pos位置
            _allocate_nodes((old_start - count)._node, old_start._node);        // 申请该范围的内存
            _start = old_start - count;                                         // 更新起始位置
            std::copy(old_start, p, _start);                                    // 拷贝数据到新的位置
            std::fill_n(_start + nums_before, count, value);                    // 填充数据
        } else {
            // 将pos及后面的元素向后移动，先判断是否需要扩容
            _reserve_map_at_back(count / _deque_buffer_size() + 1);
            iterator old_finish = _finish;                                          // 记录结束位置
            _allocate_nodes(old_finish._node + 1, (old_finish + count)._node + 1);  // 申请该范围的内存
            _finish = old_finish + count;                                           // 记录新的结束位置
            std::copy_backward(p, old_finish, _finish);                             // 拷贝数据到新的位置
            std::fill_n(p, count, value);                                           // 填充数据
        }
        return _start + nums_before;
    }

    /**
     * @brief 在pos位置插入[first, last)区间的元素
     */
    template <class InputIt>
    iterator _emplace_it(const_iterator pos, InputIt first, InputIt last)
    {
        iterator p = pos;
        difference_type nums_before = p - _start;
        difference_type count = std::distance(first, last);

        if (nums_before < static_cast<difference_type>(size() / 2)) {
            // 将pos前面的元素向前移动，先判断是否需要扩容
            _reserve_map_at_front(count / _deque_buffer_size() + 1);
            iterator old_start = _start;                                        // 记录起始位置
            p = old_start + nums_before;                                        // 更新pos位置
            _allocate_nodes((old_start - count)._node, old_start._node);        // 申请该范围的内存
            _start = old_start - count;                                         // 更新起始位置
            std::copy(old_start, p, _start);                                    // 拷贝数据到新的位置
            std::copy(first, last, p);                                          // 拷贝数据到新的位置
        } else {
            // 将pos及后面的元素向后移动，先判断是否需要扩容
            _reserve_map_at_back(count / _deque_buffer_size() + 1);
            iterator old_finish = _finish;                                          // 记录结束位置
            _allocate_nodes(old_finish._node + 1, (old_finish + count)._node + 1);  // 申请该范围的内存
            _finish = old_finish + count;                                           // 记录新的结束位置
            std::copy_backward(p, old_finish, _finish);                             // 拷贝数据到新的位置
            std::copy(first, last, p);                                              // 拷贝数据到新的位置
        }
        return _start + nums_before;
    }

    [[noreturn]] reference _throw_out_of_range() const
    {
        throw std::out_of_range("invalid deque<T, Allocator> subscript");
    }
};

// 非成员函数

template <
    class T,
    class Alloc
> bool operator==(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <
    class T,
    class Alloc
> bool operator!=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(lhs == rhs);
}

template <
    class T,
    class Alloc
> bool operator<(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <
    class T,
    class Alloc
> bool operator>(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return rhs < lhs;
}

template <
    class T,
    class Alloc
> bool operator<=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(rhs < lhs);
}

template <
    class T,
    class Alloc
> bool operator>=(const deque<T, Alloc> & lhs, const deque<T, Alloc> & rhs)
{
    return !(lhs < rhs);
}

template <
    class T,
    class Alloc
> void swap(deque<T, Alloc> & lhs, deque<T, Alloc> & rhs)
{
    lhs.swap(rhs);
}

} // namespace wwstl

#endif // __WW_DEQUE_H__