cmake_minimum_required(VERSION 3.9)
project(BerserkFreeImageImporter)

# FreeImage Importer System files

set(
        SOURCES

        Private/FreeImageImporter.cpp
        Public/FreeImageImporter.h
)

# Create static FreeImage Importer Subsystem library

add_library(BerserkFreeImageImporter STATIC ${SOURCES})