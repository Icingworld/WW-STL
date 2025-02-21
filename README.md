# WW-STL

![C++](https://img.shields.io/badge/language-C++-blue)
![Standard](https://img.shields.io/badge/standard-C++11-green)
![MIT License](https://img.shields.io/github/license/Icingworld/WW-STL?color=red)

![Last commit](https://img.shields.io/github/last-commit/Icingworld/WW-STL?color=yellow)
![Release](https://img.shields.io/github/v/release/Icingworld/WW-STL?label=release)
![Test](https://img.shields.io/github/actions/workflow/status/Icingworld/WW-STL/stl_test.yml)

## 一、简介

本仓库为作者在学习 STL 的过程中，对 C++11 标准下 STL 容器的一种实现。欢迎学习交流！

根据 C++11 标准（详情见参考文献 [2]），本仓库包含了以下内容的实现：

+ 底层功能：

| 标准名 | 本仓库名 | 说明 | 完成情况 |
| :---: | :---: | :---: | :---: |
| memory | ww_memory.h | 分配器部分 | 完成 |
| iterator | ww_iterator.h | 迭代器 | 完成 |
| algorithm | ww_algorithm.h | 算法heap部分 | 完成 |
| - | ww_hash.h | 哈希 | 完成 |
| - | ww_hashtable.h | 哈希表 | 完成 |
| - | ww_rbtree.h | 红黑树 | 完成 |

+ 序列型容器：

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| array | ww_array.h | 完成 |
| vector | ww_vector.h | 完成 |
| deque | ww_deque.h | 完成 |
| forward_list | ww_forward_list.h | 完成 |
| list | ww_list.h | 完成 |

+ 关联型容器:

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| set | ww_set.h | 完成 |
| multiset | ww_set.h | 完成 |
| map | ww_map.h | 完成 |
| multimap | ww_map.h | 完成 |

+ 无序关联型容器：

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| unordered_set | ww_unordered_set.h | 完成 |
| unordered_multiset | ww_unordered_set.h | 完成 |
| unordered_map | ww_unordered_map.h | 完成 |
| unordered_multimap | ww_unordered_map.h | 完成 |

+ 容器适配器

| 标准名 | 本仓库名 | 完成情况 |
| :---: | :---: | :---: |
| stack | ww_stack.h | 完成 |
| queue | ww_queue.h | 完成 |
| priority_queue | ww_queue.h | 完成 |

## 二、设计理念

1. 本实现基于 c++11 标准，在不使用更高版本 C++ 特性的情况下，遵循 STL 设计理念，实现 STL 容器相关接口，使得本实现适配 C++ 标准库

2. 本实现采用单头文件形式，编译时不需要包含源码文件

3. 本实现不做过多的抽象封装，在实现 STL 功能的情况下尽可能简洁明了

4. 本实现从《STL源码剖析》（见参考文献 [1]）开始，部分参考 MSVC（见参考文献 [3]） 的 STL 算法设计，将《STL源码剖析》中未完成的接口实现

5. 本实现仅关注容器实现部分，算法部分使用标准库算法

## 三、项目结构

| 目录 | 说明 |
| :---: | :---: |
| stl/ | WW-STL实现 |
| test/ | 测试用例 |
| benchmark/ | 性能测试用例 |
| third-party/ | 第三方库 |

## 四、使用方法

### 1. 使用WW-STL

本实现全部位于`wwstl`命名空间下

1. 包含头文件

    ```c++
    #include "ww_vector.h"
    ```

2. 使用`wwstl`命名空间

    ```c++
    using namespace wwstl;
    ```

3. 像使用标准库一样使用它

### 2. 运行仓库测试用例

本仓库使用 Google Test 和 Google Benchmark 进行测试，glone 仓库后，安装子模块来进行测试：

```bash
git submodule update --init --recursive
```

等待子模块下载完成，使用 cmake 编译测试用例：

#### 2.1 Google Test

```bash
mkdir build && cd build
cmake .. -DENABLE_TEST=ON
make -j4
```

运行测试用例：

```bash
./test/test_all
```

#### 2.2 Google Benchmark

```bash
mkdir build && cd build
cmake .. -DENABLE_BENCHMARK=ON
make -j4
```

运行性能测试用例：

```bash
./benchmark/benchmark_all
```

## 参考文献

[1] 侯捷. STL源码剖析[M]. 北京: 机械工业出版社, 2002.

[2] cppreference. cppreference.com[EB/OL]. [2025-01-16]. <https://zh.cppreference.com>, 2025-01-16.

[3] Microsoft. STL[EB/OL]. [2025-01-16]. <https://github.com/microsoft/STL>, 2025-01-16.
