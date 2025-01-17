# 序列容器之 vector

## 一、vector简介

vector 是一种封装动态数组的容器，它可以动态地存储一组元素，能够高效地进行随机访问和动态扩展。与 C 风格的数组不同，vector 可以在运行时自动调整其大小，支持灵活的内存管理。

vector 的特点是：

+ 动态大小：vector 会根据元素的插入或删除自动调整其容量。它在需要时会自动分配更多的内存空间，因此可以容纳更多元素
+ 连续内存布局：vector 内部的数据存储在连续的内存区域中，类似于 C 风格的数组。这意味着它支持高效的随机访问操作，且可以与 C 函数兼容
+ 快速随机访问：由于元素存储在连续的内存块中，可以通过下标访问元素，时间复杂度为 O(1)
+ 灵活性：可以高效地在容器尾部添加元素，且支持按需扩展空间，但在中间插入或删除元素的效率较低，因为需要移动元素

## 二、vector的实现

vector 的完整实现位于[ww_vector.h](../stl/ww_vector.h)。

### 1. vector的迭代器

作为指向数组的迭代器，vector 的迭代器和 array 的迭代器完全一致，详情见[四、序列容器之 array](./四、序列容器之%20array.md)，此处不再详述。

### 2. vector的定义

```c++
template <
    class T,
    class Allocator = wwstl::allocator<T>
> class vector;
```

vector 模板类接受两个模板参数，第一个模板参数是元素类型，第二个模板参数是 vector 使用的分配器，如果不提供分配器，则使用默认的分配器`wwstl::allocator<T>`。

### 3. vector的结构

```c++
pointer _start;             // 起始元素指针
pointer _finish;            // 结束元素的下一位指针
pointer _end_of_storage;    // 空间末尾指针
allocator_type _allocator;  // 分配器
```

vector 维护三个指针，分别指向所持有的内存区域的**起始位置**，**结束元素的下一个位置**和**空间末尾的位置**。

拥有这三个指针，我们能很方便的计算出 vector 中元素的个数（大小），和 vector 能持有的最大元素个数（容量）

```c++
size_type size = _finish - _start;                  // 元素个数
size_type capacity = _end_of_storage - _start;      // 容量
```

### 4. vector的接口

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
 * @brief 访问第一个元素
 */
reference front()
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
 * @brief 直接访问底层连续存储
 */
pointer data()
{
    return _start;
}
```

vector 的元素访问与 array 一致，都是直接通过指针访问底层数组。

+ 迭代器

```c++
/**
 * @brief 返回指向末尾的迭代器
 */
iterator end() noexcept
{ return iterator(_finish); }
```

vector 的迭代器接口与 array 几乎一致，除了在返回尾迭代器`end()`时，指向的是最后一个元素的后一个位置，即`_finish`指向的位置，而不是`_end_of_storage`。

+ 容量

```c++
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
 * @brief 返回当前存储空间能够容纳的元素数
 */
size_type capacity() const noexcept
{
    return _end_of_storage - _start;
}
```

对于容量的计算和判断，都是指针的简单计算。

但是还有两个接口`reserve()`和`shrink_to_fit()`，用于动态扩容和缩容，这两个接口会修改 vector 的大小，重新分配内存空间，从而达到改变 vector 的大小的效果，实现起来更为复杂。

```c++
/**
 * @brief 预留存储空间
 */
void reserve(size_type new_cap)
{
    if (new_cap > capacity()) {
        if (new_cap > max_size()) {
            _throw_length_error();
        }
        _reallocate(new_cap);
    }
}

/**
 * @brief 通过释放未使用的内存减少内存的使用
 */
void shrink_to_fit()
{
    _reallocate(size());
}
```

`reserve()`接口接受一个大小作为参数，如果该大小大于当前的容量，就会重新分配内存空间，大小等于`new_cap`。

`shrink_to_fit()`接口会释放未使用的内存空间，减少内存的使用，释放后，`_end_of_storage`将和`_finish`相等，这里重新分配空间，其大小恰好等于`size()`。

这里使用了一个内部函数`_reallocate(size_type new_cap)`来实现内存的重新分配：

```c++
/**
 * @brief 重新分配内存
 * @details 需要保证 new_cap >= size
 */
void _reallocate(size_type new_cap)
{
    size_type old_size = size();                                        // 计算原来的大小
    pointer new_start = _allocator.allocate(new_cap);                   // 分配新的内存
    std::uninitialized_copy(begin(), end(), iterator(new_start));       // 将原来的数据复制到新的内存
    _clean();                                                           // 销毁原来的内存
    _set_new_space(new_start, old_size, new_cap);                       // 设置新的空间
}
```

当我们调用这个函数的时候，需要保证`new_cap`是大于等于`size()`的，在该函数中，首先计算出原来的大小，然后分配新的内存空间，将原来的数据复制到新的内存空间中，然后销毁原来的内存空间，最后设置新的内存空间。

其中，`_clean()`是一个用于销毁空间的函数，它会遍历原来的内存空间，销毁每个元素，然后释放内存空间：

```c++
/**
 * @brief 清空vector为初始状态
 */
void _clean()
{
    if (_start != nullptr) {
        for (size_type i = 0; i < size(); ++i) {
            _allocator.destroy(_start + i);
        }
        _allocator.deallocate(_start, capacity());
        _start = nullptr;
        _finish = nullptr;
        _end_of_storage = nullptr;
    }
}
```

为什么不使用`clear()`函数呢？因为`clear()`函数只是销毁所有的元素，而不会释放已经申请的内存空间。

`_set_new_space()`是一个用于设置新的内存空间的函数，它会设置起始指针`_start`，结束指针`_finish`和空间末尾指针`_end_of_storage`的值：

```c++
/**
 * @brief 设置新空间指针
 */
void _set_new_space(pointer new_start, size_type new_size, size_type new_cap)
{
    _start = new_start;
    _finish = new_start + new_size;
    _end_of_storage = new_start + new_cap;
}
```

+ 修改器

```c++
/**
 * @brief 清除内容
 */
void clear() noexcept
{
    _destroy_n(_start, size());
    _finish = _start;
}
```

`clear()`接口只销毁容器中的所有元素，而不释放已经申请的内存空间，因此我们遍历所有的元素并销毁，然后将`_finish`设置为`_start`。

这里调用了`_destroy_n()`函数来销毁元素，它会遍历所有的元素并销毁它们：

```c++
/**
 * @brief 销毁从某个位置开始的count个元素
 */
void _destroy_n(pointer start, size_type count)
{
    for (size_type i = 0; i < count; ++i) {
        _allocator.destroy(start + i);
    }
}
```

`_destroy_n()`函数接受一个起始指针和一个元素个数作为参数，遍历从起始指针开始的`count`个元素，销毁它们。

接下来是 vector 的插入操作，考虑一个更通用的插入接口，我们利用 C++11 的新特性，构建一个支持万能引用参数包的接口：

```c++
/**
 * @brief 在pos位置插入count个元素
 */
template <class... Args>
iterator _emplace_n(const_iterator pos, size_type count, Args&&... args)
{
    difference_type offset = pos - begin();
    _check_newsize(count);     // 判断是否需要扩容
    std::move_backward(begin() + offset, end(), end() + count);
    std::fill_n(begin() + offset, count, std::forward<Args>(args)...);
    _finish += count;
    return begin() + offset;
}
```

这里首先计算插入位置的偏移量，然后调用`_check_newsize()`函数判断插入`count`个元素是否需要扩容，如果需要扩容，在该函数中已经完成扩容，然后使用`std::move_backward()`函数将原来的元素向后移动`count`个位置，再使用`std::fill_n()`函数填充新的元素，最后更新`_finish`指针，返回插入位置的迭代器。这里使用偏移量是因为，如果在检查扩容的过程中发生了扩容，那么原来的迭代器就全部失效了，只能通过新的迭代器偏移到达插入点。

下面介绍一下内部函数`_check_newsize()`：

```c++
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
```

它是 vector 实现动态数组的绝对核心，判断插入`count`个元素是否需要重新分配内存，如果需要，那么调用`reserve()`函数来扩容。

**面试常问**：在这个函数中，我们采用了两倍扩容的策略，当数组需要扩容时，将其扩大为原来的两倍，在某些实现中，可能采用其他的扩容策略。因此，为了避免频繁的扩容，我们如果能够预知 vector 将要储存的数据量，应当在使用前调用`reserve()`直接扩容到接近目标大小。

随后，我们所有的插入操作，都由`_emplace_n()`函数来完成，它的实现如下所示：

```c++
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
    class = std::enable_if_t<wwstl::is_iterator_v<InputIterator>>
> iterator insert(const_iterator pos, InputIterator first, InputIterator last)
{
    size_type n = std::distance(begin(), pos);
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
```

在上述接口中，有一个特殊的接口`iterator insert(const_iterator pos, InputIterator first, InputIterator last)`，它的插入逻辑与`_emplace_n`相似但无法兼容，因此单独实现，而在它的模板参数中，我是用了`class = std::enable_if_t<wwstl::is_iterator_v<InputIterator>>`。这是利用了 C++11 的 SFINAE 特性，全称：Substitution Failure Is Not An Error，它的理念是，如果一个模板参数的类型满足某个条件，那么就可以使用这个模板参数，否则就不可以使用。在这里，我限制了`InputIterator`的类型，只有满足`wwstl::is_iterator_v<InputIterator>`的类型，即是一个迭代器类型才可以使用。

如果我们不使用这个条件会发什么？我们在调用`iterator insert(const_iterator pos, size_type count, const value_type & value)`，且`value_type`和输入的`count`类型一致时（如int），编译器可能会认为调用了`iterator insert(const_iterator pos, InputIterator first, InputIterator last)`重载：

```c++
wwstl::vector<int> v;
v.insert(v.begin(), 10, 1);         // 10,1既可能被认为是count，value；也可能被认为是first，last
```

关于接口的复用，这里`insert()`接口无论是复用`emplace()`还是内部函数`_emplace_n()`，都是一样的，只是设计理念不同。

关于 vector 的擦除，就简单很多：

```c++
/**
 * @brief 擦除元素
 */
iterator erase(const_iterator pos)
{
    pointer p = pos._ptr;
    std::move(p + 1, _finish, p);                       // 将pos后的元素整体向前移动
    _allocator.destroy(_finish);                        // 销毁最后多出来的元素
    --_finish;                                          // 调整_finish
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
```

上述接口的实现，就是把要删除元素之后的元素向前移动，将要被删除的部分覆盖，这个时候，最后的 n 个元素就是无效的了，直接销毁，并调整指针即可。

接下来的`push_back()`和`pop_back()`接口，就是简单的调用`emplace_back()`和`erase()`：

```c++
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
```

`resize()`接口也是根据具体逻辑，复用已经实现的接口：

```c++
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
```

当`count`小于当前的元素个数时，调用`erase()`接口删除多出来的元素，当`count`大于当前的元素个数时，调用`insert()`接口插入多个元素，如果没有指定插入值，那么使用默认值`value_type()`。

+ 赋值

赋值是一个复杂的操作，需要根据不同的元素个数决定操作的逻辑：

```c++
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
```

首先我们比较新 vector 的大小与原来的容量：

+ 如果新的大小大于原来的容量，那么我们直接销毁旧空间，重新分配新空间并使用`std::uninitialized_fill_n()`函数进行初始化，最后调整三个指针。
+ 如果超过大小但不超过容量，那么不需要重新分配空间，只需要将原来的区间赋值（因为已经构造过，可以直接赋值），剩下的长度使用`std::uninitialized_fill_n()`函数进行初始化，最后调整指针。
+ 如果比原来的大小还要小，则先赋值，再将多余的区间销毁，调整指针即可。

剩下的赋值接口思路与之一致。

当我们设计赋值重载时，需要考虑左值引用和右值引用的不同语义：

```c++
/**
 * @brief 将值赋给容器
 */
vector & operator=(const vector & other)
{
    if (this != &other) {
        assign(other.begin(), other.end());
        if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
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
        _start = other._start;
        _finish = other._finish;
        _end_of_storage = other._end_of_storage;
        if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
            _allocator = std::move(other._allocator);
        }
        // 清空原来的资源
        other._start = nullptr;
        other._finish = nullptr;
        other._end_of_storage = nullptr;
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
```

对于左值引用的版本，我们不应该影响`other`的状态，因此调用`assign`将其赋值到本数组中；对于右值引用的版本，我们需要接管`other`的资源，因此直接销毁旧有的空间，然后移动新的资源到本数组中，在移动完毕后，将`other`的指针置空。

最后，对于初始化列表的赋值，我们直接调用`assign`接口进行赋值即可。

## 三、思考

### 1. std::move的作用

在 C++11 中，我们可能会使用`std::move`来将左值转换为右值引用，它将资源所有权移动到新的对象，而不是拷贝，这样做能够提高程序运行的性能。但是在使用`std::move`时，其实并没有实现资源转移的操作，它仅仅是一个标记，告诉编译器目标对象可以安全地将其资源转移出去。

```c++
std::string str1 = "Hello";
std::string str2 = std::move(str1); // 资源转移，str1 变为空
```

需要注意的是，我们在实现移动语义的时候，被移动的对象处于什么状态取决于我们的实现，一般将其恢复为初始状态，即空。

### 2. std::move和std::copy

`std::copy()`是一个拷贝算法，用于将一个范围内的元素逐个复制到另一个范围。拷贝时，它会调用元素的拷贝构造函数或拷贝赋值运算符，原始数据保持不变。

`std::move()`是一个移动算法，用于将一个范围内的元素逐个移动到另一个范围。移动时，它会调用元素的移动构造函数或移动赋值运算符，原始数据会被移动，处于未定义的状态，但通常是有效的。它一般用于容器内元素的转移。

对于 copy 和 move 算法，有两种移动方式，以 move 为例，存在`std::move()`和`std::move_backward()`两种移动方式。前者用于数据向前移动，后者用于数据向后移动，它们在行为上的区别是，`move()`从前向后移动，而`move_backward()`从后向前移动，对于不同的移动方向，应该选择相应的移动操作，这样可以避免在源范围和目标范围存在重叠时，移动导致数据失效的问题。

详情见[https://zh.cppreference.com/w/cpp/algorithm/move](https://zh.cppreference.com/w/cpp/algorithm/move)和[https://zh.cppreference.com/w/cpp/algorithm/move_backward](https://zh.cppreference.com/w/cpp/algorithm/move_backward)
