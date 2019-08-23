cmake_minimum_required(VERSION 3.9)
project(BerserkCoreSystem)

# Core Subsystem files

include(CMakeSources.cmake)

# Create static Core Subsystem library

add_library(BerserkCoreSystem STATIC ${SOURCE_LIB} Public/Containers/TRemoveIterator.h Public/Math/Rect.h Public/Math/Size.h)