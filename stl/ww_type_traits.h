#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include <type_traits>
#include <iterator>

namespace wwstl
{

template <class... Ts>
using void_t = void;

template <
    class T,
    class = void
> class is_iterator
    : std::false_type
{
};

// 如果存在 iterator_traits<T>::iterator_category，则是迭代器
template <class T>
class is_iterator<T, void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type
{
};

template <class T>
constexpr bool is_iterator_v = is_iterator<T>::value;


} // namespace wwstl

#endif