#ifndef __WW_QUEUE_H__
#define __WW_QUEUE_H__

#include "ww_deque.h"
#include "ww_vector.h"
#include "ww_algorithm.h"

namespace wwstl
{

/**
 * @brief queue
 * @link https://zh.cppreference.com/w/cpp/container/queue
 * @details 队列默认以deque作为底层容器
 */
template <
    class T,
    class Container = wwstl::deque<T>
> class queue
{
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;       // 基础容器

public:
    queue()
        : queue(container_type())
    {
    }

    explicit queue(const container_type & cont)
        : c(cont)
    {
    }

    explicit queue(container_type && cont)
        : c(std::move(cont))
    {
    }

    template <class Alloc>
    explicit queue(const Alloc & alloc)
        : c(alloc)
    {   // TODO
        // 这里需要底层容器知分配，才能参与重载决议
    }

    template <class Alloc>
    queue(const container_type & cont, const Alloc & alloc)
        : c(cont, alloc)
    {
    }

    template <class Alloc>
    queue(container_type && cont, const Alloc & alloc)
        : c(std::move(cont), alloc)
    {
    }

    template <class Alloc>
    queue(const queue & other, const Alloc & alloc)
        : c(other.c, alloc)
    {
    }

    template <class Alloc>
    queue(queue && other, const Alloc & alloc)
        : c(std::move(other.c), alloc)
    {
    }

    ~queue() = default;

public:
    // 元素访问

    /**
     * @brief 访问第一个元素
     */
    reference front()
    { return c.front(); }

    /**
     * @brief 访问第一个元素
     */
    const_reference front() const
    { return c.front(); }

    /**
     * @brief 访问最后一个元素
     */
    reference back()
    { return c.back(); }

    /**
     * @brief 访问最后一个元素
     */
    const_reference back() const
    { return c.back(); }

    // 容量

    /**
     * @brief 检查容器适配器是否为空
     */
    bool empty() const
    { return c.empty(); }

    /**
     * @brief 返回元素数
     */
    size_type size() const
    { return c.size(); }

    // 修改器

    /**
     * @brief 向队列尾部插入元素
     */
    void push(const value_type & x)
    { c.push_back(x); }

    /**
     * @brief 向队列尾部插入元素
     */
    void push(value_type && x)
    { c.push_back(std::move(x)); }

    /**
     * @brief 在尾部原位构造元素
     */
    template <class... Args>
    void emplace(Args&&... args)
    { c.emplace_back(std::forward<Args>(args)...); }

    /**
     * @brief 移除首个元素
     */
    void pop()
    { c.pop_front(); }

    /**
     * @brief 交换内容
     */
    void swap(queue & other) noexcept
    { c.swap(other.c); }
};

// 非成员函数

template <
    class T,
    class Container
> bool operator==(const queue<T, Container> & x, const queue<T, Container> & y)
{ return x.c == y.c; }

template <
    class T,
    class Container
> bool operator!=(const queue<T, Container> & x, const queue<T, Container> & y)
{ return !(x == y); }

template <
    class T,
    class Container
> bool operator<(const queue<T, Container> & x, const queue<T, Container> & y)
{ return x.c < y.c; }

template <
    class T,
    class Container
> bool operator>(const queue<T, Container> & x, const queue<T, Container> & y)
{ return y < x; }

template <
    class T,
    class Container
> bool operator<=(const queue<T, Container> & x, const queue<T, Container> & y)
{ return !(y < x); }

template <
    class T,
    class Container
> bool operator>=(const queue<T, Container> & x, const queue<T, Container> & y)
{ return !(x < y); }

template <
    class T,
    class Container
> void swap(queue<T, Container> & x, queue<T, Container> & y)
{ x.swap(y); }

/**
 * @brief 优先队列
 * @link https://zh.cppreference.com/w/cpp/container/priority_queue
 * @details 优先队列默认以vector作为底层容器
 */
template <
    class T,
    class Container = wwstl::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue
{
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;   // 基础容器
    Compare comp;       // 比较器

public:
    // 构造函数

    priority_queue()
        : priority_queue(Compare(), Container())
    {
    }

    explicit priority_queue(const Compare & compare)
        : priority_queue(compare, Container())
    {
    }

    priority_queue(const Compare & compare, const Container & cont)
        : c(cont), comp(compare)
    {
    }

    priority_queue(const Compare & compare, Container && cont)
        : c(std::move(cont)), comp(compare)
    {
    }

    priority_queue(const priority_queue & other)
        : c(other.c), comp(other.comp)
    {
    }

    priority_queue(priority_queue && other)
        : c(std::move(other.c)), comp(std::move(other.comp))
    {
    }

    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare & compare = Compare())
        : c(first, last), comp(compare)
    { wwstl::make_heap(c.begin(), c.end(), comp); }

    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare & compare, const Container & cont)
        : c(cont), comp(compare)
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare & compare, Container && cont)
        : c(std::move(cont)), comp(compare)
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > explicit priority_queue(const Alloc & alloc)
        : c(alloc), comp(Compare())
    {
    }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > explicit priority_queue(const Compare & compare, const Alloc & alloc)
        : c(alloc), comp(compare)
    {
    }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(const Compare & compare, const Container & cont, const Alloc & alloc)
        : c(cont, alloc), comp(compare)
    { wwstl::make_heap(c.begin(), c.end(), comp); }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(const Compare & compare, Container && cont, const Alloc & alloc)
        : c(std::move(cont), alloc), comp(compare)
    { wwstl::make_heap(c.begin(), c.end(), comp); }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(const priority_queue & other, const Alloc & alloc)
        : c(other.c, alloc), comp(other.comp)
    {
    }

    template <
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(priority_queue && other, const Alloc & alloc)
        : c(std::move(other.c), alloc), comp(other.comp)
    {
    }

    template <
        class InputIt,
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(InputIt first, InputIt last, const Alloc & alloc)
        : c(alloc), comp(Compare())
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    template <
        class InputIt,
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(InputIt first, InputIt last, const Compare & compare, const Alloc & alloc)
        : c(alloc), comp(compare)
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    template <
        class InputIt,
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(InputIt first, InputIt last, const Compare & compare, const Container & cont, const Alloc & alloc)
        : c(cont, alloc), comp(compare)
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    template <
        class InputIt,
        class Alloc,
        typename = std::enable_if_t<std::uses_allocator<container_type, Alloc>::value>
    > priority_queue(InputIt first, InputIt last, const Compare & compare, Container && cont, const Alloc & alloc)
        : c(std::move(cont), alloc), comp(compare)
    {
        c.insert(c.end(), first, last);
        wwstl::make_heap(c.begin(), c.end(), comp);
    }

    ~priority_queue() = default;

public:
    // 成员函数

    /**
     * @brief 将值赋给容器适配器
     */
    priority_queue & operator=(const priority_queue & other)
    {
        c = other.c;
        comp = other.comp;
        return *this;
    }

    /**
     * @brief 将值赋给容器适配器
     */
    priority_queue & operator=(priority_queue && other)
    {
        c = std::move(other.c);
        comp = std::move(other.comp);
        return *this;
    }

    // 元素访问

    /**
     * @brief 访问队首元素
     */
    const_reference top() const
    { return c.front(); }

    // 容量

    /**
     * @brief 检查容器适配器是否为空
     */
    bool empty() const
    { return c.empty(); }

    /**
     * @brief 返回元素数
     */
    size_type size() const
    { return c.size(); }

    // 修改器

    /**
     * @brief 插入元素，并对底层容器排序
     */
    void push(const value_type & x)
    {
        c.push_back(x);
        wwstl::push_heap(c.begin(), c.end(), comp);
    }

    /**
     * @brief 插入元素，并对底层容器排序
     */
    void push(value_type && x)
    {
        c.push_back(std::move(x));
        wwstl::push_heap(c.begin(), c.end(), comp);
    }

    /**
     * @brief 原位构造元素并排序底层容器
     */
    template <class... Args>
    void emplace(Args&&... args)
    {
        c.emplace_back(std::forward<Args>(args)...);
        wwstl::push_heap(c.begin(), c.end(), comp);
    }

    /**
     * @brief 移除队首元素
     */
    void pop()
    {
        wwstl::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

    /**
     * @brief 交换内容
     */
    void swap(priority_queue & other) noexcept
    {
        std::swap(c, other.c);
        std::swap(comp, other.comp);
    }
};

template <
    class T,
    class Container,
    class Compare
> void swap(priority_queue<T, Container, Compare> & lhs, priority_queue<T, Container, Compare> & rhs)
{ lhs.swap(rhs); }

} // namespace wwstl

#endif // __WW_QUEUE_H__