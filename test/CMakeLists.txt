file(GLOB TEST_FILES "*.cpp")

add_executable(test_all ${TEST_FILES})

target_link_libraries(test_all PRIVATE
    stl
    GTest::gtest
    GTest::gtest_main
)