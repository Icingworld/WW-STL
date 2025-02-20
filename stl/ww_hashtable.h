#ifndef __WW_HASHTABLE_H__
#define __WW_HASHTABLE_H__

#include <initializer_list>
#include <cmath>
#include "ww_algorithm.h"
#include "ww_vector.h"
#include "ww_hash.h"
#include "ww_functional.h"
#include "ww_type_traits.h"

namespace wwstl
{

template <
    class Key,
    class T,
    class Value,
    class Hash,
    class KeyEqual,
    class ExtractKey,
    class Allocator
> class hashtable;

/**
 * @brief hashtable节点
 */
template <class Value>
class _hashtable_node
{
public:
    using value_type = Value;
    using node_pointer = _hashtable_node<value_type>*;

public:
    value_type _data;           // 当前节点值
    node_pointer _next;         // 下一个节点

public:
    _hashtable_node()
        : _data()
        , _next(nullptr)
    { // 构造空节点
    }

    explicit _hashtable_node(const value_type & data)
        : _data(data)
        , _next(nullptr)
    { // 以值构造一个节点
    }
};

/**
 * @brief hashtable常量迭代器
 */
template <
    class Key,
    class T,
    class Value,
    class Hash,
    class KeyEqual,
    class ExtractKey,
    class Allocator
> class _hashtable_const_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using reference = const value_type&;
    using pointer = const value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using self = _hashtable_const_iterator<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>;
    using node_pointer = _hashtable_node<value_type>*;
    using hashtable_pointer = hashtable<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>*;
    using const_hashtable_pointer = const hashtable<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>*;

public:
    node_pointer _node;     // 指向哈希表节点
    hashtable_pointer _ht;  // 指向迭代器所属于的哈希表

public:
    _hashtable_const_iterator(const node_pointer node, const_hashtable_pointer ht)
        : _node(const_cast<node_pointer>(node))
        , _ht(const_cast<hashtable_pointer>(ht))
    { // 以节点构造迭代器
      // 注意传入的第一个参数是const node_pointer，而第二个参数是const hashtable*，而不能使用const hashtable_pointer
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
        const node_pointer old = _node;
        _node = _node->_next;   // 尝试指向下一个节点
        if (_node == nullptr) {
            // 不存在下一个节点，需要找到下一个非空桶
            size_type index = _ht->_hash_key(_ht->_get_key(old->_data)) + 1;  // 使用_hash_key的重载
            for (; _node == nullptr && index < _ht->bucket_count(); ++index) {
                _node = _ht->_buckets[index];
            }
        }
        // 如果到达尾部，会返回一个iterator(nullptr, ht)，即end()
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    /**
     * @brief 比较操作符
     * @details 只有当两个迭代器指向同一个节点时才相等，因为哈希表中允许有相同键值对的节点，所以连next也需要相等
     */
    bool operator==(const self & rhs) const
    {
        return _node == rhs._node;
    }

    bool operator!=(const self & rhs) const
    {
        return _node != rhs._node;
    }
};

/**
 * @brief hashtable迭代器
 */
template <
    class Key,
    class T,
    class Value,
    class Hash,
    class KeyEqual,
    class ExtractKey,
    class Allocator
> class _hashtable_iterator
    : public _hashtable_const_iterator<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>
{
public:
    using base = _hashtable_const_iterator<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>;
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using reference = value_type&;
    using pointer = value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using self = _hashtable_iterator<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>;
    using node_pointer = _hashtable_node<value_type>*;
    using hashtable_pointer = hashtable<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator>*;

public:
    _hashtable_iterator(node_pointer node, hashtable_pointer ht)
        : base(node, ht)
    { // 以节点构造迭代器
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
};

/**
 * @brief hashtable local常量迭代器
 */
template <class Value>
class _hashtable_const_local_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using reference = const value_type&;
    using pointer = const value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    using self = _hashtable_const_local_iterator<Value>;
    using node_pointer = _hashtable_node<value_type>*;

public:
    node_pointer _node;     // 指向哈希表节点

public:
    explicit _hashtable_const_local_iterator(const node_pointer node)
        : _node(const_cast<node_pointer>(node))
    { // 构造空迭代器
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

    bool operator==(const self & rhs) const
    {
        return _node == rhs._node;
    }

    bool operator!=(const self & rhs) const
    {
        return _node != rhs._node;
    }
};

/**
 * @brief hashtable local迭代器
 */
template <class Value>
class _hashtable_local_iterator
    : public _hashtable_const_local_iterator<Value>
{
public:
    using base = _hashtable_const_local_iterator<Value>;
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using reference = value_type&;
    using pointer = value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using self = _hashtable_local_iterator<Value>;
    using node_pointer = _hashtable_node<value_type>*;

public:
    explicit _hashtable_local_iterator(node_pointer node)
        : base(node)
    { // 以节点构造迭代器
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
};

/**
 * @brief hashtable
 */
template <
    class Key,
    class T,
    class Value,
    class Hash,
    class KeyEqual,
    class ExtractKey,
    class Allocator
> class hashtable
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = Value;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using key_extractor = ExtractKey;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = _hashtable_iterator<key_type, mapped_type, value_type, hasher, key_equal, key_extractor, allocator_type>;
    using const_iterator = _hashtable_const_iterator<key_type, mapped_type, value_type, hasher, key_equal, key_extractor, allocator_type>;
    using local_iterator = _hashtable_local_iterator<value_type>;
    using const_local_iterator = _hashtable_const_local_iterator<value_type>;

    using node = _hashtable_node<value_type>;
    using node_pointer = node*;
    using node_allocator_type = typename allocator_type::template rebind<node>::other;

public:
    wwstl::vector<node_pointer> _buckets;   // 桶
    size_type _num_elements;                // 元素个数
    float _max_load_factor;                 // 最大负载因子
    hasher _hash;                           // 哈希函数
    key_equal _equals;                      // 判断键值是否相等
    key_extractor _get_key;                 // 获取键值
    node_allocator_type _node_allocator;    // 节点分配器

public:
    /**
     * 1. hashtable的构造函数只负责构造空哈希表，不插入元素
     * 因为无法预知适配器的插入行为，涉及初始化元素的方法都由适配器自行实现
     * 2. 哈希表桶初始化大小为2 ^ N, N > 0，这里选择8
     * 上层适配器的构造函数默认大小也应该设置为8
     */

    hashtable()
        : hashtable(size_type(8))
    {
    }

    explicit hashtable(size_type bucket_count, const hasher & hash = hasher(), const key_equal & equals = key_equal(), const allocator_type & alloc = allocator_type())
        : _buckets(bucket_count)
        , _num_elements(0)
        , _max_load_factor(1.0)
        , _hash(hash)
        , _equals(equals)
        , _get_key()
        , _node_allocator()
    {
    }

    explicit hashtable(const allocator_type & alloc)
        : _buckets(8)
        , _num_elements(0)
        , _max_load_factor(1.0)
        , _hash()
        , _equals()
        , _get_key()
        , _node_allocator(alloc)
    {
    }

    hashtable(const hashtable & other)
        : _buckets(8)
        , _num_elements(0)
        , _max_load_factor(1.0)
        , _hash(other._hash)
        , _equals(other._equals)
        , _get_key(other._get_key)
        , _node_allocator(other._node_allocator)
    {
        _buckets.reserve(other._buckets.size());    // 预留空间
        for (auto it = other.begin(); it != other.end(); ++it) {
            emplace_equal(*it);     // 可以信任适配器的插入行为，允许重复插入来实现拷贝
        }
    }

    hashtable(const hashtable & other, const allocator_type & alloc)
        : _buckets()
        , _num_elements(0)
        , _max_load_factor(1.0)
        , _hash(other._hash)
        , _equals(other._equals)
        , _get_key(other._get_key)
        , _node_allocator(alloc)
    {
        _buckets.reserve(other._buckets.size());    // 预留空间
        for (auto it = other.begin(); it != other.end(); ++it) {
            emplace_equal(*it);     // 可以信任适配器的插入行为，允许重复插入来实现拷贝
        }
    }

    hashtable(hashtable && other)
        : _buckets(std::move(other._buckets))
        , _num_elements(other._num_elements)
        , _max_load_factor(other._max_load_factor)
        , _hash(other._hash)
        , _equals(other._equals)
        , _get_key(other._get_key)
        , _node_allocator(std::move(other._node_allocator))
    {
        other._num_elements = 0;
        other._max_load_factor = 1.0;
    }

    hashtable(hashtable && other, const allocator_type & alloc)
        : _buckets(std::move(other._buckets))
        , _num_elements(other._num_elements)
        , _max_load_factor(other._max_load_factor)
        , _hash(other._hash)
        , _equals(other._equals)
        , _get_key(other._get_key)
        , _node_allocator(alloc)
    {
        other._num_elements = 0;
        other._max_load_factor = 1.0;
    }

    ~hashtable()
    {
        clear();
    }

public:
    /**
     * @brief 将值赋给容器
     */
    hashtable & operator=(const hashtable & other)
    {
        if (this != &other) {
            clear();
            // 拷贝哈希表
            _buckets.reserve(other._buckets.size());    // 预留空间
            _max_load_factor = other._max_load_factor;
            for (auto it = other.begin(); it != other.end(); ++it) {
                emplace_equal(*it);     // 可以信任适配器的插入行为，允许重复插入来实现拷贝
            }
            _hash = other._hash;
            _equals = other._equals;
            _get_key = other._get_key;
            if (allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                _node_allocator = other._node_allocator;
            }
        }
        return *this;
    }

    /**
     * @brief 将值赋给容器
     */
    hashtable & operator=(hashtable && other)
    {
        if (this != &other) {
            clear();
            // 移动哈希表
            _buckets = std::move(other._buckets);
            _num_elements = other._num_elements;
            _max_load_factor = other._max_load_factor;
            _hash = other._hash;
            _equals = other._equals;
            _get_key = other._get_key;
            if (allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                _node_allocator = std::move(other._node_allocator);
            }
            // 清空原哈希表，数组有自身的移动赋值，不需要手动清空
            other._num_elements = 0;
            other._max_load_factor = 1.0;
        }
        return *this;
    }

    /**
     * 关于initializer_list的赋值，此处不提供
     * 因为无法预知上层适配器是否允许重复元素，需要适配器自行实现
     */

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const
    {
        return _node_allocator;
    }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    {
        return iterator(_get_begin(), this);
    }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    {
        return const_iterator(_get_begin(), this);
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
        return iterator(nullptr, this);
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    {
        return const_iterator(nullptr, this);
    }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator cend() const noexcept
    {
        return end();
    }

    // 容量

    /**
     * @brief 检查容器是否为空
     */
    bool empty() const noexcept
    {
        return _num_elements == 0;
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    {
        return _num_elements;
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
        for (size_type n = 0; n < _buckets.size(); ++n) {
            node_pointer cur = _buckets[n];
            while (cur != nullptr) {
                node_pointer next = cur->_next;
                _destroy_node(cur);
                cur = next;
            }
            _buckets[n] = nullptr;
        }
        _num_elements = 0;
    }

    /**
     * emplace_unique和emplace_equal是hashtable的插入实现，为上层适配器提供插入能力
     * 不再提供insert，因为insert的实现会有歧义，不知道是unique还是equal
     * 因此在适配器中，根据上层容器的特性重新设计insert和emplace
     */

    /**
     * @brief 原位构造元素，不允许重复
     * @details 如果发生了插入，返回true，插入失败并返回false
     * @details 用于unordered_set和unordered_map
     */
    template <class... Args>
    std::pair<iterator, bool> emplace_unique(Args&&... args)
    {
        _resize(_num_elements + 1);
        node_pointer p = _create_node(std::forward<Args>(args)...);
        const size_type n = _hash_key(_get_key(p->_data));
        node_pointer first = _buckets[n];
        for (node_pointer cur = first; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), _get_key(p->_data))) {
                // 找到相同键值的节点，插入失败，返回阻碍插入的节点迭代器
                _destroy_node(p);
                return std::make_pair(iterator(cur, this), false);
            }
        }
        // 未找到相同键值的节点，插入到链表头部
        p->_next = first;
        _buckets[n] = p;
        ++_num_elements;
        return std::make_pair(iterator(p, this), true);
    }

    /**
     * @brief 原位构造元素，允许重复
     * @details 用于unordered_multiset和unordered_multimap
     */
    template <class... Args>
    iterator emplace_equal(Args&&... args)
    {
        _resize(_num_elements + 1);
        node_pointer p = _create_node(std::forward<Args>(args)...);
        const size_type n = _hash_key(_get_key(p->_data));
        node_pointer first = _buckets[n];
        for (node_pointer cur = first; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), _get_key(p->_data))) {
                // 找到相同键值的节点，插入到链表中
                p->_next = cur->_next;
                cur->_next = p;
                ++_num_elements;
                // _begin一定不需要更新
                return iterator(p, this);
            }
        }
        // 未找到相同键值的节点，插入到链表头部
        p->_next = first;
        _buckets[n] = p;
        ++_num_elements;
        return iterator(p, this);
    }

    /**
     * emplace_hint_unique和emplace_hint_equal是hashtable的根据提示插入实现
     * 该接口返回值不提供插入成功与否的信息，因为emplace_hint的目的在于提高性能
     * 因此不关注插入是否成功，用户应对插入数据的行为负责
     * 注：MSVC的xhash和gcc的hashtable.h也没有实现hint
     */

    /**
     * @brief 根据提示插入元素，不允许重复
     */
    template <class... Args>
    iterator emplace_hint_unique(const_iterator, Args&&... args)
    {
        // 忽略hint
        return emplace_unique(std::forward<Args>(args)...).first;
    }

    /**
     * @brief 根据提示插入元素，允许重复
     */
    template <class... Args>
    iterator emplace_hint_equal(const_iterator, Args&&... args)
    {
        // 忽略hint
        return emplace_equal(std::forward<Args>(args)...);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        if (pos == end()) {
            // hashtable_iterator不可能指向桶的末尾，无需多判断
            return end();
        }

        const size_type n = _hash_key(_get_key(*pos));
        for (node_pointer cur = _buckets[n], prev = nullptr; cur != nullptr; prev = cur, cur = cur->_next) {
            if (cur == pos._node) {
                node_pointer next = cur->_next;
                if (prev == nullptr) {
                    _buckets[n] = next;
                } else {
                    prev->_next = next;
                }
                _destroy_node(cur);
                --_num_elements;

                if (next != nullptr) {
                    return iterator(next, this);
                }
                
                // 寻找下一个非空桶
                for (size_type i = n + 1; i < bucket_count(); ++i) {
                    if (_buckets[i] != nullptr) {
                        return iterator(_buckets[i], this);
                    }
                }
                return end();
            }
        }
        return end();
    }

    /**
     * @brief 擦除元素
     * @details 需要优化逻辑
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        // 计算[first, last)的桶范围
        size_type first_bucket = _hash_key(_get_key(*first));
        size_type last_bucket = bucket_count();  // 用于区分last是否为end()，实际上不可访问
        if (last != end()) {
            last_bucket = _hash_key(_get_key(*last));
        }

        if (first_bucket == last_bucket) {
            // 位于同一个桶，寻找first的前一个节点
            node_pointer prev = nullptr;
            node_pointer cur = _buckets[first_bucket];
            while (cur != nullptr && cur != first._node) {
                prev = cur;
                cur = cur->_next;
            }
            // 逐个销毁节点
            while (cur != nullptr && cur != last._node) {
                node_pointer next = cur->_next;
                _destroy_node(cur);
                cur = next;
                --_num_elements;
            }
            // 更新链表
            if (prev == nullptr) {
                _buckets[first_bucket] = cur;
            } else {
                prev->_next = cur;
            }
        } else {
            // 位于不同桶，分别处理
            if (last_bucket == -1) {
                // 说明last是end()，需要擦除后面所有的桶
                last_bucket = _buckets.size() - 1;
            }
            // first所在的桶，直接销毁first后面的节点
            node_pointer prev = nullptr;
            node_pointer cur = _buckets[first_bucket];
            while (cur != nullptr && cur != first._node) {
                prev = cur;
                cur = cur->_next;
            }
            if (prev == nullptr) {
                // 说明first是桶的第一个节点，直接销毁链表
                erase(_get_key(*first));
            } else {
                // 说明first不是桶的第一个节点，从prev开始销毁
                node_pointer cur = first._node;
                while (cur != nullptr) {
                    node_pointer next = cur->_next;
                    _destroy_node(cur);
                    cur = next;
                    --_num_elements;
                }
                prev->_next = nullptr;
            }
            // 中间的桶，直接销毁整个桶
            for (size_type n = first_bucket + 1; n < last_bucket - 1; ++n) {
                node_pointer cur = _buckets[n];
                while (cur != nullptr) {
                    node_pointer next = cur->_next;
                    _destroy_node(cur);
                    cur = next;
                    --_num_elements;
                }
                _buckets[n] = nullptr;
            }
            // last所在的桶，销毁last前面的节点
            cur = _buckets[last_bucket];
            while (cur != last._node) {
                node_pointer next = cur->_next;
                _destroy_node(cur);
                cur = next;
                --_num_elements;
            }
            _buckets[last_bucket] = cur;
        }
        return iterator(last._node, this);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key)
    {
        const size_type n = _hash_key(key);
        size_type erased = 0;

        for (node_pointer cur = _buckets[n], prev = nullptr; cur != nullptr; ) {
            if (_equals(_get_key(cur->_data), key)) {
                node_pointer next = cur->_next;
                if (prev == nullptr) {
                    _buckets[n] = next;
                } else {
                    prev->_next = next;
                }
                _destroy_node(cur);
                cur = next;
                ++erased;
                --_num_elements;
            } else {
                prev = cur;
                cur = cur->_next;
            }
        }
        return erased;
    }

    /**
     * @brief 交换内容
     */
    void swap(hashtable & other)
    {
        std::swap(_buckets, other._buckets);
        std::swap(_num_elements, other._num_elements);
        std::swap(_max_load_factor, other._max_load_factor);
        std::swap(_hash, other._hash);
        std::swap(_equals, other._equals);
        std::swap(_get_key, other._get_key);
        if (wwstl::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
            std::swap(_node_allocator, other._node_allocator);
        }
    }

    // 查找

    /**
     * @brief 带越界检查访问指定的元素
     * @details 由于在set适配器中不需要mapped_type，使用了void，为了避免void &的编译错误，这里用SFINAE避免
     */
    template <typename U = mapped_type, typename = typename std::enable_if<!std::is_void<U>::value>::type>
    U & at(const key_type & key)
    {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("hashtable at out of range");
        }
        return it->second;
    }

    /**
     * @brief 带越界检查访问指定的元素
     */
    template <typename U = mapped_type, typename = typename std::enable_if<!std::is_void<U>::value>::type>
    const U & at(const key_type & key) const
    {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("hashtable at out of range");
        }
        return it->second;
    }

    /**
     * @brief 访问或插入指定的元素
     */
    template <typename U = mapped_type, typename = typename std::enable_if<!std::is_void<U>::value>::type>
    U & operator[](const key_type & key)
    {
        auto it = emplace_unique(std::make_pair(key, mapped_type()));
        return it.first->second;
    }

    /**
     * @brief 访问或插入指定的元素
     */
    template <typename U = mapped_type, typename = typename std::enable_if<!std::is_void<U>::value>::type>
    U & operator[](key_type && key)
    {
        auto it = emplace_unique(std::make_pair(std::move(key), mapped_type()));
        return it.first->second;
    }

    /**
     * @brief 返回匹配特定键的元素数量
     */
    size_type count(const key_type & key) const
    {
        const size_type n = _hash_key(key);
        size_type result = 0;
        for (node_pointer cur = _buckets[n]; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), key)) {
                ++result;
            }
        }
        return result;
    }

    /**
     * @brief 查找特定键的元素
     */
    iterator find(const key_type & key)
    {
        return iterator(_find(key), this);
    }

    /**
     * @brief 查找特定键的元素
     */
    const_iterator find(const key_type & key) const
    {
        return const_iterator(_find(key), this);
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        const size_type n = _hash_key(key);
        for (node_pointer cur = _buckets[n]; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), key)) {
                // 找到第一个匹配的节点
                node_pointer first = cur;
                while (cur->_next != nullptr && _equals(_get_key(cur->_next->_data), key)) {
                    // 该节点还是匹配的，继续向后
                    cur = cur->_next;
                }
                iterator last = iterator(cur, this);  // 因为可能是桶的末尾，所以不能直接移动指针，而应该移动迭代器，否则可能返回end()
                return std::make_pair(iterator(first, this), std::next(last));
            }
        }
        return std::make_pair(end(), end());
    }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        const size_type n = _hash_key(key);
        for (node_pointer cur = _buckets[n]; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), key)) {
                node_pointer first = cur;
                while (cur->_next != nullptr && _equals(_get_key(cur->_next->_data), key)) {
                    cur = cur->_next;
                }
                const_iterator last = const_iterator(cur, this);
                return std::make_pair(const_iterator(first, this), std::next(last));
            }
        }
        return std::make_pair(cend(), cend());
    }

    // 桶接口

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    local_iterator begin(size_type n)
    {
        return local_iterator(_buckets[n]);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator begin(size_type n) const
    {
        return const_local_iterator(_buckets[n]);
    }

    /**
     * @brief 返回指向指定的桶的开始的迭代器
     */
    const_local_iterator cbegin(size_type n) const
    {
        return begin(n);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    local_iterator end(size_type n)
    {
        return local_iterator(nullptr);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator end(size_type n) const
    {
        return const_local_iterator(nullptr);
    }

    /**
     * @brief 返回指向指定的桶的末尾的迭代器
     */
    const_local_iterator cend(size_type n) const
    {
        return end(n);
    }

    /**
     * @brief 返回桶数
     */
    size_type bucket_count() const
    {
        return _buckets.size();
    }

    /**
     * @brief 返回桶的最大数量
     */
    size_type max_bucket_count() const
    {
        return _buckets.max_size();
    }

    /**
     * @brief 返回特定的桶中的元素数量
     */
    size_type bucket_size(size_type n) const
    {
        size_type result = 0;
        for (node_pointer cur = _buckets[n]; cur != nullptr; cur = cur->_next) {
            ++result;
        }
        return result;
    }

    /**
     * @brief 返回带有特定键的桶
     */
    size_type bucket(const key_type & key) const
    {
        return _hash_key(key);
    }

    // 散列策略

    /**
     * @brief 返回负载因子
     */
    float load_factor() const
    {
        return static_cast<float>(size()) / static_cast<float>(bucket_count());
    }

    /**
     * @brief 返回最大负载因子
     */
    float max_load_factor() const
    {
        return _max_load_factor;
    }

    /**
     * @brief 设置最大负载因子
     */
    void max_load_factor(float ml)
    {
        _max_load_factor = ml;
    }

    /**
     * @brief 预留至少指定数量的桶并重新生成散列表
     */
    void rehash(size_type count)
    {
        wwstl::vector<node_pointer> new_buckets(count, nullptr);
        // 重新计算每个元素的位置
        for (size_type n = 0; n < _buckets.size(); ++n) {
            for (node_pointer cur = _buckets[n]; cur != nullptr; ) {
                node_pointer next = cur->_next;
                const size_type new_n = _hash_key(_get_key(cur->_data), count);
                cur->_next = new_buckets[new_n];
                new_buckets[new_n] = cur;
                cur = next;
            }
        }
        // 交换桶
        _buckets.swap(new_buckets);
    }

    /**
     * @brief 为至少指定数量的元素预留空间并重新生成散列表
     */
    void reserve(size_type count)
    {
        rehash(std::ceil(count / max_load_factor()));
    }

    // 观察器

    /**
     * @brief 返回用于对键求散列的函数
     */
    hasher hash_function() const
    {
        return _hash;
    }

    /**
     * @brief 返回用于比较键的相等性的函数
     */
    key_equal key_eq() const
    {
        return _equals;
    }

public:
    /**
     * @brief 计算key所在的桶
     * @param key 键
     * @param n 桶的数量
     */
    size_type _hash_key(const key_type & key, size_type n) const
    {
        return _hash(key) % n;
    }

    /**
     * @brief 计算key所在的桶
     * @param key 键
     */
    size_type _hash_key(const key_type & key) const
    {
        return _hash_key(key, _buckets.size());
    }

    node_pointer _get_node()
    {
        node_pointer p = allocator_traits<node_allocator_type>::allocate(_node_allocator, 1);
        return p;
    }

    void _put_node(node_pointer p)
    {
        allocator_traits<node_allocator_type>::deallocate(_node_allocator, p, 1);
    }

    template <class... Args>
    node_pointer _create_node(Args&&... args)
    {
        node_pointer p = _get_node();
        allocator_traits<node_allocator_type>::construct(_node_allocator, p, std::forward<Args>(args)...);
        return p;
    }

    void _destroy_node(node_pointer p)
    {
        allocator_traits<node_allocator_type>::destroy(_node_allocator, p);
        _put_node(p);
    }

    /**
     * @brief 返回指向第一个元素节点的指针
     */
    node_pointer _get_begin() const
    {
        for (size_type n = 0; n < _buckets.size(); ++n) {
            if (_buckets[n] != nullptr) {
                return _buckets[n];
            }
        }
        return nullptr;
    }

    /**
     * @brief 判断是否需要重建哈希表
     */
    void _resize(size_type new_count)
    {
        if (load_factor() > max_load_factor()) {
            rehash(new_count * 2);
        }
    }

    node_pointer _find(const key_type & key) const
    {
        const size_type n = _hash_key(key);
        for (node_pointer cur = _buckets[n]; cur != nullptr; cur = cur->_next) {
            if (_equals(_get_key(cur->_data), key)) {
                return cur;
            }
        }
        return nullptr;
    }
};

// 非成员函数

/**
 * 此处不提供operator==和operator!=
 * 因为判断逻辑需要根据适配器的性质决定
 */

template <
    class Key,
    class T,
    class Value,
    class Hash,
    class KeyEqual,
    class ExtractKey,
    class Allocator
> void swap(hashtable<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator> & lhs,
            hashtable<Key, T, Value, Hash, KeyEqual, ExtractKey, Allocator> & rhs)
{
    lhs.swap(rhs);
}

} // namespace wwstl

#endif // __WW_HASHTABLE_H__