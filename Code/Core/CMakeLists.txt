cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/BuildOptions.h
    Typedefs.h
    TestUtility.h
    TestMacro.h
    Errors.h
    ErrorList.h
    Format.h
    Platform/Memory.h
    Platform/Alloc.h
    Platform/Mutex.h
    Platform/OS.h
    Platform/File.h
    Platform/Atomic.h
    Platform/Input.h
    String/CString.h
    String/StringUtility.h
    String/StringStatic.h
    Math/Math.h
    Math/TVectorN.h
    Math/Vec3f.h
    Math/Vec4f.h
    Math/Point2i.h
    Platform/InputEnums.h)

set(Target_SOURCES
    Typedefs.cpp
    Errors.cpp
    Platform/Memory.cpp
    Platform/Alloc.cpp
    String/CString.cpp
    Platform/Input.cpp)

add_library(BerserkCore SHARED ${Target_HEADERS} ${Target_SOURCES})

target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(BerserkCore PUBLIC cxx_std_11)

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)
