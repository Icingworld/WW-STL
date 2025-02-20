#ifndef __WW_UNORDERED_SET_H__
#define __WW_UNORDERED_SET_H__

#include "ww_hashtable.h"

namespace wwstl
{

/**
 * @brief unordered_set
 * @link https://zh.cppreference.com/w/cpp/container/unordered_set
 */
template <
    class Key,
    class Hash = wwstl::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = wwstl::allocator<Key>
> class unordered_set
{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hashtable_type = wwstl::hashtable<key_type, void, value_type, hasher, key_equal, wwstl::keyExtractor<value_type>, allocator_type>;
    using iterator = typename hashtable_type::iterator;
    using const_iterator = typename hashtable_type::const_iterator;
    using local_iterator = typename hashtable_type::local_iterator;
    using const_local_iterator = typename hashtable_type::const_local_iterator;

public:
    hashtable_type _ht;     // 哈希表

public:
    unordered_set()
        : unordered_set(size_type(8))
    {
    }

    explicit unordered_set(size_type bucket_count, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
    }

    explicit unordered_set(const allocator_type & alloc)
        : _ht(alloc)
    {
    }

    template <class InputIt>
    unordered_set(InputIt first, InputIt last, size_type bucket_count = 8, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
        insert(first, last);
    }

    unordered_set(const unordered_set & other)
        : _ht(other._ht)
    {
    }

    unordered_set(unordered_set && other)
        : _ht(std::move(other._ht))
    {
    }

    unordered_set(const unordered_set & other, const allocator_type & alloc)
        : _ht(other._ht, alloc)
    {
    }

    unordered_set(unordered_set && other, const allocator_type & alloc)
        : _ht(std::move(other._ht), alloc)
    {
    }

    unordered_set(std::initializer_list<value_type> ilist, size_type bucket_count = 8, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
        insert(ilist);
    }

    ~unordered_set() = default;

public:
    /**
     * @brief 将值赋给容器
     */
    unordered_set & operator=(const unordered_set & other)
    {
        _ht = other._ht;
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    unordered_set & operator=(unordered_set && other)
    {
        _ht = std::move(other._ht);
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    unordered_set & operator=(std::initializer_list<value_type> ilist)
    {
        clear();
        insert(ilist);
        return *this;
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const noexcept
    {
        return _ht.get_allocator();
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return _ht.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return _ht.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return _ht.cbegin();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return _ht.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return _ht.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return _ht.cend();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _ht.empty();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _ht.size();
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return _ht.max_size();
    }

    // 修改器

    /**
     * @brief 清除元素
     */
    void clear() noexcept
    {
        _ht.clear();
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(const value_type & value)
    {
        return _ht.emplace_unique(value);
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(value_type && value)
    {
        return _ht.emplace_unique(std::move(value));
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(const_iterator hint, const value_type & value)
    {
        return _ht.emplace_hint_unique(hint, value);
    }

    /**
     * @brief 插入元素
     */
    std::pair<iterator, bool> insert(const_iterator hint, value_type && value)
    {
        return _ht.emplace_hint_unique(hint, std::move(value));
    }

    /**
     * @brief 插入元素
     */
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
        {
            _ht.emplace_unique(*first);
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
     * @details 调用emplace_unique，不允许重复
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        return _ht.emplace_unique(std::forward<Args>(args)...);
    }

    /**
     * @brief 使用提示原位构造元素
     */
    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return _ht.emplace_hint_unique(hint, std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        return _ht.erase(pos);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        return _ht.erase(first, last);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key)
    {
        return _ht.erase(key);
    }

    /**
     * @brief 交换内容
     */
    void swap(unordered_set & other) noexcept
    {
        _ht.swap(other._ht);
    }

    // 查找

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type & key) const
    {
        return _ht.count(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    iterator find(const key_type & key)
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    const_iterator find(const key_type & key) const
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        return _ht.equal_range(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        return _ht.equal_range(key);
    }

    // 桶接口

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    local_iterator begin(size_type n)
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator begin(size_type n) const
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator cbegin(size_type n) const
    {
        return _ht.cbegin(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    local_iterator end(size_type n)
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator end(size_type n) const
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator cend(size_type n) const
    {
        return _ht.cend(n);
    }

    /**
     * @brief 返回桶数
     */
    size_type bucket_count() const
    {
        return _ht.bucket_count();
    }

    /**
     * @brief 返回桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _ht.max_bucket_count();
    }

    /**
     * @brief 返回特定的桶中的元素数量
     */
    size_type bucket_size(size_type n) const
    {
        return _ht.bucket_size(n);
    }

    /**
     * @brief 返回带有特定键的桶
     */
    size_type bucket(const key_type& key) const
    {
        return _ht.bucket(key);
    }

    // 散列策略

    /**
     * @brief 负载因子
     */
    float load_factor() const
    {
        return _ht.load_factor();
    }

    /**
     * @brief 最大负载因子
     */
    float max_load_factor() const
    {
        return _ht.max_load_factor();
    }

    /**
     * @brief 设置最大负载因子
     */
    void max_load_factor(float ml)
    {
        _ht.max_load_factor(ml);
    }

    /**
     * @brief 预留至少指定数量的桶并重新生成散列表
     */
    void rehash(size_type count)
    {
        _ht.rehash(count);
    }

    /**
     * @brief 为至少指定数量的元素预留空间并重新生成散列表
     */
    void reserve(size_type count)
    {
        _ht.reserve(count);
    }

    // 观察器

    /**
     * @brief 返回用于对键求散列的函数
     */
    hasher hash_function() const
    {
        return _ht.hash_function();
    }

    /**
     * @brief 返回用于比较键的相等性的函数
     */
    key_equal key_eq() const
    {
        return _ht.key_eq();
    }
};

// 非成员函数

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> bool operator==(const unordered_set<Key, Hash, KeyEqual, Allocator> & lhs,
                  const unordered_set<Key, Hash, KeyEqual, Allocator> & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (auto it = lhs.begin(); it != lhs.end(); ++it) {
        auto it2 = rhs.find(*it);
        if (it2 == rhs.end())
            return false;
    }

    return true;
}

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> bool operator!=(const unordered_set<Key, Hash, KeyEqual, Allocator> & lhs,
                  const unordered_set<Key, Hash, KeyEqual, Allocator> & rhs)
{
    return !(lhs == rhs);
}

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> void swap(unordered_set<Key, Hash, KeyEqual, Allocator> & lhs,
            unordered_set<Key, Hash, KeyEqual, Allocator> & rhs)
{
    lhs.swap(rhs);
}

/**
 * @brief unordered_multiset
 * @link https://zh.cppreference.com/w/cpp/container/unordered_multiset
 */
template <
    class Key,
    class Hash = wwstl::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = wwstl::allocator<Key>
> class unordered_multiset
{
public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using hashtable_type = wwstl::hashtable<key_type, void, value_type, hasher, key_equal, wwstl::keyExtractor<value_type>, allocator_type>;
    using iterator = typename hashtable_type::iterator;
    using const_iterator = typename hashtable_type::const_iterator;
    using local_iterator = typename hashtable_type::local_iterator;
    using const_local_iterator = typename hashtable_type::const_local_iterator;

public:
    hashtable_type _ht;     // 哈希表

public:
    unordered_multiset()
        : unordered_multiset(size_type(8))
    {
    }

    explicit unordered_multiset(size_type bucket_count, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
    }

    explicit unordered_multiset(const allocator_type & alloc)
        : _ht(alloc)
    {
    }

    template <class InputIt>
    unordered_multiset(InputIt first, InputIt last, size_type bucket_count = 8, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
        insert(first, last);
    }

    unordered_multiset(const unordered_multiset & other)
        : _ht(other._ht)
    {
    }

    unordered_multiset(unordered_multiset && other)
        : _ht(std::move(other._ht))
    {
    }

    unordered_multiset(const unordered_multiset & other, const allocator_type & alloc)
        : _ht(other._ht, alloc)
    {
    }

    unordered_multiset(unordered_multiset && other, const allocator_type & alloc)
        : _ht(std::move(other._ht), alloc)
    {
    }

    unordered_multiset(std::initializer_list<value_type> ilist, size_type bucket_count = 8, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _ht(bucket_count, hash, equals, alloc)
    {
        insert(ilist);
    }

    ~unordered_multiset() = default;

public:
    /**
     * @brief 将值赋给容器
     */
    unordered_multiset & operator=(const unordered_multiset & other)
    {
        _ht = other._ht;
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    unordered_multiset & operator=(unordered_multiset && other)
    {
        _ht = std::move(other._ht);
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    unordered_multiset & operator=(std::initializer_list<value_type> ilist)
    {
        clear();
        insert(ilist);
        return *this;
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const noexcept
    {
        return _ht.get_allocator();
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return _ht.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return _ht.begin();
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    {
        return _ht.cbegin();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    {
        return _ht.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return _ht.end();
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return _ht.cend();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _ht.empty();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _ht.size();
    }

    /**
     * @brief 返回可容纳的最大元素数
     */
    size_type max_size() const noexcept
    {
        return _ht.max_size();
    }

    // 修改器

    /**
     * @brief 清除元素
     */
    void clear() noexcept
    {
        _ht.clear();
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const value_type & value)
    {
        return _ht.emplace_equal(value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(value_type && value)
    {
        return _ht.emplace_equal(std::move(value));
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, const value_type & value)
    {
        return _ht.emplace_hint_equal(hint, value);
    }

    /**
     * @brief 插入元素
     */
    iterator insert(const_iterator hint, value_type && value)
    {
        return _ht.emplace_hint_equal(hint, std::move(value));
    }

    /**
     * @brief 插入元素
     */
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
        {
            _ht.emplace_equal(*first);
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
     * @details 调用emplace_equal，允许重复
     */
    template <class... Args>
    iterator emplace(Args&&... args)
    {
        return _ht.emplace_equal(std::forward<Args>(args)...);
    }

    /**
     * @brief 使用提示原位构造元素
     */
    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args)
    {
        return _ht.emplace_hint_equal(hint, std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        return _ht.erase(pos);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        return _ht.erase(first, last);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key)
    {
        return _ht.erase(key);
    }

    /**
     * @brief 交换内容
     */
    void swap(unordered_multiset & other) noexcept
    {
        _ht.swap(other._ht);
    }

    // 查找

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type & key) const
    {
        return _ht.count(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    iterator find(const key_type & key)
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素
     */
    const_iterator find(const key_type & key) const
    {
        return _ht.find(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        return _ht.equal_range(key);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        return _ht.equal_range(key);
    }

    // 桶接口

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    local_iterator begin(size_type n)
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator begin(size_type n) const
    {
        return _ht.begin(n);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator cbegin(size_type n) const
    {
        return _ht.cbegin(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    local_iterator end(size_type n)
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator end(size_type n) const
    {
        return _ht.end(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator cend(size_type n) const
    {
        return _ht.cend(n);
    }

    /**
     * @brief 返回桶数
     */
    size_type bucket_count() const
    {
        return _ht.bucket_count();
    }

    /**
     * @brief 返回桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _ht.max_bucket_count();
    }

    /**
     * @brief 返回特定的桶中的元素数量
     */
    size_type bucket_size(size_type n) const
    {
        return _ht.bucket_size(n);
    }

    /**
     * @brief 返回带有特定键的桶
     */
    size_type bucket(const key_type& key) const
    {
        return _ht.bucket(key);
    }

    // 散列策略

    /**
     * @brief 负载因子
     */
    float load_factor() const
    {
        return _ht.load_factor();
    }

    /**
     * @brief 最大负载因子
     */
    float max_load_factor() const
    {
        return _ht.max_load_factor();
    }

    /**
     * @brief 设置最大负载因子
     */
    void max_load_factor(float ml)
    {
        _ht.max_load_factor(ml);
    }

    /**
     * @brief 预留至少指定数量的桶并重新生成散列表
     */
    void rehash(size_type count)
    {
        _ht.rehash(count);
    }

    /**
     * @brief 为至少指定数量的元素预留空间并重新生成散列表
     */
    void reserve(size_type count)
    {
        _ht.reserve(count);
    }

    // 观察器

    /**
     * @brief 返回用于对键求散列的函数
     */
    hasher hash_function() const
    {
        return _ht.hash_function();
    }

    /**
     * @brief 返回用于比较键的相等性的函数
     */
    key_equal key_eq() const
    {
        return _ht.key_eq();
    }
};

// 非成员函数

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> bool operator==(const unordered_multiset<Key, Hash, KeyEqual, Allocator> & lhs,
                  const unordered_multiset<Key, Hash, KeyEqual, Allocator> & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (auto it = lhs.begin(); it != lhs.end(); ) {
        auto range_l = lhs.equal_range(*it);
        auto range_r = rhs.equal_range(*it);
        if (!std::is_permutation(range_l.first, range_l.second, range_r.first, range_r.second))
            return false;
        
        it = range_l.second;
    }

    return true;
}

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> bool operator!=(const unordered_multiset<Key, Hash, KeyEqual, Allocator> & lhs,
                  const unordered_multiset<Key, Hash, KeyEqual, Allocator> & rhs)
{
    return !(lhs == rhs);
}

template <
    class Key,
    class Hash,
    class KeyEqual,
    class Allocator
> void swap(unordered_multiset<Key, Hash, KeyEqual, Allocator> & lhs,
            unordered_multiset<Key, Hash, KeyEqual, Allocator> & rhs)
{
    lhs.swap(rhs);
}

} // namespace wwstl

#endif // __WW_UNORDERED_SET_H__