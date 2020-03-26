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
    TPtrShared.h
    TIterable.h
    TArray.h
    TPair.h
    TMap.h
    TSet.h
    TPredicates.h
    TAlgo.h
    TimeValue.h
    UUID.h
    Alloc.h
    AllocPool.h
    Crc32.h
    Resource.h
    Platform/Memory.h
    Platform/IMutex.h
    Platform/ISystem.h
    Platform/IFile.h
    Platform/IDirectory.h
    Platform/IAtomic.h
    Platform/IInput.h
    String/CString.h
    String/WString.h
    String/CStringStatic.h
    String/TString.h
    String/TStringUtility.h
    String/TStringStaticN.h
    String/CStringBuilder.h
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
    Math/TRange.h
    IO/ILog.h
    IO/Logs.h
    IO/IOutputDevice.h
    IO/Json.h
    IO/Archive.h
    IO/ArchiveFile.h
    IO/IResourceImporter.h
        )


set(Target_SOURCES
    Typedefs.cpp
    ErrorMacro.cpp
    Platform/IDirectory.cpp
    Platform/Memory.cpp
    Platform/IInput.cpp
    Platform/ISystem.cpp
    String/TString.cpp
    Math/Math.cpp
    Math/Color4f.cpp
    TPtrShared.cpp
    UUID.cpp
    Alloc.cpp
    AllocPool.cpp
    Crc32.cpp
    Resource.cpp
    IO/ILog.cpp
    IO/Logs.cpp
    IO/IOutputDevice.cpp
    IO/Json.cpp
    IO/Archive.cpp
    IO/ArchiveFile.cpp
        )

add_library(BerserkCore SHARED ${Target_HEADERS} ${Target_SOURCES})

target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(BerserkCore PUBLIC cxx_std_11)

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)