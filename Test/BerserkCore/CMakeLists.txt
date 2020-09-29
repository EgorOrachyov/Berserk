add_executable(Minimals Minimals.cpp)
target_link_libraries(Minimals PUBLIC BerserkCore)
target_link_libraries(Minimals PUBLIC BerserkPlatform)
target_link_libraries(Minimals PUBLIC gtest)