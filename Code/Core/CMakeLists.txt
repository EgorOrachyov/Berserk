cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/BuildOptions.h
    Typedefs.h
    TestUtility.h
    TestMacro.h
    ErrorMacro.h
    Error.h
    Format.h
    TPtr.h
    TPtrUnique.h
    TIterable.h
    TArray.h
    TMap.h
    TPair.h
    THashMap.h
    TPredicates.h
    TAlgo.h
    TimeValue.h
    UUID.h
    Alloc.h
    AllocPool.h
    Crc32.h
    Platform/Memory.h
    Platform/IMutex.h
    Platform/ISystem.h
    Platform/IFile.h
    Platform/IAtomic.h
    Platform/IInput.h
    String/CString.h
    String/CStringStatic.h
    String/TString.h
    String/TStringUtility.h
    String/TStringStaticN.h
    Math/Math.h
    Math/TVecN.h
    Math/Vec2f.h
    Math/Vec3f.h
    Math/Vec4f.h
    Math/Point2i.h
    Math/Random.h
    Math/TMatMxN.h
    Math/Mat4x4f.h
    Math/Color4f.h
    IO/ILog.h
    IO/Logs.h
    IO/IOutputDevice.h
    IO/Json.h
        )


set(Target_SOURCES
    Typedefs.cpp
    ErrorMacro.cpp
    Platform/Memory.cpp
    Platform/IInput.cpp
    Platform/ISystem.cpp
    String/TString.cpp
    Math/Math.cpp
    Math/Color4f.cpp
    UUID.cpp
    Alloc.cpp
    AllocPool.cpp
    Crc32.cpp
    IO/ILog.cpp
    IO/Logs.cpp
    IO/IOutputDevice.cpp
    IO/Json.cpp
        )

add_library(BerserkCore SHARED ${Target_HEADERS} ${Target_SOURCES})

target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(BerserkCore PUBLIC cxx_std_11)

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)