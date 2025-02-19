#ifndef __WW_SET_H__
#define __WW_SET_H__

#include "ww_rb_tree.h"

namespace wwstl
{

/**
 * @brief set
 * @link https://zh.cppreference.com/w/cpp/container/set
 */
template <
    class Key,
    class Compare = std::less<Key>,
    class Allocator = wwstl::allocator<Key>
> class set
{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator>::iterator;
    using const_iterator = typename wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator>::const_iterator;
    using reverse_iterator = typename wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = typename wwstl::reverse_iterator<const_iterator>;

public:
    wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator> _tree;     // 红黑树

public:
    set()
        : set(Compare())
    {
    }

    explicit set(const Compare & comp, const Allocator & alloc = Allocator())
        : _tree(comp, alloc)
    {
    }

    explicit set(const Allocator & alloc)
        : _tree(alloc)
    {
    }

    template <class InputIt>
    set(InputIt first, InputIt last, const Compare & comp = Compare(), const Allocator & alloc = Allocator())
        : _tree(first, last, comp, alloc)
    {
    }

    set(const set & other)
        : _tree(other._tree)
    {
    }

    set(const set & other, const Allocator & alloc)
        : _tree(other._tree, alloc)
    {
    }

    set(set && other)
        : _tree(std::move(other._tree))
    {
    }

    set(set && other, const Allocator & alloc)
        : _tree(std::move(other._tree), alloc)
    {
    }

    set(std::initializer_list<value_type> init, const Compare & comp = Compare(), const Allocator & alloc = Allocator())
        : _tree(comp, alloc)
    {
        insert(init);
    }

public:
    /**
     * @brief 将值赋给容器
     */
    set & operator=(const set & other)
    {
        if (this != &other) {
            _tree = other._tree;
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    set & operator=(set && other)
    {
        if (this != &other) {
            _tree = std::move(other._tree);
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    set & operator=(std::initializer_list<value_type> & ilist)
    {
        clear();
        insert(ilist);
        return *this;
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const
    {
        return _tree.get_allocator();
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return _tree.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return _tree.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return _tree.cbegin();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return _tree.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return _tree.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return _tree.cend();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    reverse_iterator rbegin() noexcept
    {
        return _tree.rbegin();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator rbegin() const noexcept
    {
        return _tree.rbegin();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator crbegin() const noexcept
    {
        return _tree.crbegin();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    reverse_iterator rend() noexcept
    {
        return _tree.rend();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator rend() const noexcept
    {
        return _tree.rend();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator crend() const noexcept
    {
        return _tree.crend();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _tree.empty();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _tree.size();
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return _tree.max_size();
    }

    // 修改器

    /**
     * @brief 清除内容
     */
    void clear() noexcept
    {
        _tree.clear();
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(const value_type & value)
    {
        return _tree.emplace_unique(value);
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(value_type && value)
    {
        return _tree.emplace_unique(std::move(value));
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, const value_type & value)
    {
        return _tree.emplace_hint_unique(hint, value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, value_type && value)
    {
        return _tree.emplace_hint_unique(hint, std::move(value));
    }

    /**
     * @brief 插入元素
     */
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
        {
            _tree.emplace_unique(*first);
        }
    }

    /**
     * @brief 插入元素
     */
    void insert(std::initializer_list<value_type> ilist)
    {
        insert(ilist.begin(), ilist.end());
    }

    /**
     * @brief 原位构造元素
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&... args)
    {
        return _tree.emplace_unique(std::forward<Args>(args)...);
    }

    /**
     * @brief 使用提示原位构造元素
     */
    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args &&... args)
    {
        return _tree.emplace_hint_unique(hint, std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        return _tree.erase(pos);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        return _tree.erase(first, last);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key)
    {
        return _tree.erase(key);
    }

    /**
     * @brief 交换内容
     */
    void swap(set & other)
    {
        _tree.swap(other._tree);
    }

    // 查找

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type & key) const
    {
        return _tree.count(key);
    }

    /**
     * @brief 寻找带有特定键的元素
     */
    iterator find(const key_type & key)
    {
        return _tree.find(key);
    }

    /**
     * @brief 寻找带有特定键的元素
     */
    const_iterator find(const key_type & key) const
    {
        return _tree.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        return _tree.equal_range(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        return _tree.equal_range(key);
    }

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    iterator lower_bound(const key_type & key)
    {
        return _tree.lower_bound(key);
    }

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    const_iterator lower_bound(const key_type & key) const
    {
        return _tree.lower_bound(key);
    }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    iterator upper_bound(const key_type & key)
    {
        return _tree.upper_bound(key);
    }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    const_iterator upper_bound(const key_type & key) const
    {
        return _tree.upper_bound(key);
    }

    // 观察器

    /**
     * @brief 返回用于比较键的函数
     */
    key_compare key_comp() const
    {
        return key_compare();
    }

    /**
     * @brief 返回用于比较 value_type 类型的对象中的键的函数
     */
    value_compare value_comp() const
    {
        return value_compare();
    }
};

// 非成员函数

template <
    class Key,
    class Compare,
    class Allocator
> void swap(set<Key, Compare, Allocator> & lhs,
            set<Key, Compare, Allocator> & rhs)
{ lhs.swap(rhs);
    }

/**
 * @brief multiset
 * @link https://zh.cppreference.com/w/cpp/container/multiset
 */
template <
    class Key,
    class Compare = std::less<Key>,
    class Allocator = wwstl::allocator<Key>
> class multiset
{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = typename wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator>::iterator;
    using const_iterator = typename wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator>::const_iterator;
    using reverse_iterator = typename wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = typename wwstl::reverse_iterator<const_iterator>;

public:
    wwstl::rb_tree<key_type, void, value_type, keyExtractor<value_type>, value_compare, Allocator> _tree;     // 红黑树

public:
    multiset()
        : multiset(Compare())
    {
    }

    explicit multiset(const Compare & comp, const Allocator & alloc = Allocator())
        : _tree(comp, alloc)
    {
    }

    explicit multiset(const Allocator & alloc)
        : _tree(alloc)
    {
    }

    template <class InputIt>
    multiset(InputIt first, InputIt last, const Compare & comp = Compare(), const Allocator & alloc = Allocator())
        : _tree(first, last, comp, alloc)
    {
    }

    multiset(const multiset & other)
        : _tree(other._tree)
    {
    }

    multiset(const multiset & other, const Allocator & alloc)
        : _tree(other._tree, alloc)
    {
    }

    multiset(multiset && other)
        : _tree(std::move(other._tree))
    {
    }

    multiset(multiset && other, const Allocator & alloc)
        : _tree(std::move(other._tree), alloc)
    {
    }

    multiset(std::initializer_list<value_type> init, const Compare & comp = Compare(), const Allocator & alloc = Allocator())
        : _tree(comp, alloc)
    {
        insert(init);
    }

public:
    /**
     * @brief 将值赋给容器
     */
    multiset & operator=(const multiset & other)
    {
        if (this != &other) {
            _tree = other._tree;
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    multiset & operator=(multiset && other)
    {
        if (this != &other) {
            _tree = std::move(other._tree);
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    multiset & operator=(std::initializer_list<value_type> & ilist)
    {
        clear();
        insert(ilist);
        return *this;
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const
    {
        return _tree.get_allocator();
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return _tree.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return _tree.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return _tree.cbegin();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return _tree.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return _tree.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return _tree.cend();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    reverse_iterator rbegin() noexcept
    {
        return _tree.rbegin();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator rbegin() const noexcept
    {
        return _tree.rbegin();
    }

    /**
     * @brief 返回指向起始的逆向迭代器
     */
    const_reverse_iterator crbegin() const noexcept
    {
        return _tree.crbegin();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    reverse_iterator rend() noexcept
    {
        return _tree.rend();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator rend() const noexcept
    {
        return _tree.rend();
    }

    /**
     * @brief 返回指向末尾的逆向迭代器
     */
    const_reverse_iterator crend() const noexcept
    {
        return _tree.crend();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _tree.empty();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _tree.size();
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return _tree.max_size();
    }

    // 修改器

    /**
     * @brief 清除内容
     */
    void clear() noexcept
    {
        _tree.clear();
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const value_type & value)
    {
        return _tree.emplace_equal(value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(value_type && value)
    {
        return _tree.emplace_equal(std::move(value));
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, const value_type & value)
    {
        return _tree.emplace_hint_equal(hint, value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, value_type && value)
    {
        return _tree.emplace_hint_equal(hint, std::move(value));
    }

    /**
     * @brief 插入元素
     */
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
        {
            _tree.emplace_equal(*first);
        }
    }

    /**
     * @brief 插入元素
     */
    void insert(std::initializer_list<value_type> ilist)
    {
        insert(ilist.begin(), ilist.end());
    }

    /**
     * @brief 原位构造元素
     */
    template <class... Args>
    iterator emplace(Args &&... args)
    {
        return _tree.emplace_equal(std::forward<Args>(args)...);
    }

    /**
     * @brief 使用提示原位构造元素
     */
    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args &&... args)
    {
        return _tree.emplace_hint_equal(hint, std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        return _tree.erase(pos);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        return _tree.erase(first, last);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key)
    {
        return _tree.erase(key);
    }

    /**
     * @brief 交换内容
     */
    void swap(multiset & other)
    {
        _tree.swap(other._tree);
    }

    // 查找

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type & key) const
    {
        return _tree.count(key);
    }

    /**
     * @brief 寻找带有特定键的元素
     */
    iterator find(const key_type & key)
    {
        return _tree.find(key);
    }

    /**
     * @brief 寻找带有特定键的元素
     */
    const_iterator find(const key_type & key) const
    {
        return _tree.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        return _tree.equal_range(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        return _tree.equal_range(key);
    }

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    iterator lower_bound(const key_type & key)
    {
        return _tree.lower_bound(key);
    }

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    const_iterator lower_bound(const key_type & key) const
    {
        return _tree.lower_bound(key);
    }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    iterator upper_bound(const key_type & key)
    {
        return _tree.upper_bound(key);
    }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    const_iterator upper_bound(const key_type & key) const
    {
        return _tree.upper_bound(key);
    }

    // 观察器

    /**
     * @brief 返回用于比较键的函数
     */
    key_compare key_comp() const
    {
        return _tree.key_comp();
    }
};

// 非成员函数

template <
    class Key,
    class Compare,
    class Allocator
> void swap(multiset<Key, Compare, Allocator> & lhs,
            multiset<Key, Compare, Allocator> & rhs)
{
    lhs.swap(rhs);
}

} // namespace wwstl

#endif // __WW_SET_H__