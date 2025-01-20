# 序列容器之 deque

## 一、deque简介

我们在使用 vector 时，发现它只提供了`push_back`、`pop_back`等在尾端操作的接口，而没有提供在头部操作的接口，因为在 vector 的头部操作的时间复杂度将是 O(n) ，这是不可接受的，它是一个某种意义上单向的容器；而本节介绍的 deque，即双端队列，是一种双向的容器，它能够以 O(1) 的时间复杂度从头和尾部进行操作。

![deque operation](./img/deque%20operation.png)

## 二、deque的实现

deque 的完整实现位于[ww_deque.h](../stl/ww_deque.h)。

### 1. deque的结构

为了拥有在双端操作的能力，deque 的结构异常复杂，它由以下部分组成:

+ 缓冲区：缓冲区是一个长度固定的数组，用于储存一定量的数据
+ 中控器：中控器是一个数组，存储指向缓冲区的指针

deque 的数据储存在缓冲区中，由中控器将它们连接起来，以达到看起来数据是连续的假象，实际上缓冲区之间是没有连接的，完全由中控器负责维护。

![deque structure](./img/deque%20structure.png)

从图中可以看到，deque 的前后端都有空闲的空间，因此 deque 在遇到缓冲区空间不足时，可以方便地自动扩展缓冲区，然后添加到中控器中；而不是像 vector 那样，重新分配内存再销毁旧内存来实现扩展，因此 deque 没有 vector 那样“容量”的概念，也不提供`capacity()`接口。

当然，中控器也会出现耗尽的情况，此时只要重新分配一块更大的内存，然后将缓冲区移动过去，然后销毁旧中控器即可，其性能损耗相比 vector 实在是小了很多。

对于 deque 复杂的数据结构，它的实现相当困难，我们将通过实现 deque 的迭代器，来理解 deque 的结构。

### 2. deque的迭代器

deque 作为“内存连续”的双端队列，其迭代器理所当然是随机访问迭代器，其整体结构和前文中 array、vector 的迭代器有所不同，它维护了四个指针：

```c++
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
};
```

它的结构如图所示：

![deque iterator](./img/deque%20iterator.png)

通过`_first`和`_last`来描述当前缓冲区的头尾，通过`_cur`来描述迭代器所指向的数据，通过`_node`来描述当前缓冲区在中控器中的位置。因为中控器是真正连续的内存，所以通过移动`_node`可以很容易地跳转到其他缓冲区中。

当迭代器向后移动时：

```c++
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
```

当`_cur`指针移动到缓冲区尾部，迭代器需要跳转到下一个缓冲区的头部，这时需要调用`_set_node()`函数来移动`_node`指针，然后将`_cur`指针移动到下一个缓冲区的起始位置。

`_set_node()`函数是一个内部函数，实现如下：

```c++
/**
 * @brief 设置当前迭代器指向的缓冲区
 */
void _set_node(map_pointer new_node)
{
    _node = new_node;
    _first = *_node;
    _last = _first + _buffer_size();
}
```

在设置`_last`时，我们是通过计算`_first`的偏移来实现的，这里调用的`_buffer_size()`函数是一个内部函数，它的实现如下：

```c++
/**
 * @brief 获取缓冲区的大小
 * @details 在本实现中设置为8个元素大小
 */
size_type _buffer_size() const noexcept
{ return 8 * sizeof(value_type); }
```

在本实现中，deque 的每个缓冲区，都能存储8个元素，因此`_buffer_size()`函数的返回值为`8 * sizeof(value_type)`。

这样我们就实现了迭代器的向后移动。当我们向前移动时，逻辑与之类似：

```c++
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
```

需要注意的是，哪怕是在迭代器中，头部和尾部的概念依然符合 STL 左闭右开的设计理念。

还有一个特殊的接口是比较运算符重载：

```c++
bool operator<(const self & other) const
{
    if (_node == other._node) {
        return _cur < other._cur;
    }
    return _node < other._node;
}
```

在 deque 的情况下，迭代器的比较操作会比较两个迭代器所指向的缓冲区，如果是同一个缓冲区，则比较两个迭代器指向的元素是否相等，如果不是同一个缓冲区，则直接比较缓冲区。

### 3. deque的定义

```c++
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class deque;
```

deque 的模板参数和前两个序列容器一致。

在 deque 的嵌套类型定义中，有两个特殊的定义：

```c++
using map_pointer = value_type**;   // 中控器指针类型
using map_allocator_type = typename allocator_type::template rebind<pointer>::other;    // 中控器分配器类型
```

第一个是中控器指针类型，它是一个指向指针的指针，指向的是缓冲区的指针。

第二个是中控器分配器类型，它是一个用`rebind`重新绑定了类型的分配器类型，注意它的特殊写法，其中，`typename`用于表明这是一个类型，`template`用于表明后面的部分是模板，而不是到类中寻找静态成员函数。

### 3. deque的结构

实现了 deque 的迭代器，下面开始实现 deque 的具体细节。首先是 deque 的底层数据：

```c++
iterator _start;                    // 起始位置迭代器，指向头部
iterator _finish;                   // 结束位置迭代器，指向尾部的下一个位置
map_pointer _map;                   // 中控器指针
size_type _map_size;                // 中控器大小
allocator_type _allocator;          // 元素分配器
map_allocator_type  _map_allocator; // 中控器分配器
```

和 vector 不同的是，这里直接维护了两个迭代器作为起始和末尾，而不是使用指针，因为 deque 复杂的结构，直接使用迭代器有助于理解和后续实现。

同时，我们构造了两个不同的分配器，一个用于管理元素，一个专门用于管理中控器的内存。

### 4. deque的实现

deque 的实现中，有相当多的内部函数，我们先介绍最基础的关于内存管理的函数：

```c++

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
```

这些函数是关于申请、释放内存的辅助函数，在后续的实现中会使用到。

接下来是接口：

+ 元素访问

```c++
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
 * @brief 访问指定的元素
 */
reference operator[](size_type pos)
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
 * @brief 访问最后一个元素
 */
reference back()
{
    return *(--end());
}
```

由于我们底层直接维护了迭代器，这里只需要解引用即可。

+ 迭代器

```c++
/**
 * @brief 返回指向起始的迭代器
 */
iterator begin() noexcept
{
    return _start;
}

......
```

迭代器相关接口更加简单，直接返回迭代器即可，不需要进行任何操作。

+ 容量

```c++
/**
 * @brief 检查容器是否为空
 */
bool empty() const noexcept
{ return _start == _finish; }

/**
 * @brief 返回元素数
 */
size_type size() const noexcept
{ return _finish - _start; }

/**
 * @brief 返回可容纳的最大元素数
 */
size_type max_size() const noexcept
{ return std::numeric_limits<size_type>::max() / sizeof(value_type); }

/**
 * @brief 通过释放未使用的内存减少内存的使用
 */
void shrink_to_fit() noexcept
{ // TODO
}
```

对于容量接口，只是简单的比较和计算。

这里需要注意的是，`shrink_to_fit()`是一个非强制实现的接口，实现与否取决于实现，详情见[https://zh.cppreference.com/w/cpp/container/deque/shrink_to_fit](https://zh.cppreference.com/w/cpp/container/deque/shrink_to_fit)。本实现中忽略该接口。

+ 修改器

我们将从插入和删除两个大方向开始，逐步解析接口。

首先是插入，根据插入数量的不同，我们可以将其分为两种情况：**插入一个元素**和**插入多个元素**。

对于插入一个元素的情况，作者参考了《STL源码剖析》中的做法，又分为**在头部插入**，**在尾部插入**和**在中间插入**三种情况：

```c++
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
```

这样的设计是因为可以复用`emplace_front()`和`emplace_back()`这两个函数，减少代码量。下面展开讲解这两个函数：

```c++
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
```

对于`emplace_front`函数，首先判断当前位置是否在缓冲区的头部，如果不在，说明可以直接将当前位置向前移动，然后原位构造。如果在，则需要跳跃缓冲区，然后再原位构造。但是在跳跃缓冲区的时候，会遇到一个问题：前面可能没有缓冲区了，因此我们在`_emplace_front()`中，将会对中控器进行检测，如果前面的中控器耗尽，将会调整或扩展中控器。

```c++
/**
 * @brief 在头部插入元素的辅助函数
 * @details 当需要跳跃缓冲区时调用该函数
 */
template <class... Args>
void _emplace_front(Args&&... args)
{
    // 判断前面是否需要扩展
    _reserve_map_at_front();
    // 申请内存
    *(_start._node - 1) = _allocate_node();
    // 需要先移动到上一个缓冲区的最后一个位置，再构造元素
    _start._set_node(_start._node - 1);
    _start._cur = _start._last - 1;
    _allocator.construct(_start._cur, std::forward<Args>(args)...);
}
```

我们使用这个内部函数来辅助插入，首先调用`_reserve_map_at_front`来扩展中控器，然后申请一个缓冲区的内存，将缓冲区连接到中控器中，最后移动到上一个缓冲区的最后一个位置，然后原位构造。

```c++
/**
 * @brief 在需要时于头部扩展中控器
 */
void _reserve_map_at_front(size_type n = 1)
{
    if (n > _start._node - _map) {
        _reallocate_map(n, true);
    }
}
```

在这个内部函数中，我们比较中控器前面还有多少空间，如果不足`n`个，则调用`_reallocate_map()`来扩展中控器，这个函数是 deque 的核心函数之一，用于维护中控器的内存：

```c++
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
```

在这个实现中，我们将其分为**调整**和**重新分配**两种情况，当此时中控器空闲空间还足够，只是在前面或者后面不太够用了，只需要将整个中控器前后移动，平衡前后的空闲空间即可；当空闲空间不够时，则需要申请新的足够大的内存空间，然后拷贝数据到新的中控器上。

```c++
if (is_front) {
    new_start = new_map + (new_map_size - new_num_nodes) / 2 + n;
} else {
    new_start = new_map + (new_map_size - new_num_nodes) / 2;
}
```

我们在调整或扩容时，使用这样的策略：因此如果是在头部调整或扩容，则需要一半的空间加上需要`n`个需要使用的空间，这样在插入完毕后，前面依然还有一半的空间；如果是在后面调整或扩容，则只需要一半的空间。这样的策略是为了方便头部的操作，毕竟在使用双端队列的情况下，需要考虑在头部操作的频率。

最后，设置迭代器的起始和末尾节点即可。这样我们就完成了整个在头部插入单个元素的流程，并捋清了全部的细节。

![deque reallocate](./img/deque%20reallocate.png)

当我们在尾部插入元素时，逻辑和上面类似：

```c++
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
```

当不在末尾时，直接构造并移动到下一个位置即可，如果在末尾时，需要跳跃缓冲区，因此要调用`_emplace_back()`辅助插入。

```c++
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
 * @brief 在需要时于尾部扩展中控器
 */
void _reserve_map_at_back(size_type n = 1)
{
    // 如果后备的缓冲区不够，则需要重新分配中控器
    if (n + 1 > _map_size - (_finish._node - _map)) {
        _reallocate_map(n, false);
    }
}
```

其逻辑和`_emplace_front()`类似。这样我们完成了插入单个元素中的前两种简单情况。

当插入位置不在头部或尾部时，就会遇到一个大问题，需要移动一些元素来空出位置用于插入，因此我们调用`_emplace()`函数来判断移动元素的方式并完成插入：

```c++
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
```

该函数的策略其实很简单，就是移动更少的元素：首先计算该位置距离头部有多远，然后判断是前面的元素多，还是后面的元素多，然后根据判断结果整体移动这一部分，最后在该位置插入元素即可。

需要注意的，在函数中我们使用`push_back()`和`push_front()`函数来插入一个元素，这是因为缓冲区中未到达的地方起始是没有初始化的，仅仅申请了空间，如果我们不初始化，那么后续调用`std::copy()`和`std::copy_backward()`时会出现错误，因为它们要求拷贝的目的地已经完成初始化。

到这里我们就完成了一整个插入单个元素的流程。对于插入多个元素，因为不再需要复用逻辑，我们没有必要再分为**头部插入**，**尾部插入**和**中间插入**三种情况了，直接调用`_emplace_n()`插入即可：

```c++
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
        std::fill_n(_start, count, value);                                  // 填充数据
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
```

其整体思路和上面单个元素的是一样的。

对于插入迭代器区间的重载版本，也属于插入多个元素的范畴，移动元素、扩容的方式是一样的，但最后插入元素的行为不同：

```c++
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
```

到这里我们拥有了所有插入方法的思路和工具，下面使用它们完成所有相关接口：

```c++
/**
 * @brief 插入元素
 */
iterator insert(const_iterator pos, const value_type & value)
{ return emplace(pos, value); }

/**
 * @brief 插入元素
 */
iterator insert(const_iterator pos, value_type && value)
{ return emplace(pos, std::move(value)); }

/**
 * @brief 插入元素
 */
iterator insert(const_iterator pos, size_type count, const value_type & value)
{ return _emplace_n(pos, count, value); }

/**
 * @brief 插入元素
 */
template <
    class InputIt,
    class = std::enable_if_t<wwstl::is_iterator_v<InputIt>>
> iterator insert(const_iterator pos, InputIt first, InputIt last)
{ return _emplace_it(pos, first, last); }

/**
 * @brief 插入元素
 */
iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
{ return insert(pos, ilist.begin(), ilist.end()); }

/**
 * @brief 将元素添加到容器末尾
 */
void push_back(const value_type & value)
{ emplace_back(value); }

/**
 * @brief 将元素添加到容器末尾
 */
void push_back(value_type && value)
{ push_back(value); }

/**
 * @brief 插入元素到容器起始
 */
void push_front(const value_type & value)
{ emplace_front(value); }

/**
 * @brief 插入元素到容器起始
 */
void push_front(value_type && value)
{ push_front(value); }
```

接下来考虑删除操作，删除操作相对简单，因为无论如何，删除元素不可能需要扩容了。

```c++
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
```

当删除头部和尾部的元素时，只需要考虑是否需要跳跃到其他缓冲区的情况，然后销毁即可。

当删除中间的元素时，也可以通过移动某一部分元素，然后删除头部或尾部，这是个很妙的思路：

```c++
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
```

根据这个思路，作者模仿实现了删除多个元素的版本：

```c++
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
```

接下来是剩余的修改器接口：

```c++
/**
 * @brief 改变存储元素的个数
 */
void resize(size_type count)
{ resize(count, value_type()); }

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
```

根据当前长度判断，调用`erase()`删除多余元素，还是调用`insert()`插入需要的元素，如果没有指定插入值，那么使用默认值`value_type()`。

+ 赋值

关于赋值接口，都是先销毁原来的元素，然后再调用已经实现的接口插入新的元素。值得注意的只有移动赋值接口：

```c++
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
```

当我们移动时，使用`std::move(other)`来移动迭代器，但由于迭代器并没有实现移动赋值，且迭代器的成员变量都是裸指针，编译器会默认生成移动赋值将指针拷贝，但没有处理被移动的元素，所以需要手动将`other`的两个迭代器恢复成默认状态。

```c++
/**
 * @brief 清空迭代器
 */
void _clear() noexcept
{
    _cur = _first = _last = nullptr;
    _node = nullptr;
}
```
