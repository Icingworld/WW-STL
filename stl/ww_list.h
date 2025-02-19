#ifndef __WW_LIST_H__
#define __WW_LIST_H__

#include <initializer_list>
#include "ww_type_traits.h"
#include "ww_iterator.h"
#include "ww_memory.h"

namespace wwstl
{

/**
 * @brief list节点类
 */
template <class T>
class _list_node
{
public:
    using value_type = T;
    using node_pointer = _list_node<value_type>*;

public:
    node_pointer _prev;     // 上一个节点
    node_pointer _next;     // 下一个节点
    value_type _data;       // 节点数据

public:
    _list_node()
        : _prev(nullptr)
        , _next(nullptr)
        , _data()
    { // 空节点
    }

    explicit _list_node(const value_type & value)
        : _prev(nullptr)
        , _next(nullptr)
        , _data(value)
    { // 带数据的节点
    }

    explicit _list_node(value_type && value)
        : _prev(nullptr)
        , _next(nullptr)
        , _data(std::move(value))
    { // 带数据的节点
    }
};

/**
 * @brief list常量迭代器类
 */
template <class T>
class _list_const_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    using self = _list_const_iterator<value_type>;
    using node_pointer = _list_node<value_type>*;

public:
    node_pointer _node;     // 指向当前节点

public:
    _list_const_iterator()
        : _node(nullptr)
    { // 空迭代器
    }

    explicit _list_const_iterator(const node_pointer node)
        : _node(const_cast<node_pointer>(node))
    { // 节点初始化迭代器
    }

public:
    reference operator*() const
    {
        return _node->_data;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    self & operator++()
    {
        _node = _node->_next;
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
        _node = _node->_prev;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        _node = _node->_prev;
        return tmp;
    }

    bool operator==(const self & other) const
    {
        return _node == other._node;
    }

    bool operator!=(const self & other) const
    {
        return !(*this == other);
    }
};

/**
 * @brief list迭代器类
 */
template <class T>
class _list_iterator
    : public _list_const_iterator<T>
{
public:
    using base = _list_const_iterator<T>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    using self = _list_iterator<value_type>;
    using node_pointer = _list_node<value_type>*;

public:
    _list_iterator()
        : base()
    { // 空迭代器
    }

    explicit _list_iterator(node_pointer node)
        : base(node)
    { // 节点初始化迭代器
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
};

/**
 * @brief list
 * @link https://zh.cppreference.com/w/cpp/container/list
 */
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class list
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
    using iterator = _list_iterator<value_type>;
    using const_iterator = _list_const_iterator<value_type>;
    using reverse_iterator = wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = wwstl::reverse_iterator<const_iterator>;

    using node_type = _list_node<value_type>;
    using node_pointer = node_type*;
    using node_allocator_type = typename allocator_type::template rebind<node_type>::other;

public:
    node_pointer _finish;                   // 指向最后一个节点后一位的虚拟节点
    size_type _size;                        // 节点个数
    node_allocator_type _node_allocator;    // 节点分配器

public:
    list()
        : list(Allocator())
    {
    }

    explicit list(const Allocator & alloc)
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
    }

    explicit list(size_type count, const Allocator & alloc = Allocator())
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
        insert(begin(), count, value_type());
    }

    list(size_type count, const value_type & value, const Allocator & alloc = Allocator())
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
        insert(begin(), count, value);
    }

    template<
        class InputIt,
        class = typename std::enable_if<wwstl::is_iterator<InputIt>::value>::type
    > list(InputIt first, InputIt last, const Allocator & alloc = Allocator())
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
        insert(begin(), first, last);
    }

    list(const list & other)
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(other._node_allocator)
    { 
        _init_list();
        assign(other.begin(), other.end());
    }

    list(list && other)
        : _finish(other._finish)
        , _size(other._size)
        , _node_allocator(std::move(other._node_allocator))
    {
        other._init_list();
        other._size = 0;
    }

    list(const list & other, const Allocator & alloc)
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
        assign(other.begin(), other.end());
    }

    list(list && other, const Allocator & alloc)
        : _finish(other._finish)
        , _size(other._size)
        , _node_allocator(alloc)
    {
        other._init_list();
        other._size = 0;
    }

    list(std::initializer_list<T> init, const Allocator & alloc = Allocator())
        : _finish(nullptr)
        , _size(0)
        , _node_allocator(alloc)
    {
        _init_list();
        assign(init);
    }

    ~list()
    {
        clear();
        _destroy_node(_finish);
    }

public:
    /**
     * @brief 将值赋给容器
     */
    list & operator=(const list & other)
    {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    list & operator=(list && other)
    {
        if (this != &other) {
            clear();
            _finish = other._finish;
            _size = other._size;
            other._finish = nullptr;
            other._size = 0;
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    list & operator=(std::initializer_list<value_type> ilist)
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
        insert(begin(), count, value);
    }

    /**
     * @brief 将值赋给容器
     */
    template <
        class InputIterator,
        class = typename std::enable_if<wwstl::is_iterator<InputIterator>::value>::type
    > void assign(InputIterator first, InputIterator last)
    {
        clear();
        insert(begin(), first, last);
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const noexcept
    {
        return _node_allocator;
    }

    // 元素访问

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
        return iterator(_finish->_next);
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return const_iterator(_finish->_next);
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return const_iterator(_finish->_next);
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
        return const_iterator(_finish);
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
        return begin() == end();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _size;
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return std::min(std::numeric_limits<difference_type>::max(), wwstl::allocator_traits<node_allocator_type>::max_size(_node_allocator));
    }

    // 修改器

    /**
     * @brief 清除内容
     */
    void clear()
    {
        node_pointer p = _finish->_next;
        while (p != _finish) {
            node_pointer q = p->_next;
            _destroy_node(p);
            p = q;
        }
        _finish->_next = _finish;
        _finish->_prev = _finish;
        _size = 0;
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
        if (count == 0)
            return iterator(pos._node);

        // 首先处理first和前一个节点的连接
        node_pointer first = _create_node(value);
        first->_prev = pos._node->_prev;
        pos._node->_prev->_next = first;
        // 后续的节点是插入在first和pos._node之间
        node_pointer cur = first;
        for (size_type i=1; i<count; ++i) {
            node_pointer tmp = _create_node(value);
            tmp->_prev = cur;
            cur->_next = tmp;
            cur = tmp;
        }
        // 最后合并cur和pos._node
        cur->_next = pos._node;
        pos._node->_prev = cur;
        _size += count;
        return iterator(first);
    }

    /**
     * @brief 插入元素
     */
    template <
        class InputIt,
        class = typename std::enable_if<wwstl::is_iterator<InputIt>::value>::type
    > iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
            return iterator(pos._node);

        node_pointer first_node = _create_node(*first);
        first_node->_prev = pos._node->_prev;
        pos._node->_prev->_next = first_node;
        node_pointer cur = first_node;
        for (auto it=std::next(first); it!=last; ++it) {
            node_pointer tmp = _create_node(*it);
            tmp->_prev = cur;
            cur->_next = tmp;
            cur = tmp;
        }
        cur->_next = pos._node;
        pos._node->_prev = cur;
        _size += std::distance(first, last);
        return iterator(first_node);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief 原位构造元素
     */
    template <class... Arg>
    iterator emplace(const_iterator pos, Arg&&... args)
    {
        node_pointer tmp = _create_node(std::forward<Arg>(args)...);
        tmp->_prev = pos._node->_prev;
        tmp->_next = pos._node;
        pos._node->_prev->_next = tmp;
        pos._node->_prev = tmp;
        ++_size;
        return iterator(tmp);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        node_pointer p = pos._node;
        // 修改前后指针
        p->_prev->_next = p->_next;
        p->_next->_prev = p->_prev;
        node_pointer next = p->_next;
        // 销毁p
        _destroy_node(p);
        --_size;
        return iterator(next);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last)
            return iterator(last._node);

        size_type count = std::distance(first, last);
        node_pointer first_node = first._node->_prev;
        node_pointer last_node = last._node;
        // 逐个销毁
        for (node_pointer cur=first._node; cur!=last._node;) {
            node_pointer next = cur->_next;
            _destroy_node(cur);
            cur = next;
        }
        // 连接节点
        first_node->_next = last_node;
        last_node->_prev = first_node;
        _size -= count;
        return iterator(last_node);
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
    template <class... Arg>
    void emplace_back(Arg&&... args)
    {
        emplace(end(), std::forward<Arg>(args)...);
    }

    /**
     * @brief 移除末元素
     */
    void pop_back()
    {
        erase(--end());
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
    template <class... Arg>
    void emplace_front(Arg&&... args)
    {
        emplace(begin(), std::forward<Arg>(args)...);
    }

    /**
     * @brief 移除首元素
     */
    void pop_front()
    {
        erase(begin());
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
        size_type old_size = size();
        if (count == old_size)
            return;

        else if (count < old_size)
        {
            for (size_type i = 0; i < old_size - count; ++i) {
                pop_back();
            }
        } else {
            for (size_type i = 0; i < count - old_size; ++i) {
                push_back(value);
            }
        }
    }

    /**
     * @brief 交换内容
     */
    void swap(list & other)
    {
        std::swap(_finish, other._finish);
        std::swap(_size, other._size);
        if (wwstl::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_node_allocator, other._node_allocator);
        }
    }

    // 操作

    /**
     * @brief 合并两个有序列表
     */
    void merge(list & other)
    {
        merge(other, std::less<value_type>());
    }

    /**
     * @brief 合并两个有序列表
     */
    void merge(list && other)
    {
        merge(std::move(other), std::less<value_type>());
    }

    /**
     * @brief 合并两个有序列表
     */
    template <class Compare>
    void merge(list & other, Compare comp)
    {
        merge(std::move(other), comp);
    }

    /**
     * @brief 合并两个有序列表
     */
    template <class Compare>
    void merge(list && other, Compare comp)
    {
        if (this == &other)
            return;
        
        node_pointer this_cur = _finish->_next;
        node_pointer other_cur = other._finish->_next;

        while (this_cur != _finish && other_cur != other._finish) {
            if (comp(this_cur->_data, other_cur->_data)) {
                this_cur = this_cur->_next;
            } else {
                node_pointer other_next = other_cur->_next;
                _splice(iterator(this_cur), other, iterator(other_cur), iterator(other_next), 1);
                other_cur = other_next;
            }
        }

        if (other_cur != other._finish) {
            iterator other_it(other_cur);
            _splice(end(), other, other_it, other.end(), std::distance(other_it, other.end()));
        }
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list & other)
    {
        if (this != &other) {
            _splice(pos, other, other.begin(), other.end(), other.size());
        }
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list && other)
    {
        splice(pos, other);
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list & other, const_iterator it)
    {
        if (it == other.end()) {
            throw std::out_of_range("list splice out of range");
        } else {
            const_iterator next = std::next(it);
            if (this != &other || (it != pos && it != next)) {
                // 如果不是同一个链表，或者pos不在[it, next)范围内，可以转移
                _splice(pos, other, it, next, 1);
            }
        }
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list && other, const_iterator it)
    {
        splice(pos, other, it);
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list & other, const_iterator first, const_iterator last)
    {
        if (first != last && (this != &other || pos == last)) {
            // [first, last)有效，且(不是同一链表，或者是同一链表但last不在pos，如果在的话不需要移动)
            size_type count = 0;
            if (this != &other) {
                if (first == other.begin() && last == other.end()) {
                    // 移动整个other
                    count = other.size();
                } else {
                    for (const_iterator it=first; it!=last; ++it, ++count) {
                        if (it == other.end()) {
                            throw std::length_error("list bad splice");
                        }
                    }
                }
            }
            // 如果是同一个链表，长度不变
            _splice(pos, other, first, last, count);
        }
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void splice(const_iterator pos, list && other, const_iterator first, const_iterator last)
    {
        splice(pos, other, first, last);
    }

    /**
     * @brief 移除满足特定标准的元素
     */
    void remove(const value_type & value)
    {
        remove_if([value](value_type x) {
            return x == value;
        });
    }

    /**
     * @brief 移除满足特定标准的元素
     */
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate p)
    {
        for (iterator it = begin(); it!=end(); ) {
            if (p(*it)) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }

    /**
     * @brief 反转元素的顺序
     */
    void reverse() noexcept
    {
        node_pointer cur = _finish->_next;
        node_pointer next = nullptr;
        
        while (cur != _finish) {
            next = cur->_next;
            cur->_next = cur->_prev;
            cur->_prev = next;
            cur = next;
        }

        // 反转虚拟尾节点
        next = _finish->_next;
        _finish->_next = _finish->_prev;
        _finish->_prev = next;
    }

    /**
     * @brief 删除连续的重复元素
     */
    void unique()
    {
        unique(std::equal_to<value_type>());
    }

    /**
     * @brief 删除连续的重复元素
     */
    template <class BinaryPredicate>
    void unique(BinaryPredicate p)
    {
        node_pointer cur = _finish->_next;
        node_pointer next = cur->_next;

        while (next != _finish) {
            if (p(cur->_data, next->_data)) {
                // 重复了，需要删除
                node_pointer to_del = next;
                next = next->_next;
                // 销毁节点
                _destroy_node(to_del);
                // 重新设置指针
                cur->_next = next;
                next->_prev = cur; 
                // 修改大小
                --_size;
            } else {
                // 不重复
                cur = next;
                next = next->_next;
            }
        }
    }

    /**
     * @brief 对元素进行排序
     */
    void sort()
    {
        sort(std::less<value_type>());
    }

    /**
     * @brief 对元素进行排序
     * @details 归并排序
     */
    template <class Compare>
    void sort(Compare comp)
    {
        _sort(begin(), end(), comp, size());
    }

public:
    /**
     * @brief 申请一个节点的空间
     */
    node_pointer _get_node()
    {
        return wwstl::allocator_traits<node_allocator_type>::allocate(_node_allocator, 1);
    }

    /**
     * @brief 释放一个节点
     */
    void _put_node(node_pointer p)
    {
        wwstl::allocator_traits<node_allocator_type>::deallocate(_node_allocator, p, 1);
    }

    /**
     * @brief 创建一个节点
     */
    template <class... Arg>
    node_pointer _create_node(Arg&&... args)
    {
        node_pointer p = _get_node();
        wwstl::allocator_traits<node_allocator_type>::construct(_node_allocator, p, std::forward<Arg>(args)...);
        return p;
    }

    /**
     * @brief 销毁一个节点
     */
    void _destroy_node(node_pointer p)
    {
        wwstl::allocator_traits<node_allocator_type>::destroy(_node_allocator, p);
        _put_node(p);
    }

    /**
     * @brief 初始化链表
     */
    void _init_list()
    {
        _finish = _create_node(value_type());
        _finish->_prev = _finish;
        _finish->_next = _finish;
    }

    /**
     * @brief 从另一个list中移动元素
     */
    void _splice(const_iterator pos, list & other, const_iterator first, const_iterator last, size_type count)
    {
        // 此时可以认为所有的参数条件都是合法的
        // 将first前节点的next连接到last
        first._node->_prev->_next = last._node;
        // 将last前节点的next连接到pos
        last._node->_prev->_next = pos._node;
        // 将pos前节点的next连接到first
        pos._node->_prev->_next = first._node;

        // 保存first前节点，因为马上就需要修改first的prev指针
        node_pointer p = first._node->_prev;
        // 将first的prev连接到pos的前节点
        first._node->_prev = pos._node->_prev;
        // 将pos的prev连接到last的前节点
        pos._node->_prev = last._node->_prev;
        // 将last的前节点连接到first的前节点
        last._node->_prev = p;

        // 修改链表大小
        _size += count;
        other._size -= count;
    }

    /**
     * @brief 排序辅助函数
     */
    template <class Compare>
    iterator _sort(iterator first, iterator last, Compare comp, size_type count)
    {
        if (count < 2)
            return first;

        iterator mid = std::next(first, static_cast<difference_type>(count >> 1));
        // 递归
        first = _sort(first, mid, comp, count >> 1);
        mid = _sort(mid, last, comp, count - (count >> 1));
        
        // 合并有序链表
        iterator new_first = first;
        // 使用循环的运行逻辑，使得is_init初次运行后变为false
        for (bool is_init = true; ; is_init = false) {
            if (comp(*mid, *first)) {
                // 需要从mid移动到first
                if (is_init) {
                    new_first = mid;
                }
                splice(first, *this, mid++);
                if (mid == last) {
                    // [mid, last)消耗完毕
                    return new_first;
                }
            } else {
                ++first;
                if (first == mid) {
                    // [first, mid)消耗完毕
                    return new_first;
                }
            }
        }
    }

    [[noreturn]] void _throw_out_of_range() const
    {
        throw std::out_of_range("invalid list<T> subscript");
    }
};

// 非成员函数

template <
    class T,
    class Alloc
> bool operator==(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <
    class T,
    class Alloc
> bool operator!=(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return !(lhs == rhs);
}

template <
    class T,
    class Alloc
> bool operator<(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <
    class T,
    class Alloc
> bool operator>(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return rhs < lhs;
}

template <
    class T,
    class Alloc
> bool operator<=(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return !(rhs < lhs);
}

template <
    class T,
    class Alloc
> bool operator>=(const list<T, Alloc> & lhs, const list<T, Alloc> & rhs)
{
    return !(lhs < rhs);
}

template <
    class T,
    class Alloc
> void swap(list<T, Alloc> & lhs, list<T, Alloc> & rhs)
{
    lhs.swap(rhs);
}

} // namespace wwstl

#endif // __WW_LIST_H__