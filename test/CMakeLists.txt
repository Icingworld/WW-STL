set(TEST_FILES
    ww_memory_test.cpp

    # ww_array_test.cpp
    # ww_vector_test.cpp
    # ww_deque_test.cpp
    # ww_forward_list_test.cpp
    # ww_list_test.cpp

    # ww_set_test.cpp
    # ww_multiset_test.cpp
    # ww_map_test.cpp
    # ww_multimap_test.cpp

    # ww_unordered_set_test.cpp
    # ww_unordered_multiset_test.cpp
    # ww_unordered_map_test.cpp
    # ww_unordered_multimap_test.cpp
    
    # ww_stack_test.cpp
    # ww_queue_test.cpp
    # ww_priority_queue_test.cpp
)

add_executable(test_all ${TEST_FILES})

target_link_libraries(test_all PRIVATE
    stl
    GTest::gtest
    GTest::gtest_main
)