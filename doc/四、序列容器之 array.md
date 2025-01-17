# 序列容器之 array

## 一、array简介

array 是封装固定大小数组的容器，是 C++11 标准提出的容器。它维护一个 C 风格的数组`T[N]`作为其唯一非静态数据成员。和数组不同的是，数组会退化成`T*`用以指向数组的首位地址，但 array 不会。

array 是一个聚合类型，可以聚合初始化（列表初始化的一种）。因此根据聚合体的规定，array 没有构造和析构函数，或者说是隐式的。详情见[https://zh.cppreference.com/w/cpp/language/aggregate_initialization](https://zh.cppreference.com/w/cpp/language/aggregate_initialization)。

array 既拥有数组的性能和优点，也集成了 STL 容器在访问、算法上的优点。

```c++
int a[3] = {1, 2, 3};                   // C 风格数组

wwstl::array<int, 3> b = {1, 2, 3};     // array
```

## 二、array的实现

array 的完整实现位于[ww_array.h](../stl/ww_array.h)。

### 1. array的迭代器

array 的迭代器是一种随机访问迭代器，它维护一个指向 array 底层数组的指针。

这里`const_iterator`和`iterator`使用了一个小技巧，阅读代码可以发现，`const_iterator`中维护的指针并非`pointer`类型，而是又定义了一个`_Ptr`类型：

```c++
template <class T>
class _array_const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using self = _array_const_iterator<value_type>;

    using _Ptr = value_type*;   // 底层使用非const指针
};
```

为什么要这么做呢？首先我们考虑，如果我们使用`pointer`，那么在`const_iterator`中维护的就是`const value_type * _ptr`，这当然是符合语义的。但是在使用`iterator`时就会出现问题。

对于解引用的两个接口，`iterator`分别返回`reference`和`pointer`类型，这是非 const 的，而底层则维护 const 指针，需要使用`const_cast`来去除 const 属性，这样的实践并不好，违反了 const 设计的用意，且使得迭代器实现更复杂。

而在我们的实现中，`const_iterator`同样维护`value_type *`，而在解引用时，会隐式转换为`const value_type *`，不会出现问题。唯一需要注意的是，在`const_itrator`的构造函数中，我们有一个重载：

```c++
explicit _array_const_iterator(pointer ptr)
    : _ptr(const_cast<_Ptr>(ptr))
{ // 用指针ptr构造一个迭代器
}
```

它接受一个`const value_type *`类型的指针，然后使用`const_cast`来去除 const 属性，这样在解引用时就不会出现问题。这种情况是少见的，而当我们使用普通的`value_type *`来构造时，也能够匹配`const value_type *`。

接下来就是迭代器的接口，是简单的指针操作，无需赘述。需要注意的是，MSVC 在实现 STL 时，对代码的复用令作者大开眼界，体现在迭代器的设计中，它尽可能使用复用，绝不写任何重复的代码，以达到更容易维护的目的。

### 2. array的定义

array 的定义如下：

```c++
template <
    class T,
    std::size_t N
> class array;
```

它接受两个模板参数，第一个模板参数是元素类型，第二个模板参数是元素个数。也就是说，在声明 array 数组时，array 的大小就确定了，这点和 C 风格数组是一致的。

### 3. array的构造

由于 array 需要符合聚合体的规定，因此它的构造函数、析构函数都是隐式的，且赋值重载也是隐式的。

详情见[https://zh.cppreference.com/w/cpp/container/array](https://zh.cppreference.com/w/cpp/container/array)。

### 4. array的接口

注意：**接口中，与 const 语义有关的重载将不再介绍**

+ 元素访问

```c++
/**
 * @brief 带越界检查访问指定的元素
 */
reference at(size_type pos)
{
    if (pos >= N) {
        _throw_out_of_range();
    }
    return _data[pos];
}

/**
 * @brief 访问指定的元素
 */
reference operator[](size_type pos)
{
    return _data[pos];
}

/**
 * @brief 访问第一个元素
 */
reference front()
{
    return _data[0];
}

/**
 * @brief 访问最后一个元素
 */
reference back()
{
    return _data[N - 1];
}

/**
 * @brief 直接访问底层连续存储
 */
pointer data()
{
    return _data;
}
```

其中，`_throw_out_of_range()`是一个用于抛出异常的内部函数，其定义如下：

```c++
[[noreturn]] void _throw_out_of_range() const
{
    throw std::out_of_range("invalid array<T, N> subscript");
}
```

当我们调用`at`接口访问容器时，会检测索引是否有效，无效则抛出异常。

+ 迭代器

```c++
/**
 * @brief 返回指向起始的迭代器
 */
iterator begin() noexcept
{
    return iterator(_data);
}

/**
 * @brief 返回指向起始的逆向迭代器
 */
reverse_iterator rbegin() noexcept
{
    return reverse_iterator(end());
}
```

STL 容器中关于迭代器接口的实现大多类似，对于`begin()`接口，通过指针来构造一个迭代器，然后返回它的拷贝；对于`rbegin()`接口，则使用一个迭代器`end()`来构造。关于逆向迭代器的知识，详情见[三、迭代器](./三、迭代器.md)。

+ 容量

```c++
/**
 * @brief 检查容器是否为空
 */
constexpr bool empty() const noexcept
{
    return false;
}

/**
 * @brief 返回元素数
 */
constexpr size_type size() const noexcept
{
    return N;
}

/**
 * @brief 返回最大容量
 */
constexpr size_type max_size() const noexcept
{
    return size();
}
```

关于 array 的容量，由于数组初始化 N 一定是大于0的，所以容器必不为空。那么如果使用模板时传入`N = 0`怎么办？根据 STL 要求，需要提供一个模板特化的类，来满足这个要求：

```c++
template <class T>
class array<T, 0>;
```

这是一个 array 模板的偏特化，有意思的点在于，维护的是一个`T[1]`，因为 C++ 不允许声明空数组。它的具体行为见代码，这里不详细介绍。

+ 操作

```c++
/**
 * @brief 以指定值填充容器
 */
void fill(const value_type & value)
{
    std::fill_n(_data, N, value);
}

/**
 * @brief 交换内容
 */
void swap(array<T, N> & other) noexcept
{
    std::swap(_data, other._data);
}
```

array 提供两种操作接口：

一个是`fill`，调用了标准库算法中的`std::fill_n`，它能够以指定值从指定位置开始填充指定个数的元素，事实上该函数是一个模板函数，接受迭代器参数，但是原生指针正是一种随机访问迭代器，因此匹配该算法。

另一个是`swap`，调用标准库算法中的`std::swap`，它能够交换两个指针的数据，也就是交换 array 的底层数组的内容。

+ 非成员函数

```c++
/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> T & get(array<T, N> & a)
{ return a[I]; }

/**
 * @brief 获取array中第I个元素
 */
template <
    std::size_t I,
    class T,
    std::size_t N
> T && get(array<T, N> && a)
{ return std::move(a[I]); }

// 辅助类

template <
    std::size_t I,
    class T
> class tuple_element;

template <
    std::size_t I,
    class T,
    std::size_t N
> class tuple_element<I, array<T, N>>
{
public:
    using type = T;
};

template <class T>
class tuple_size;

template<
    class T,
    std::size_t N
> class tuple_size<array<T, N>> : std::integral_constant<std::size_t, N>
{
};
```

根据要求，还需要提供一些非成员函数，它们是一系列函数的重载，包括了 STL 通用的比较运算符、交换重载，还有 array 特有的函数，用于提取 array 的数据。

其中，`tuple_element`用于获取 array 的元素类型，`tuple_size`用于获取 array 的大小。

## 三、思考

### 1. 新特性

C++11 推出了一个名为**基于范围的 for 循环**的特性，我们常使用它来遍历容器：

```c++
for (auto & it : container) {
    // do something
}
```

这实际上是调用了容器的迭代器接口`begin()`和`end()`，因此只要我们的容器完成了迭代器接口的实现，就可以使用这种 for 循环。
