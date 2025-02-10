#ifndef __WW_QUEUE_H__
#define __WW_QUEUE_H__

#include "ww_deque.h"

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

} // namespace wwstl

#endif // __WW_QUEUE_H__