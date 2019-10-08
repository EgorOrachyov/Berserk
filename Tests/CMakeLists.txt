cmake_minimum_required(VERSION 3.1)
project(BerserkTests)

set(
        SOURCES

        main.cpp
)

set(
        LIBRARIES

        BerserkCoreSystem
)

add_executable(BerserkTests ${SOURCES})
target_link_libraries(BerserkTests ${LIBRARIES})