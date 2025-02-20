#ifndef __WW_FUNCTIONAL_H__
#define __WW_FUNCTIONAL_H__

#include <utility>
#include <functional>

namespace wwstl
{

/**
 * @brief set容器默认keyExtractor
 */
template <class T>
class keyExtractor
{
public:
    const T & operator()(const T & key) const
    {
        return key;
    }
};

/**
 * @brief map容器默认keyExtractor
 */
template <typename T, typename U>
class keyExtractor<std::pair<const T, U>>
{
public:
    const T & operator()(const std::pair<const T, U> & p) const
    {
        return p.first;
    }
};

} // namespace wwstl

#endif // __WW_FUNCTIONAL_H__