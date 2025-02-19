#ifndef __WW_ALGORITHM_H__
#define __WW_ALGORITHM_H__

namespace wwstl
{

/**
 * @brief 调整一个堆为正确的堆
 * @details 当某个堆可能不正确时使用
 */
template <
    class RandomIt,
    class Compare
> void _heap_heapify(RandomIt first, RandomIt last, Compare comp, int rootIndex = 0)
{
    // 交换父节点和两个子节点中的最大值
    auto length = last - first;
    auto leftIndex = 2 * rootIndex + 1;
    auto rightIndex = 2 * rootIndex + 2;
    auto largestIndex = rootIndex;

    if (leftIndex < length && comp(*(first + largestIndex), *(first + leftIndex))) {
        largestIndex = leftIndex;
    }
    if (rightIndex < length && comp(*(first + largestIndex), *(first + rightIndex))) {
        largestIndex = rightIndex;
    }

    if (largestIndex != rootIndex) {
        std::swap(*(first + largestIndex), *(first + rootIndex));
        // 通过递归的方式继续调整
        _heap_heapify(first, last, comp, largestIndex);
    }
}

/**
 * @brief 建立堆
 * @link https://zh.cppreference.com/w/cpp/algorithm/make_heap
 */
template <
    class RandomIt,
    class Compare
> void make_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
        return;

    auto length = last - first;
    for (int i = length / 2 - 1; i >= 0; --i) {
        _heap_heapify(first, last, comp, i);
    }
}

/**
 * @brief 建立堆
 */
template <class RandomIt>
void make_heap(RandomIt first, RandomIt last)
{
    wwstl::make_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

/**
 * @brief 新元素加入堆后调整
 * @link https://zh.cppreference.com/w/cpp/algorithm/push_heap
 * @details RandomIt必须是一个随机访问迭代器，Compare必须满足比较
 */
template <
    class RandomIt,
    class Compare
> void push_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
        return;

    // 新值从最后开始，向上调整
    auto targetIndex = last - first - 1;
    auto parentIndex = (targetIndex - 1) / 2;
    while (targetIndex > 0 && comp(*(first + parentIndex), *(first + targetIndex))) {
        // 交换
        std::swap(*(first + targetIndex), *(first + parentIndex));
        targetIndex = parentIndex;
        parentIndex = (targetIndex - 1) / 2;
    }
}

/**
 * @brief 新元素加入堆后调整
 * @details RandomIt必须是一个随机访问迭代器
 */
template <class RandomIt>
void push_heap(RandomIt first, RandomIt last)
{
    wwstl::push_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

/**
 * @brief 删除堆顶元素后调整
 * @link https://zh.cppreference.com/w/cpp/algorithm/pop_heap
 */
template <
    class RandomIt,
    class Compare
> void pop_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
        return;

    // 先把堆顶元素和最后一个元素交换
    std::swap(*first, *(last - 1));
    // 此时[first, last - -1)是一个不一定满足堆性质的序列，需要调整
    _heap_heapify(first, last - 1, comp);
}

/**
 * @brief 删除堆顶元素后调整
 */
template <class RandomIt>
void pop_heap(RandomIt first, RandomIt last)
{
    wwstl::pop_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

template <
    class RandomIt,
    class Compare
> void sort_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
        return;

    while (last - first > 1) {
        wwstl::pop_heap(first, last, comp);
        --last;
    }
}

template <class RandomIt>
void sort_heap(RandomIt first, RandomIt last)
{
    wwstl::sort_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

} // namespace wwstl

#endif // __WW_ALGORITHM_H__