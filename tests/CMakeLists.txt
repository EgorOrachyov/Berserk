function(berserk_test_target target)
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PRIVATE berserk_runtime_dynamic)
    target_link_libraries(${target} PRIVATE gtest)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
endfunction()

berserk_test_target(TestIO)
berserk_test_target(TestFileSystem)