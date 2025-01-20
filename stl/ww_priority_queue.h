#ifndef __WW_PRIORITY_QUEUE_H__
#define __WW_PRIORITY_QUEUE_H__

#include "ww_vector.h"
#include "ww_algorithm.h"

namespace wwstl
{

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

#endif // __WW_PRIORITY_QUEUE_H__