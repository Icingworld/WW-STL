#ifndef __WW_FORWARD_LIST_H__
#define __WW_FORWARD_LIST_H__

#include <initializer_list>
#include "ww_type_traits.h"
#include "ww_memory.h"

namespace wwstl
{

template <class T>
class _forward_list_node
{
public:
    using value_type = T;
    using node_pointer = _forward_list_node<value_type>*;

public:
    node_pointer _next;     // 下一个节点
    value_type _data;       // 节点数据

public:
    _forward_list_node()
        : _next(nullptr), _data()
    { // 构造空节点
    }

    explicit _forward_list_node(const value_type & data)
        : _next(nullptr), _data(data)
    { // 以值构造节点
    }
};

template <class T>
class _forward_list_const_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using self = _forward_list_const_iterator<value_type>;
    using node_pointer = _forward_list_node<value_type>*;

public:
    node_pointer _node;     // 当前节点

public:
    explicit _forward_list_const_iterator(const node_pointer node)
        : _node(const_cast<node_pointer>(node))
    { // 以节点构造迭代器
    }

public:
    bool operator==(const self & other) const
    { return _node == other._node; }

    bool operator!=(const self & other) const
    { return !(*this == other); }

    reference operator*() const
    { return _node->_data; }

    pointer operator->() const
    { return &(operator*()); }

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
};

template <class T>
class _forward_list_iterator
    : public _forward_list_const_iterator<T>
{
public:
    using base = _forward_list_const_iterator<T>;
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;
    using self = _forward_list_iterator<value_type>;
    using node_pointer = _forward_list_node<value_type>*;

public:
    explicit _forward_list_iterator(node_pointer node)
        : base(node)
    { // 以节点构造迭代器
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
};

/**
 * @brief forward_list
 * @link https://zh.cppreference.com/w/cpp/container/forward_list
 */
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class forward_list
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = _forward_list_iterator<value_type>;
    using const_iterator = _forward_list_const_iterator<value_type>;

    using node_type = _forward_list_node<value_type>;
    using node_pointer = node_type*;
    using node_allocator_type = typename Allocator::template rebind<node_type>::other;

public:
    node_pointer _start;                    // 指向头节点之前的虚拟节点
    node_allocator_type _node_allocator;    // 节点分配器

public:
    forward_list()
        : forward_list(Allocator())
    {
    }

    explicit forward_list(const Allocator & alloc)
        : _start(nullptr), _node_allocator(alloc)
    { _init_forward_list(); }

    explicit forward_list(size_type count, const Allocator & alloc = Allocator())
        : _start(nullptr), _node_allocator(alloc)
    {
        _init_forward_list();
        _init_with_n(count);
    }

    forward_list(size_type count, const value_type & value, const Allocator & alloc = Allocator())
        : _start(nullptr), _node_allocator(alloc)
    {
        _init_forward_list();
        _init_with_n(count, value);
    }

    template <
        class InputIt,
        class = std::enable_if_t<wwstl::is_iterator_v<InputIt>>
    > forward_list(InputIt first, InputIt last, const Allocator & alloc = Allocator())
        : _start(nullptr), _node_allocator(alloc)
    {
        _init_forward_list();
        _init_with_range(first, last);
    }

    forward_list(const forward_list & other)
        : _start(nullptr), _node_allocator(other._node_allocator)
    {
        _init_forward_list();
        assign(other.begin(), other.end());
    }

    forward_list(forward_list && other)
        : _start(other._start), _node_allocator(std::move(other._node_allocator))
    {
        _start = other._start;
        other._start = nullptr;
        other._init_forward_list();
    }

    forward_list(std::initializer_list<value_type> init, const Allocator & alloc = Allocator())
        : _start(nullptr), _node_allocator(alloc)
    {
        _init_forward_list();
        assign(init);
    }

public:
    /**
     * @brief 将值赋给容器
     */
    forward_list & operator=(const forward_list & other)
    {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    forward_list & operator=(forward_list && other)
    {
        if (this != &other) {
            clear();
            _node_allocator = std::move(other._node_allocator);
            _start = other._start;
            other._init_forward_list();
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    forward_list & operator=(std::initializer_list<value_type> ilist)
    {
        assign(ilist.begin(), ilist.end());
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(size_type count, const value_type & value)
    {
        clear();
        insert_after(begin_before(), count, value);
    }

    /**
     * @brief 将值赋给容器
     */
    template <class InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear();
        insert_after(begin_before(), first, last);
    }

    /**
     * @brief 将值赋给容器
     */
    void assign(std::initializer_list<value_type> ilist)
    { assign(ilist.begin(), ilist.end()); }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const noexcept
    { return _node_allocator; }

    // 元素访问

    /**
     * @brief 访问第一个元素
     */
    reference front()
    { return *begin(); }

    /**
     * @brief 访问第一个元素
     */
    const_reference front() const
    { return *begin(); }

    // 迭代器

    /**
     * @brief 返回指向容器开头之前的迭代器
     */
    iterator begin_before() noexcept
    { return iterator(_start); }

    /**
     * @brief 返回指向容器开头之前的迭代器
     */
    const_iterator begin_before() const noexcept
    { return const_iterator(_start); }

    /**
     * @brief 返回指向容器开头之前的迭代器
     */
    const_iterator cbefore_begin() const noexcept
    { return const_iterator(_start); }

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    { return iterator(_start->_next); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    { return const_iterator(_start->_next); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    { return begin(); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    { return iterator(nullptr); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    { return const_iterator(nullptr); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    { return end(); }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    { return begin() == end(); }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    { return std::numeric_limits<difference_type>::max(); }

    // 修改器

    /**
     * @brief 清除内容
     */
    void clear() noexcept
    {
        for (node_pointer p = _start->_next; p != nullptr; ) {
            node_pointer next = p->_next;
            _destroy_node(p);
            p = next;
        }
        _start->_next = nullptr;
    }

    /**
     * @brief 在某个元素后插入新元素
     */
    iterator insert_after(const_iterator pos, const value_type & value)
    { return emplace_after(pos, value); }

    /**
     * @brief 在某个元素后插入新元素
     */
    iterator insert_after(const_iterator pos, value_type && value)
    { return emplace_after(pos, std::move(value)); }

    /**
     * @brief 在某个元素后插入新元素
     */
    iterator insert_after(const_iterator pos, size_type count, const value_type & value)
    {
        if (count == 0)
            return pos;
        
        forward_list tmp(count, value);
        return _splice_after(pos, tmp, tmp.begin_before(), tmp.end());
    }

    /**
     * @brief 在某个元素后插入新元素
     */
    template<class InputIt>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
            return iterator(pos._node);
        
        forward_list tmp(first, last);
        return _splice_after(iterator(pos._node), tmp, tmp.begin_before(), tmp.end());
    }

    /**
     * @brief 在某个元素后插入新元素
     */
    iterator insert_after(const_iterator pos, std::initializer_list<value_type> ilist)
    { return insert_after(pos, ilist.begin(), ilist.end()); }

    /**
     * @brief 在元素后原位构造元素
     */
    template <class... Arg>
    iterator emplace_after(const_iterator pos, Arg&&... args)
    { return _emplace(pos, std::forward<Arg>(args)...); }

    /**
     * @brief 擦除元素后的元素
     */
    iterator erase_after(const_iterator pos)
    {
        if (pos._node->_next == nullptr)
            return end();
        
        return erase_after(pos, std::next(pos, 2)); }

    /**
     * @brief 擦除元素后的元素
     */
    iterator erase_after(const_iterator first, const_iterator last)
    {
        node_pointer to_del = first._node->_next;
        _destroy_node(to_del);
        first._node->_next = last._node;
        return iterator(last._node);
    }

    /**
     * @brief 插入元素到容器起始
     */
    void push_front(const value_type & value)
    { emplace_front(value); }

    /**
     * @brief 插入元素到容器起始
     */
    void push_front(value_type && value)
    { emplace_front(std::move(value)); }

    /**
     * @brief 在容器头部原位构造元素
     */
    template <class... Arg>
    void emplace_front(Arg&&... args)
    { _emplace(begin_before(), std::forward<Arg>(args)...); }

    /**
     * @brief 移除首元素
     */
    void pop_front()
    { erase_after(begin_before()); }

    /**
     * @brief 交换内容
     */
    void swap(forward_list & other)
    {
        std::swap(_start, other._start);
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value){
            std::swap(_node_allocator, other._node_allocator);
        }
    }

    // 操作

    /**
     * @brief 合并两个有序列表
     */
    void merge(forward_list & other)
    { merge(other, std::less<value_type>()); }

    /**
     * @brief 合并两个有序列表
     */
    void merge(forward_list && other)
    { merge(other); }

    /**
     * @brief 合并两个有序列表
     */
    template <class Compare>
    void merge(forward_list & other, const Compare & comp)
    {
        if (this == &other)
            return;

        node_pointer this_prev = _start;
        node_pointer this_cur = _start->_next;
        node_pointer other_prev = other._start;
        node_pointer other_cur = other._start->_next;

        while (this_cur != nullptr && other_cur != nullptr) {
            if (comp(this_cur->_data, other_cur->_data)) {
                // 不需要交换
                this_prev = this_cur;
                this_cur = this_cur->_next;
            } else {
                // 需要交换，把other_cur插入到this_prev后面
                node_pointer other_next = other_cur->_next;
                iterator new_prev = _splice_after(iterator(this_prev), other, iterator(other_prev), iterator(other_next));
                this_prev = new_prev._node;     // 交换后，this_prev指向交换后的节点
                other_cur = other_next;
            }
        }

        if (other_cur != nullptr) {
            // other剩下的元素都要插入到this的尾部
            _splice_after(iterator(this_prev), other, other.begin_before(), other.end());
        }
    }

    /**
     * @brief 合并两个有序列表
     */
    template <class Compare>
    void merge(forward_list && other, const Compare & comp)
    { merge(other, comp); }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list & other)
    {
        if (this != &other) {
            _splice_after(iterator(pos._node), other, other.begin_before(), other.end());
        }
    }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list && other)
    { splice_after(pos, other); }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list & other, const_iterator it)
    {
        // 关于判断合法性，到底是比较node == nullptr还是it == end()呢
        if (it == other.end() || std::next(it) == other.end()) {
            throw std::out_of_range("forward_list splice_after out of range");
        } else {
            const_iterator next = std::next(it);
            if (this != &other || (pos != it && pos != next)) {
                _splice_after(pos, other, it, ++next);
            }
        }
    }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list && other, const_iterator it)
    { splice_after(pos, other, it); }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list & other, const_iterator first, const_iterator last)
    {
        if (first == last || first._node->_next == last._node)
            return;

        if (this != &other || (pos != first && pos._node->_next != first._node->_next)) {
            _splice_after(pos, other, first, last);
        }
    }

    /**
     * @brief 从另一forward_list移动元素
     */
    void splice_after(const_iterator pos, forward_list && other, const_iterator first, const_iterator last)
    { _splice_after(pos, other, first, last); }

    /**
     * @brief 移除满足特定标准的元素
     */
    void remove(const value_type & value)
    {
        remove_if([value](int x) {
            return x == value;
        });
    }

    /**
     * @brief 移除满足特定标准的元素
     */
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate p)
    {
        iterator prev = begin_before();
        iterator cur = begin();

        while (cur != end()) {
            if (p(*cur)) {
                // 需要移除
                cur = erase_after(prev);
            } else {
                // 不需要移除，继续遍历
                ++prev;
                ++cur;
            }
        }
    }

    /**
     * @brief 反转元素的顺序
     */
    void reverse() noexcept
    {
        if (_start->_next == nullptr || _start->_next->_next == nullptr) {
            // 空链表或只有一个节点，直接返回
            return;
        }

        node_pointer prev = _start;
        node_pointer cur = _start->_next;

        while (cur != nullptr) {
            node_pointer next = cur->_next;
            cur->_next = prev;
            prev = cur;
            cur = next;
        }

        // 设置第一个节点的next为nullptr
        _start->_next->_next = nullptr;
        // 反转虚拟头节点
        _start->_next = prev;
    }

    /**
     * @brief 删除连续的重复元素
     */
    void unique()
    { unique(std::equal_to<value_type>()); }

    /**
     * @brief 删除连续的重复元素
     */
    template <class BinaryPredicate>
    void unique(BinaryPredicate p)
    {
        if (_start->_next == nullptr || _start->_next->_next == nullptr)
            return;

        iterator prev = begin();
        iterator cur = std::next(prev);

        while (cur != end()) {
            if (p(*prev, *cur)) {
                // 需要移除
                cur = erase_after(prev);
            } else {
                // 不需要移除，继续遍历
                ++prev;
                ++cur;
            }
        }
    }

    /**
     * @brief 对元素进行排序
     */
    void sort()
    { sort(std::less<value_type>()); }

    /**
     * @brief 对元素进行排序
     * @details 归并排序
     */
    template <class Compare>
    void sort(Compare comp)
    { _sort(begin_before(), end(), comp, std::distance(begin(), end())); }

public:
    /**
     * @brief 申请一个节点的空间
     */
    node_pointer _get_node()
    { return _node_allocator.allocate(1); }

    /**
     * @brief 释放一个节点
     */
    void _put_node(node_pointer p)
    { _node_allocator.deallocate(p, 1); }

    /**
     * @brief 创建一个节点
     */
    template <class... Arg>
    node_pointer _create_node(Arg&&... args)
    {
        node_pointer p = _get_node();
        _node_allocator.construct(p, std::forward<Arg>(args)...);
        return p;
    }

    /**
     * @brief 销毁一个节点
     */
    void _destroy_node(node_pointer p)
    {
        _node_allocator.destroy(p);
        _put_node(p);
    }

    /**
     * @brief 初始化虚拟头节点
     */
    void _init_forward_list()
    {
        _start = _create_node(value_type());
        _start->_next = nullptr;
    }

    void _init_with_n(size_type count, const value_type & value = value_type())
    {
        for (size_type i=0; i<count; ++i) {
            push_front(value);
        }
    }

    template <class InputIt>
    void _init_with_range(InputIt first, InputIt last)
    {
        iterator cur = begin_before();
        for (; first != last; ++first) {
            cur = insert_after(cur, *first);
        }
    }

    /**
     * @brief 插入元素辅助函数
     * @details 在pos之后插入个元素
     */
    template <typename... Arg>
    iterator _emplace(const_iterator pos, Arg&&... args)
    {
        node_pointer tmp = _create_node(std::forward<Arg>(args)...);
        tmp->_next = pos._node->_next;
        pos._node->_next = tmp;
        return iterator(tmp);
    }

    /**
     * @brief 移动链表辅助函数
     * @details 移动(first, last)到pos之后
     * @return 返回最后一个被移动的节点，该返回值在merge中使用
     */
    iterator _splice_after(const_iterator pos, forward_list & other, const_iterator first, const_iterator last)
    {
        // 寻找last的前节点
        node_pointer last_prev = first._node;
        while (last_prev->_next != last._node) {
            last_prev = last_prev->_next;
        }
        // 暂存pos的下一个节点
        node_pointer pos_next = pos._node->_next;
        // 连接pos.next到first的下一个节点
        pos._node->_next = first._node->_next;
        // 连接last_prev的next到pos_next
        last_prev->_next = pos_next;
        // 把first和last连接起来
        first._node->_next = last._node;
        // 返回最后一个节点
        return iterator(last_prev);
    }

    /**
     * @brief 排序辅助函数
     * @details MSVC的实现，比gcc的实现要简单
     */
    template <class Compare>
    void _sort(iterator before_first, iterator last, Compare comp, size_type count)
    {
        if (count < 2)
            return;

        iterator mid = std::next(before_first, static_cast<difference_type>(1 + count >> 1));
        _sort(before_first, mid, comp, count >> 1);
        iterator first = std::next(before_first);   // first指向第一个元素

        iterator before_mid = std::next(before_first, static_cast<difference_type>(count >> 1));
        _sort(before_mid, last, comp, count - (count >> 1));
        mid = std::next(before_mid);   // mid指向中间元素

        for (; ;) {
			if (comp(*mid, *first)) {
				splice_after(before_first, *this, before_mid);
				++before_first;
				mid = std::next(before_mid);
				if (mid == last)
					return;
            } else {
				++before_first;
				++first;
				if (first == mid)
					return;
            }
        }
    }

};

// 非成员函数

template <
    class T,
    class Alloc
> bool operator==(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return std::equal(lhs.begin(), lhs.end(), rhs.begin()); }

template <
    class T,
    class Alloc
> bool operator!=(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return !(lhs == rhs); }

template <
    class T,
    class Alloc
> bool operator<(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

template <
    class T,
    class Alloc
> bool operator>(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return rhs < lhs; }

template <
    class T,
    class Alloc
> bool operator<=(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return !(rhs < lhs); }

template <
    class T,
    class Alloc
> bool operator>=(const forward_list<T, Alloc> & lhs, const forward_list<T, Alloc> & rhs)
{ return !(lhs < rhs); }

template <
    class T,
    class Alloc
> void swap(forward_list<T, Alloc> & lhs, forward_list<T, Alloc> & rhs)
{ lhs.swap(rhs); }

} // namespace wwstl

#endif // __WW_FORWARD_LIST_H__