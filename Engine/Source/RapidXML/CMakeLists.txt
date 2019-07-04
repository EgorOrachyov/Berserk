cmake_minimum_required(VERSION 3.9)
project(BerserkXMLParser)

# XML Parser files

include(CMakeSources.cmake)

# Create static XML Parser Subsystem library

add_library(BerserkXMLParser STATIC ${SOURCE_LIB})