# WW-STL

## 一、简介

本仓库为作者在学习 STL 的过程中，对 C++11 标准下 STL 容器的一种实现。

根据 C++11 标准（详情见参考文献 [2]），本仓库包含了以下内容的实现：

+ 底层功能：

| 标准名 | 本仓库名 | 说明 | 完成情况 |
| :---: | :---: | :---: | :---: |
| memory | ww_memory.h | 分配器部分 | 完成 |
| iterator | ww_iterator.h | 迭代器 | 完成 |
| algorithm | ww_algorithm.h | 算法heap部分 | 完成 |
| - | ww_hash.h | 哈希 | 完成 |
| - | ww_hashtable.h | 哈希表 | 待验证 |
| - | ww_rbtree.h | 红黑树 | 待验证 |

+ 序列型容器：

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| array | ww_array.h | 完成 |
| vector | ww_vector.h | 完成 |
| deque | ww_deque.h | 完成 |
| forward_list | ww_forward_list.h | 待验证 |
| list | ww_list.h | 待验证 |

+ 关联型容器:

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| set | ww_set.h | 待验证 |
| multiset | ww_multiset.h | 待验证 |
| map | ww_map.h | 待验证 |
| multimap | ww_multimap.h | 待验证 |

+ 无序关联型容器：

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| unordered_set | ww_unordered_set.h | 待验证 |
| unordered_multiset | ww_unordered_multiset.h | 待验证 |
| unordered_map | ww_unordered_map.h | 待验证 |
| unordered_multimap | ww_unordered_multimap.h | 待验证 |

+ 容器适配器

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| stack | ww_stack.h | 完成 |
| queue | ww_queue.h | 完成 |
| priority_queue | ww_priority_queue.h | 完成 |

## 二、设计理念

1. 本实现基于 c++11 标准，在不使用更高版本 C++ 特性的情况下，遵循 STL 设计理念，实现 STL 容器相关接口，使得本实现适配 C++ 标准库

2. 本实现采用单头文件形式，编译时不需要包含源码文件

3. 本实现不做过多的抽象封装，在实现 STL 功能的情况下尽可能简洁明了

4. 本实现从《STL源码剖析》（见参考文献 [1]）开始，部分参考 MSVC（见参考文献 [3]） 的 STL 算法设计，将《STL源码剖析》中未完成的接口实现

5. 本实现仅关注容器实现部分，算法方面使用标准库算法

## 三、项目结构

| 目录 | 说明 |
| :---: | :---: |
| doc/ | 文档 |
| stl/ | WW-STL实现 |

## 四、项目文档

本仓库提供一系列文档，整理、记录作者在学习、实现 STL 容器的过程中所学到的知识，目录如下：

+ [一、初识 STL](./doc/一、初识%20STL.md)
+ [二、分配器](./doc/二、分配器.md)
+ [三、迭代器](./doc/三、迭代器.md)
+ [四、序列容器之 array](./doc/四、序列容器之%20array.md)
+ [五、序列容器之 vector](./doc/五、序列容器之%20vector.md)
+ [六、序列容器之 deque](./doc/六、序列容器之%20deque.md)

## 五、使用方法

本实现全部位于`wwstl`命名空间下

1. 包含头文件

    ```c++
    #include "ww_vector.h"
    ```

2. 使用`wwstl`命名空间

    ```c++
    using namespace wwstl;
    ```

## 六、更多工作

| 目标 | 完成情况 |
| :---: | :---: |
| 使用 Google Test 对容器进行功能测试 | 未完成 |
| 使用 Google Benchmark 对容器进行性能测试 | 未完成 |

## 参考文献

[1] 侯捷. STL源码剖析[M]. 北京: 机械工业出版社, 2002.

[2] cppreference. cppreference.com[EB/OL]. [2025-01-16]. <https://zh.cppreference.com>, 2025-01-16.

[3] Microsoft. STL[EB/OL]. [2025-01-16]. <https://github.com/microsoft/STL>, 2025-01-16.
