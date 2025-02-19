#ifndef __WW_STACK_H__
#define __WW_STACK_H__

#include "ww_deque.h"

namespace wwstl
{

/**
 * @brief stack
 * @link https://zh.cppreference.com/w/cpp/container/stack
 * @details 栈默认以deque作为底层容器
 */
template <
    class T,
    class Container = wwstl::deque<T>
> class stack
{
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    container_type c;            // 基础容器

public:
    stack()
        : stack(Container())
    {
    }

    explicit stack(const container_type & cont)
        : c(cont)
    {
    }

    explicit stack(container_type && cont)
        : c(std::move(cont))
    {
    }

    template <class Alloc>
    explicit stack(const Alloc & alloc)
        : c(alloc)
    {
    }

    template <class Alloc>
    stack(const container_type & cont, const Alloc & alloc)
        : c(cont, alloc)
    {
    }

    template <class Alloc>
    stack(container_type && cont, const Alloc & alloc)
        : c(std::move(cont), alloc)
    {
    }

    template <class Alloc>
    stack(const stack & other, const Alloc & alloc)
        : c(other.c, alloc)
    {
    }

    template <class Alloc>
    stack(stack && other, const Alloc & alloc)
        : c(std::move(other.c), alloc)
    {
    }

    ~stack() = default;

public:
    // 元素访问

    /**
     * @brief 访问栈顶元素
     */
    reference top()
    {
        return c.back();
    }

    /**
     * @brief 访问栈顶元素
     */
    const_reference top() const
    {
        return c.back();
    }

    // 容量

    /**
     * @brief 检查容器适配器是否为空
     */
    bool empty() const
    {
        return c.empty();
    }

    /**
     * @brief 返回元素数
     */
    size_type size() const
    {
        return c.size();
    }

    // 修改器

    /**
     * @brief 向栈顶插入元素
     */
    void push(const value_type& x)
    {
        c.push_back(x);
    }

    /**
     * @brief 向栈顶插入元素
     */
    void push(value_type&& x)
    {
        c.push_back(std::move(x));
    }

    /**
     * @brief 在顶部原位构造元素
     */
    template <class... Args>
    void emplace(Args&&... args)
    {
        c.emplace_back(std::forward<Args>(args)...);
    }

    /**
     * @brief 移除栈顶元素
     */
    void pop()
    {
        c.pop_back();
    }

    /**
     * @brief 交换内容
     */
    void swap(stack& other)
    {
        c.swap(other.c);
    }
};

// 非成员函数

template <
    class T,
    class Container
> bool operator==(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return x.c == y.c;
}

template <
    class T,
    class Container
> bool operator!=(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return !(x == y);
}

template <
    class T,
    class Container
> bool operator<(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return x.c < y.c;
}

template <
    class T,
    class Container
> bool operator>(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return y < x;
}

template <
    class T,
    class Container
> bool operator<=(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return !(y < x);
}

template <
    class T,
    class Container
> bool operator>=(const stack<T, Container> & x, const stack<T, Container> & y)
{
    return !(x < y);
}

template <
    class T,
    class Container
> void swap(stack<T, Container> & x, stack<T, Container> & y)
{
    x.swap(y);
}

} // namespace wwstl

#endif // __WW_STACK_H__