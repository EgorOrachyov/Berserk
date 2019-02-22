cmake_minimum_required(VERSION 3.9)
project(BerserkFreeImageImporter)

# Berserk FreeImage Importer files

set(SOURCE_LIB

        Private/FreeImageImporter.cpp
        Public/FreeImageImporter.h

        )

# Create static FreeImage Importer library

add_library(BerserkFreeImageImporter STATIC ${SOURCE_LIB})