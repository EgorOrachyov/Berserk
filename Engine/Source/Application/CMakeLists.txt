cmake_minimum_required(VERSION 3.9)
project(BerserkApplication)

# Berserk Application module files

set(SOURCE_LIB

        Private/ApplicationBase.cpp
        Private/ApplicationCore.cpp
        Public/ApplicationBase.h
        Public/ApplicationCore.h

        )

# Create static Application module library

add_library(BerserkApplication STATIC ${SOURCE_LIB})