#ifndef __WW_RB_TREE_H__
#define __WW_RB_TREE_H__

#include <initializer_list>
#include <functional>
#include "ww_memory.h"
#include "ww_iterator.h"
#include "ww_functional.h"

namespace wwstl
{

/**
 * 定义红黑树的颜色
 */
using color_type = bool;
constexpr color_type _red = true;        // 红色true
constexpr color_type _black = false;     // 黑色false

/**
 * @brief 红黑树节点
 * @details 节点默认为红色
 */
template <class Value>
class _rb_tree_node
{
public:
    using value_type = Value;
    using node_pointer = _rb_tree_node<Value>*;

public:
    node_pointer _parent;       // 父节点
    node_pointer _left;         // 左子节点
    node_pointer _right;        // 右子节点
    color_type _color;          // 颜色
    value_type _data;           // 值

public:
    _rb_tree_node()
        : _data(), _parent(nullptr), _left(nullptr), _right(nullptr), _color(_red)
    { // 构造空节点
    }

    _rb_tree_node(const value_type & value)
        : _data(value), _parent(nullptr), _left(nullptr), _right(nullptr), _color(_red)
    { // 以值构造一个节点
    }

    _rb_tree_node(value_type && value)
        : _data(std::move(value)), _parent(nullptr), _left(nullptr), _right(nullptr), _color(_red)
    { // 以值构造一个节点
    }

public:
    /**
     * @brief 返回最小节点
     */
    static node_pointer minimum(node_pointer x)
    {
        while (x->_left != nullptr)
            x = x->_left;
        return x;
    }

    /**
     * @brief 返回最大节点
     */
    static node_pointer maximum(node_pointer x)
    {
        while (x->_right != nullptr)
            x = x->_right;
        return x;
    }
};

/**
 * @brief 红黑树常量迭代器
 */
template <class Value>
class _rb_tree_const_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;

    using self = _rb_tree_const_iterator<Value>;
    using node = _rb_tree_node<Value>;
    using node_pointer = _rb_tree_node<Value>*;

public:
    node_pointer _node;         // 指向当前节点

public:
    _rb_tree_const_iterator(const node_pointer node)
        : _node(const_cast<node_pointer>(node))
    { // 以节点构造迭代器
    }

public:
    reference operator*() const
    { return _node->_data; }

    pointer operator->() const
    { return &(operator*()); }

    self & operator++()
    {
        // 如果存在右子节点,则返回右子节点的最小节点
        // 这种移动方式契合了重复元素的插入和搜索，找到重复元素的第一个后，右子节点的最小节点一定还是重复的
        // ~~想想如果重复元素是往左子树插入，迭代器要怎么移动呢~~
        if (_node->_right != nullptr) {
            _node = node::minimum(_node->_right);
        } else {
            // 找到父节点
            node_pointer p = _node->_parent;
            while (_node == p->_right) {
                // 如果是父节点的右子节点，则继续向上找父节点，直到找到该节点是父节点的左子节点
                _node = p;
                p = p->_parent;
            }
            if (_node->_right != p) {
                // 如果右子节点不是父节点，则返回父节点，否则返回当前节点
                _node = p;
            }
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
        if (_node->_color == _red && _node->_parent->_parent == _node) {
            // 如果当前节点是红色，且它的父节点是根节点，此时当前节点为head
            _node = _node->_right;
        } else if (_node->_left != nullptr) {
            // 如果存在左子节点，返回左子节点的最大节点
            _node = node::maximum(_node->_left);
        } else {
            node_pointer p = _node->_parent;
            while (_node == p->_left) {
                _node = p;
                p = p->_parent;
            }
            _node = p;
        }
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self & other) const
    { return _node == other._node; }

    bool operator!=(const self & other) const
    { return !(*this == other); }
};

/**
 * @brief 红黑树迭代器
 */
template <class Value>
class _rb_tree_iterator
    : public _rb_tree_const_iterator<Value>
{
public:
    using base = _rb_tree_const_iterator<Value>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    using self = _rb_tree_iterator<Value>;
    using node = _rb_tree_node<Value>;
    using node_pointer = _rb_tree_node<Value>*;

public:
    _rb_tree_iterator(node_pointer node)
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
};

/**
 * @brief 红黑树
 */
template <
    class Key,
    class T,
    class Value,
    class ExtractKey,
    class Compare = std::less<Key>,
    class Allocator = wwstl::allocator<Value>
> class rb_tree
{
public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = Value;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_extractor = ExtractKey;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = _rb_tree_iterator<value_type>;
    using const_iterator = _rb_tree_const_iterator<value_type>;
    using reverse_iterator = wwstl::reverse_iterator<iterator>;
    using const_reverse_iterator = wwstl::reverse_iterator<const_iterator>;

    using node = _rb_tree_node<value_type>;
    using node_pointer = node*;
    using node_allocator_type = typename allocator_type::template rebind<node>::other;

public:
    node_pointer _head;                     // 红黑树的头节点
    size_type _size;                        // 节点个数
    key_compare _comp;                      // 比较函数
    key_extractor _get_key;                 // 获取键值
    node_allocator_type _node_allocator;    // 节点分配器

public:
    rb_tree()
        : rb_tree(key_compare())
    {
    }

    explicit rb_tree(const key_compare & comp, const allocator_type & alloc = allocator_type())
        : _head(nullptr), _size(0), _comp(comp), _get_key(), _node_allocator(alloc)
    { _init_rb_tree(); }

    explicit rb_tree(const allocator_type & alloc)
        : _head(nullptr), _size(0), _comp(), _get_key(), _node_allocator(alloc)
    { _init_rb_tree(); }

    template <class InputIt>
    rb_tree(InputIt first, InputIt last, const key_compare & comp = key_compare(), const allocator_type & alloc = allocator_type())
        : _head(nullptr), _size(0), _comp(comp), _get_key(), _node_allocator(alloc)
    {
        _init_rb_tree();
        for (; first != last; ++first) {
            emplace_equal(*first);
        }
    }

    rb_tree(const rb_tree & other)
        : _head(nullptr), _size(0), _comp(other._comp), _get_key(other._get_key), _node_allocator(other._node_allocator)
    {
        _init_rb_tree();
        for (auto it = other.begin(); it != other.end(); ++it) {
            emplace_equal(*it);
        }
    }

    rb_tree(const rb_tree & other, const allocator_type & alloc)
        : _head(nullptr), _size(0), _comp(other._comp), _get_key(other._get_key), _node_allocator(alloc)
    {
        _init_rb_tree();
        for (auto it = other.begin(); it != other.end(); ++it) {
            emplace_equal(*it);
        }
    }

    rb_tree(rb_tree && other)
        : _head(other._head), _size(other._size), _comp(other._comp), _get_key(other._get_key), _node_allocator(other._node_allocator)
    {
        other._init_rb_tree();
        other._size = 0;
    }

    rb_tree(rb_tree && other, const allocator_type & alloc)
        : _head(other._head), _size(other._size), _comp(other._comp), _get_key(other._get_key), _node_allocator(alloc)
    {
        other._init_rb_tree();
        other._size = 0;
    }

    ~rb_tree()
    {
        if (_root() != nullptr) {
            _destroy_rb_tree(_root());
        }
        // 销毁head
        _destroy_node(_head);
    }

public:
    rb_tree & operator=(const rb_tree & other)
    {
        if (this != &other) {
            // 清空自身红黑树
            clear();
            // 拷贝
            for (auto it = other.begin(); it != other.end(); ++it) {
                emplace_equal(*it); // 选择信任other，不会用重复数据传入不应该重复的数据结构中
            }
            _comp = other._comp;
            _get_key = other._get_key;
            if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                _node_allocator = other._node_allocator;
            }
        }
        return *this;
    }

    rb_tree & operator=(rb_tree && other)
    {
        if (this != &other) {
            _head = other._head;
            _size = other._size;
            _comp = other._comp;
            _get_key = other._get_key;
            if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                _node_allocator = std::move(other._node_allocator);
            }
            // 清空原红黑树
            other._init_rb_tree();
            other._size = 0;
        }
        return *this;
    }

    /**
     * @brief 返回关联的分配器
     */
    allocator_type get_allocator() const
    { return _node_allocator; }

    // 迭代器

    /**
     * @brief 返回指向起始的迭代器
     */
    iterator begin() noexcept
    { return iterator(_leftmost()); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator begin() const noexcept
    { return const_iterator(_leftmost()); }

    /**
     * @brief 返回指向起始的迭代器
     */
    const_iterator cbegin() const noexcept
    { return begin(); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    iterator end() noexcept
    { return iterator(_head); }

    /**
     * @brief 返回指向末尾的迭代器
     */
    const_iterator end() const noexcept
    { return const_iterator(_head); }

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
    bool empty() const noexcept
    { return _size == 0; }

    /**
     * @brief 返回元素数
     */
    size_type size() const noexcept
    { return _size; }

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
        if (_root() != nullptr) {
            _destroy_rb_tree(_root());
            _init_head();
            _size = 0;
        }
    }

    /**
     * @brief 原位构造元素，不允许重复
     * @details 如果发生了插入，返回true，插入失败并返回false
     * @details 用于set和map
     */
    template <class... Args>
    std::pair<iterator, bool> emplace_unique(Args&&... args)
    {
        node_pointer new_node = _create_node(std::forward<Args>(args)...);
        key_type key = _get_key(new_node->_data);
        node_pointer parent = nullptr;      // 记录cur的父节点
        node_pointer cur = _root();
        while (cur != nullptr) {
            parent = cur;
            if (_comp(_get_key(cur->_data), key)) {
                // 当前节点的键小于目标key，向右寻找
                cur = _right(cur);
            } else if (_comp(key, _get_key(cur->_data))) {
                // 当前节点的键大于目标key，向左寻找
                cur = _left(cur);
            } else {
                // 重复，插入失败
                _destroy_node(new_node);
                return std::make_pair<iterator, bool>(iterator(cur), false);
            }
        }
        // 查找结束，在parent的位置插入
        return std::make_pair<iterator, bool>(_emplace(parent, std::move(new_node)), true);
    }

    /**
     * @brief 原位构造元素，允许重复
     * @details 用于multiset和multimap
     */
    template <class... Args>
    iterator emplace_equal(Args&&... args)
    {
        node_pointer new_node = _create_node(std::forward<Args>(args)...);
        key_type key = _get_key(new_node->_data);
        node_pointer parent = nullptr;      // 记录cur的父节点
        node_pointer cur = _root();
        // 查找插入位置
        while (cur != nullptr) {
            parent = cur;
            if (_comp(key, _get_key(cur->_data))) {
                // 目标key小于当前节点的键，向左寻找
                cur = _left(cur);
            } else {
                // 目标key大于等于当前节点，向右寻找
                cur = _right(cur);
            }
        }
        // 查找结束，在parent的位置插入
        return _emplace(parent, std::move(new_node));
    }

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

    #define wwstl_DEBUG 0
    #if wwstl_DEBUG
        #define my_printf(...) printf(__VA_ARGS__)
    #else
        #define my_printf(...)
    #endif

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator pos)
    {
        my_printf("\n目标删除=%d\n", pos._node->_data);
        const_iterator next = std::next(pos);   // 下一个节点
        node_pointer node = pos._node;          // 要擦除的节点
        node_pointer parent = node->_parent;    // 要擦除的节点的父节点
        my_printf("父节点=%d\n", parent->_data);
        my_printf("父节点颜色=%d\n", parent->_color);
        my_printf("此时根节点=%d\n", _root()->_data);
        
        /**
         * 在红黑树的删除中，可以分为三大类情况
         * 1. 删除的节点只有一个子节点，此时子节点一定是红色，且节点一定是黑色，否则违反红黑树的性质
         * 2. 删除的节点没有子节点，需要根据兄弟节点的颜色和兄弟节点的孩子情况进行调整
         * 3. 删除的节点有两个子节点，选择它的直接后继节点进行替换，并转为删除它的后继节点
         */

        // 在删除之前，修改leftmost和rightmost
        // 先调整rightmost再调整leftmost
        if (_rightmost() == node) {
            if (_leftmost() == node) {
                // 删除的是最后一个节点，pos无法向前移动了，直接设置
                _rightmost() = _head;
                my_printf("是最后一个值！\n");
            } else {
                // 向前移动
                const_iterator prev = std::prev(pos);
                _rightmost() = prev._node;
            }
            my_printf("删除了最大值\n");
        }
        if (_leftmost() == node) {
            if (next._node == _head) {
                my_printf("是最后一个值！\n");
            }
            _leftmost() = next._node;
            my_printf("删除了最小值\n");
        }

        // 1. 有两个子节点
        // 可以采用节点的直接前驱或者直接后继来替代它，然后转化为删除该前驱或者后继的情况，最后一定会变为情况2或情况3
        // 这里使用直接后继，因为erase()接口的语义需要返回下一个迭代器
        if (_left(node) != nullptr && _right(node) != nullptr) {
            // 这里可以直接递归，因为有两个子节点的情况下，不可能出现next为head的情况
            my_printf("情况1\n");
            // 将两个节点互换，这里不构造新节点替换node，而是要交换
            // 因为如果替换node，删除next，会导致next迭代器失效，在范围删除中会出bug
            node_pointer node_left_child = _left(node);
            node_pointer node_right_child = _right(node);
            // 没有next_left_child，因为直接后继一定没有左子树
            node_pointer next_right_child = _right(next._node);
            node_pointer next_parent = _parent(next._node);
            // 开始交换位置
            
            if (_right(node) == next._node) {
                // 1. next就是node的右子节点
                my_printf("next是node的右子节点\n");
                _parent(node) = next._node;    // node->parent
                std::swap(_left(node), _left(next._node));  // node->left, next->left
                _right(node) = next_right_child;    // node->right
                std::swap(_color(node), _color(next._node));    // node->color, next->color
                _parent(next._node) = parent;   // next->parent
                _right(next._node) = node;      // next->right
                // 设置子节点的父节点和父节点的子节点
                _parent(node_left_child) = next._node;
                if (next_right_child != nullptr) {
                    _parent(next_right_child) = node;
                }
            } else {
                // 2. next不是右子节点，只是直接后继
                my_printf("next不是右子节点, 只是直接后继\n");
                std::swap(_left(node), _left(next._node));
                std::swap(_right(node), _right(next._node));
                std::swap(_parent(node), _parent(next._node));
                std::swap(_color(node), _color(next._node));
                // 更新node子节点的父指针
                _parent(node_right_child) = next._node;
                _parent(node_left_child) = next._node;
                // 更新next子节点的父指针，注意没有左子节点
                if (next_right_child != nullptr) {
                    _parent(next_right_child) = node;
                }
                // 更新父节点
                if (_left(next_parent) == next._node) {
                    _left(next_parent) = node;
                } else {
                    _right(next_parent) = node;
                }
            }

            if (parent == _head) {
                _root() = next._node;
            } else if (_left(parent) == node) {
                _left(parent) = next._node;
            } else {
                _right(parent) = next._node;
            }
            my_printf("交换完毕\n");
            erase(const_iterator(node));
            return iterator(next._node);
        }

        // 2. 没有子节点
        if (node->_left == nullptr && node->_right == nullptr) {
            my_printf("没有子节点\n");
            if (_color(node) == _red) {
                my_printf("是红节点\n");
                my_printf("情况2.1\n");
                // 2.1 是红节点，直接删除节点即可，不会破坏红黑树的性质
                // 置空父节点的指针
                if (_left(parent) == node) {
                    my_printf("父节点左指针置空\n");
                    _left(parent) = nullptr;
                } else {
                    my_printf("父节点右指针置空\n");
                    _right(parent) = nullptr;
                }
                // 销毁当前节点
                _destroy_node(node);
                --_size;
                return iterator(next._node);
            } else {
                my_printf("是黑节点\n");
                // 2.2 是黑节点，需要查看兄弟节点的情况
                node_pointer sibling = nullptr;

                // 这里使用循环是因为双黑节点上移后需要继续调整，使用is_first来控制在第一次循环中销毁节点
                for (bool is_first = true; ; is_first = false) {
                    // 找到兄弟节点
                    if (parent == _head) {
                        my_printf("兄弟是自己！\n");
                        sibling = node;
                    } else
                    if (_right(parent) == node) {
                        my_printf("兄弟在左边\n");
                        sibling = _left(parent);
                    } else {
                        my_printf("兄弟在右边\n");
                        sibling = _right(parent);
                    }
                    if (_color(sibling) == _black) {
                        my_printf("兄弟是黑色节点\n");
                        // 2.2.1 兄弟节点是黑色，需要查看兄弟节点的孩子的情况
                        // 这种情况下，兄弟节点的孩子如果存在一定是红色，否则是黑色空节点
                        // 此时可能存在node是根节点的情况，也就是说node是红黑树中最后一个节点，删除后head恢复初始化
                        node_pointer left_child = _left(sibling);
                        node_pointer right_child = _right(sibling);

                        if (_left(parent) == sibling && left_child != nullptr && _color(left_child) == _red) {
                            my_printf("兄弟左孩子红色，右孩子任意\n");
                            my_printf("情况2.2.1.1\n");
                            // 2.2.1.1 兄弟节点的左孩子是红色，则全都归为LL情况，无论右孩子是什么
                            // LL情况中，需要先变色：r变s，s变p，p变黑
                            _color(left_child) = _color(sibling);
                            _color(sibling) = _color(parent);
                            _color(parent) = _black;
                            // 右旋parent节点
                            _right_rotate(parent);
                            // 销毁当前节点
                            if (is_first) {
                                // 只有第一次循环可以销毁节点，后续要调整只要变色旋转即可
                                // 后续调整来自于情况2.2.1.5.*和情况2.2.2，会继续循环
                                _right(parent) = nullptr;
                                _destroy_node(node);
                                --_size;
                            }
                            return iterator(next._node);
                        }

                        if (_left(parent) == sibling && right_child != nullptr && _color(right_child) == _red && left_child == nullptr) {
                            my_printf("兄弟右孩子红色，左孩子黑色\n");
                            my_printf("情况2.2.1.2\n");
                            // 2.2.1.2 兄弟节点的右孩子是红色，且左孩子是黑色，LR情况
                            // LR情况中，需要先变色：r变p，p变黑
                            _color(right_child) = _color(parent);
                            _color(parent) = _black;
                            // 先左旋sibling节点，再右旋parent节点
                            _left_rotate(sibling);
                            _right_rotate(parent);
                            // 销毁当前节点
                            if (is_first) {
                                _right(parent) = nullptr;
                                _destroy_node(node);
                                --_size;
                            }
                            return iterator(next._node);
                        }

                        if (_right(parent) == sibling && right_child != nullptr && _color(right_child) == _red) {
                            my_printf("兄弟右孩子红色，左孩子任意\n");
                            my_printf("情况2.2.1.3\n");
                            // 2.2.1.3 兄弟节点的右孩子是红色，且全部归为RR情况，无论左孩子是什么
                            // RR情况中，需要先变色：r变s，s变p，p变黑
                            _color(right_child) = _color(sibling);
                            _color(sibling) = _color(parent);
                            _color(parent) = _black;
                            // 左旋parent节点
                            _left_rotate(parent);
                            // 销毁当前节点
                            if (is_first) {
                                _left(parent) = nullptr;
                                _destroy_node(node);
                                --_size;
                            }
                            return iterator(next._node);
                        }

                        if (_right(parent) == sibling && left_child != nullptr && _color(left_child) == _red && right_child == nullptr) {
                            my_printf("兄弟左孩子红色，右孩子黑色\n");
                            my_printf("情况2.2.1.4\n");
                            // 2.2.1.4 兄弟节点的左孩子是红色，且右孩子是黑色，RL情况
                            // RL情况中，需要先变色：r变p，p变黑
                            _color(left_child) = _color(parent);
                            _color(parent) = _black;
                            // 先右旋sibling节点，再左旋parent节点
                            _right_rotate(sibling);
                            _left_rotate(parent);
                            // 销毁当前节点
                            if (is_first) {
                                _left(parent) = nullptr;
                                _destroy_node(node);
                                --_size;
                            }
                            return iterator(next._node);
                        }

                        // 2.2.1.5 兄弟节点的左右孩子都是黑色，即没有孩子
                        // 这种情况下，兄弟变红，双黑上移
                        _color(sibling) = _red;

                        // 此时已经不需要当前节点了，在第一次循环中直接销毁，后面还要更新node指针
                        if (is_first) {
                            if (parent == _head) {
                                my_printf("此时是根节点，父节点为head\n");
                                // 删除根节点，并恢复head
                                // 一开始已经恢复过leftmost和rightmost了
                                _root() = nullptr;
                            } else if (_left(parent) == node) {
                                my_printf("父节点左指针置空\n");
                                _left(parent) = nullptr;
                            } else {
                                my_printf("父节点右指针置空\n");
                                _right(parent) = nullptr;
                            }
                            _destroy_node(node);
                            --_size;
                        }

                        // 考虑父节点的情况
                        if (parent == _root() || _color(parent) == _red) {
                            // 2.2.1.5.1 父节点是根节点，或者父节点是红色
                            // 不需要进行额外操作，将红色父节点变黑即可
                            _color(parent) = _black;
                            my_printf("父节点是根节点，或者父节点是红色\n");
                            my_printf("情况2.2.1.5.1\n");
                            return iterator(next._node);
                        } else {
                            // 2.2.1.5.2 父节点是黑色，且不是根节点
                            // 父节点变为新的双黑节点，更新指针继续循环
                            node = parent;
                            parent = _parent(node);
                            my_printf("父节点不是根节点，且是黑色\n");
                            my_printf("情况2.2.1.5.2\n");
                        }

                        // 继续循环调整
                    } else {
                        my_printf("兄弟节点是红色\n");
                        my_printf("情况2.2.2\n");
                        // 2.2.2 兄弟节点是红色
                        // 先将s和p变色
                        _color(sibling) = !_color(sibling);
                        _color(parent) = !_color(parent);
                        // 再将父节点向双黑节点的方向旋转
                        if (node == _left(parent)) {
                            // 左旋父节点
                            _left_rotate(parent);
                        } else {
                            // 右旋父节点
                            _right_rotate(parent);
                        }
                        
                        // 在第一次循环中直接销毁
                        if (is_first) {
                            if (_left(parent) == node) {
                                my_printf("父节点左指针置空\n");
                                _left(parent) = nullptr;
                            } else {
                                my_printf("父节点右指针置空\n");
                                _right(parent) = nullptr;
                            }
                            _destroy_node(node);
                            --_size;
                        }

                        // 更新双黑节点
                        // node = parent;
                        // parent = _parent(node);
                    }
                }
            }
        }

        // 3. 只有一个子节点
        // node一定是黑色，子节点一定是红色，此时node可能是根节点
        my_printf("情况3\n");
        node_pointer child = nullptr;
        if (node->_left != nullptr) {
            child = node->_left;
        } else {
            child = node->_right;
        }

        // 直接替代并变色
        if (parent == _head) {
            // 删除的是根节点，需要移动root()
            _root() = child;
        } else if (_left(parent) == node) {
            _left(parent) = child;
        } else {
            _right(parent) = child;
        }

        _parent(child) = parent;
        _color(child) = _black;         // 子节点一定是红变黑

        // 销毁当前节点
        _destroy_node(node);
        --_size;
        return iterator(next._node);
    }

    /**
     * @brief 擦除元素
     */
    iterator erase(const_iterator first, const_iterator last)
    {
        for (const_iterator it = first; it != last; ) {
            it = erase(it);
        }
        return iterator(last._node);
    }

    /**
     * @brief 擦除元素
     */
    size_type erase(const key_type & key) {
        size_type result = 0;
        auto it = lower_bound(key);  // 查找第一个符合条件的元素

        while (it != end()) {
            if (key == _get_key(it._node->_data)) {
                // 找到匹配的元素，进行删除
                it = erase(it);;  // 更新迭代器
                ++result;
            } else {
                break;
            }
        }

        return result;
    }


    /**
     * @brief 交换内容
     */
    void swap(rb_tree & other)
    {
        std::swap(_head, other._head);
        std::swap(_size, other._size);
        std::swap(_comp, other._comp);
        std::swap(_get_key, other._get_key);
        if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
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
            throw std::out_of_range("rb_tree at out of range");
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
            throw std::out_of_range("rb_tree at out of range");
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
        size_type result = 0;
        auto lower = lower_bound(key);
        auto upper = upper_bound(key);
        for (auto it = lower; it != end() && it != upper; ++it) {
            if (_get_key(it._node->_data) == key) {
                ++result;
            }
        }
        return result;
    }

    /**
     * @brief 寻找带有特定键的元素
     */
    iterator find(const key_type & key)
    { return iterator(_find(key)); }

    /**
     * @brief 寻找带有特定键的元素
     */
    const_iterator find(const key_type & key) const
    { return const_iterator(_find(key)); }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<iterator, iterator> equal_range(const key_type & key)
    { return std::make_pair<iterator, iterator>(lower_bound(key), upper_bound(key)); }

    /**
     * @brief 返回匹配特定键的元素范围
     */
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    { return std::make_pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key)); }

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    iterator lower_bound(const key_type & key)
    { return iterator(_find_lower(key)); }    // 由重复插入和迭代器的移动可知，通过红黑树性质找到的第一个节点一定是迭代器范围中的第一个；如果没找到返回end()

    /**
     * @brief 返回指向首个不小于给定键的元素的迭代器
     */
    const_iterator lower_bound(const key_type & key) const
    { return const_iterator(_find_lower(key)); }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    iterator upper_bound(const key_type & key)
    { return iterator(_find_upper(key)); }

    /**
     * @brief 返回指向首个大于给定键的元素的迭代器
     */
    const_iterator upper_bound(const key_type & key) const
    { return const_iterator(_find_upper(key)); }

public:
    /**
     * @brief 获取根节点
     */
    node_pointer & _root() const
    { return _head->_parent; }

    /**
     * @brief 获取最小的节点
     */
    node_pointer & _leftmost() const
    { return _head->_left; }

    /**
     * @brief 获取最大的节点
     */
    node_pointer & _rightmost() const
    { return _head->_right; }

    /**
     * @brief 获取节点x的父节点
     */
    static node_pointer & _parent(const node_pointer x)
    { return x->_parent; }

    /**
     * @brief 获取节点x的左子节点
     */
    static node_pointer & _left(const node_pointer x)
    { return x->_left; }

    /**
     * @brief 获取节点x的右子节点
     */
    static node_pointer & _right(const node_pointer x)
    { return x->_right; }

    /**
     * @brief 获取节点x的值
     */
    static reference _data(const node_pointer x)    
    { return x->_data; }

    /**
     * @brief 获取节点x的键
     */
    static const key_type & _key(const node_pointer x)
    { return key_extractor()(_data(x)); }

    /**
     * @brief 获取节点x的颜色
     */
    static color_type & _color(const node_pointer x)
    { return x->_color; }

    /**
     * @brief 获取节点x子节点中的最小节点
     */
    static node_pointer _minimum(node_pointer x)
    { return node::minimum(x); }

    /**
     * @brief 获取节点x子节点中的最大节点
     */
    static node_pointer _maximum(node_pointer x)
    { return node::maximum(x); }

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
     * @details 节点默认为红色
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
     * @brief 恢复head为初始状态
     */
    void _init_head()
    {
        _root() = nullptr;          // 初始根节点为空，直到插入第一个节点
        _leftmost() = _head;
        _rightmost() = _head;
    }

    /**
     * @brief 初始化红黑树
     */
    void _init_rb_tree()
    {
        _head = _create_node();     // 红黑树的虚拟头节点的颜色为红，与红黑树的黑色根节点区分
        _init_head();
    }

    /**
     * @brief 后序遍历销毁二叉树
     */
    void _destroy_rb_tree(node_pointer p)
    {
        if (p == nullptr)
            return;
        
        _destroy_rb_tree(p->_left);
        _destroy_rb_tree(p->_right);
        _destroy_node(p);
    }

    /**
     * @brief 以某个节点为旋转点左旋
     */
    void _left_rotate(node_pointer x)
    {
        node_pointer parent = x->_parent;           // 父节点
        node_pointer right = x->_right;             // 右子节点
        node_pointer right_left = right->_left;     // 右子节点的左子节点

        // 右子节点的左子树成为x的右子树
        x->_right = right_left;
        if (right_left != nullptr) {
            right_left->_parent = x;
        }

        // x成为右子节点的左子节点
        right->_left = x;
        x->_parent = right;

        // 更新父节点
        right->_parent = parent;
        if (parent == _head) {
            // 说明x原来是根节点
            _root() = right;
        } else if (parent->_left == x) {
            parent->_left = right;
        } else {
            parent->_right = right;
        }
    }

    /**
     * @brief 以某个节点为旋转点右旋
     */
    void _right_rotate(node_pointer x)
    {
        node_pointer parent = x->_parent;           // 父节点
        node_pointer left = x->_left;               // 左子节点
        node_pointer left_right = left->_right;     // 左子节点的右子节点

        // 左子节点的右子树成为x的左子树
        x->_left = left_right;
        if (left_right != nullptr) {
            left_right->_parent = x;
        }

        // x成为左子节点的右子节点
        left->_right = x;
        x->_parent = left;

        // 更新父节点
        left->_parent = parent;
        if (parent == _head) {
            // 说明x原来是根节点
            _root() = left;
        } else if (parent->_left == x) {
            parent->_left = left;
        } else {
            parent->_right = left;
        }
    }

    /**
     * @brief 在插入节点p后重新平衡
     */
    void _rebalance(node_pointer new_node)
    {
        /** 
         * 当红黑树的性质被破坏时，需要调整，共有两种情况
         * 1. 当新节点为根节点时，违反了根节点为黑色的性质
         * 2. 当新节点的父节点为红色时，违反了不允许连续两个红色节点的性质
         */

        if (new_node == _root()) {
            // 违反了根节点为黑色的性质
            _color(new_node) = _black;
            return;
        }

        if (new_node->_parent->_color == _black) {
            // 不违反性质
            return;
        }

        // 违反了红黑树的不红红性质，需要调整
        node_pointer cur = new_node;
        node_pointer uncle = nullptr;
        while (cur != _root() && cur->_parent->_color == _red) {
            // 找到叔叔节点
            if (cur->_parent == cur->_parent->_parent->_left) {
                uncle = cur->_parent->_parent->_right;
            } else {
                uncle = cur->_parent->_parent->_left;
            }
            // 查看叔叔节点的颜色
            if (uncle != nullptr && uncle->_color == _red) {
                // 叔叔是红色，需要修改父亲、叔叔、爷爷的颜色，然后移动cur到爷爷，继续循环
                // 因为如果new_node违反了性质，那么父亲一定是红色，爷爷一定是黑色，变色后爷爷变成了红色，while判断条件还是一样的
                uncle->_color = _black;                 // 叔叔变成黑色
                cur->_parent->_color = _black;          // 父亲变成黑色
                cur->_parent->_parent->_color = _red;   // 爷爷变成红色
                cur = cur->_parent->_parent;            // 移动到爷爷
                // 爷爷由黑变红，可能违反性质，继续循环
            } else {
                // 不存在叔叔节点或叔叔是黑色，需要根据形状判断是LL，RR，LR还是RL
                if (cur->_parent->_left == cur) {
                    if (cur->_parent->_parent->_left == cur->_parent) {
                        // LL情况，直接将爷爷节点右旋
                        cur->_parent->_color = _black;          // 父亲节点变为黑色
                        cur->_parent->_parent->_color = _red;   // 爷爷节点变为红色
                        _right_rotate(cur->_parent->_parent);   // 右旋爷爷节点
                    } else {
                        // RL情况，先左旋父亲节点，再右旋爷爷节点
                        cur->_color = _black;                   // cur节点变为黑色
                        cur->_parent->_parent->_color = _red;   // 爷爷节点变为红色
                        _right_rotate(cur->_parent);            // 右旋父亲节点，变成RR情况
                        _left_rotate(cur->_parent);             // 左旋爷爷节点，此时cur已经旋转成为了爷爷节点的右子节点
                    }
                } else {
                    if (cur->_parent->_parent->_right == cur->_parent) {
                        // RR情况，直接将爷爷节点左旋
                        cur->_parent->_color = _black;          // 父亲节点变为黑色
                        cur->_parent->_parent->_color = _red;   // 爷爷节点变为红色
                        _left_rotate(cur->_parent->_parent);    // 左旋爷爷节点
                    } else {
                        // LR情况，先右旋父亲节点，再左旋爷爷节点
                        cur->_color = _black;                   // cur节点变为黑色
                        cur->_parent->_parent->_color = _red;   // 爷爷节点变为红色
                        _left_rotate(cur->_parent);             // 左旋父亲节点，变成LL情况
                        _right_rotate(cur->_parent);            // 右旋爷爷节点，此时cur已经旋转成为了爷爷节点的左子节点
                    }
                }
                break;  // 旋转后一定会满足性质，不需要再循环了
            }
        }

        // 保证根节点是黑色的，保护了违反性质且叔叔为红的情况下，移动到根节点后的变化
        _color(_root()) = _black;
    }

    /**
     * @brief 在parent处插入节点
     * @details new_node于emplace_xxx中临时创建，直接移动
     */
    iterator _emplace(node_pointer parent, node_pointer && new_node)
    {
        // 由于已经找到了插入位置，只需要处理边界情况
        if (parent == nullptr) {
            // 说明还没有根节点，new_node将成为根节点
            parent = _head;
            _root() = new_node;
        }

        // 处理需要修改最小和最大节点的情况
        if (parent == _head) {
            // 根节点，直接设置最小和最大节点，相当于设置了head的左右子节点
            _leftmost() = new_node;
            _rightmost() = new_node;
        } else if (_comp(_get_key(new_node->_data), _get_key(parent->_data))) {
            // 新节点小于原父节点
            _left(parent) = new_node;
            if (parent == _leftmost()) {
                // 新节点成为最小节点
                _leftmost() = new_node;
            }
        } else {
            // 新节点大于原父节点
            _right(parent) = new_node;
            if (parent == _rightmost()) {
                // 新节点成为最大节点
                _rightmost() = new_node;
            }
        }

        // 设置新节点
        _parent(new_node) = parent;
        _left(new_node) = nullptr;
        _right(new_node) = nullptr;
        // 注意新节点默认已经为红色

        // 重新平衡红黑树
        _rebalance(new_node);

        ++_size;
        return iterator(new_node);
    }

    /**
     * @brief 查找特定键的元素
     */
    node_pointer _find(const key_type & key) const
    {
        node_pointer cur = _root();
        // 从根节点开始查找
        while (cur != nullptr) {
            if (_comp(_get_key(cur->_data), key)) {
                // 当前节点的键小于目标key，向右寻找
                cur = _right(cur);
            } else if (_comp(key, _get_key(cur->_data))) {
                // 当前节点的键大于目标key，向左寻找
                cur = _left(cur);
            } else {
                // 返回找到的节点
                return cur;
            }
        }
        // 如果是nullptr，则返回_head表示末尾
        return _head;
    }

    /**
     * @brief 查找不小于特定键的最大节点
     */
    node_pointer _find_lower(const key_type & key) const
    {
        node_pointer cur = _root();
        node_pointer result = _head;
        while (cur != nullptr) {
            if (_comp(_get_key(cur->_data), key)) {
                // key小于当前节点，向右查找
                cur = _right(cur);
            } else {
                // key大于等于当前节点，更新，并向左查找
                result = cur;
                cur = _left(cur);
            }
        }
        return result;
    }

    /**
     * @brief 查找大于特定键的最小节点
     */
    node_pointer _find_upper(const key_type & key) const
    {
        node_pointer cur = _root();
        node_pointer result = _head;
        while (cur != nullptr) {
            if (_comp(key, _get_key(cur->_data))) {
                // key小于当前节点，向左查找
                result = cur;
                cur = _left(cur);
            } else {
                // key大于等于当前节点，向右查找
                cur = _right(cur);
            }
        }
        return result;
    }
};

// 非成员函数

template <
    class Key,
    class T,
    class Value,
    class ExtractKey,
    class Compare,
    class Allocator
> void swap(rb_tree<Key, T, Value, ExtractKey, Compare, Allocator> & lhs,
            rb_tree<Key, T, Value, ExtractKey, Compare, Allocator> & rhs)
{ lhs.swap(rhs); }

} // namespace wwstl

#endif // __WW_RB_TREE_H__