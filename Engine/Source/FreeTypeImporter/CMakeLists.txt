cmake_minimum_required(VERSION 3.9)
project(BerserkFreeTypeImporter)

# Berserk FreeType Importer files

set(
        SOURCES

        Public/FreeTypeImporter.cpp
        Public/FreeTypeImporter.h
)

# Create static FreeType Importer library

add_library(BerserkFreeTypeImporter STATIC ${SOURCES})