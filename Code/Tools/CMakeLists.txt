cmake_minimum_required(VERSION 3.11)
project(BerserkTools VERSION 1.0 LANGUAGES CXX)

add_executable(BerserkUuid BerserkUuid.cpp)
target_link_libraries(BerserkUuid PUBLIC BerserkCore)
target_link_libraries(BerserkUuid PUBLIC BerserkPlatform)
target_compile_features(BerserkUuid PUBLIC cxx_std_11)
set_target_properties(BerserkUuid PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkUuid PROPERTIES CXX_STANDARD_REQUIRED YES)