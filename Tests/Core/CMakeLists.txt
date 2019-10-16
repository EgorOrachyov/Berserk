cmake_minimum_required(VERSION 3.1)
project(BerserkTestsCore)

set(
        SOURCES

        main.cpp
        Containers.h
)

variable_watch(ENGINE_LIBS)
add_executable(BerserkTestsCore ${SOURCES})
target_link_libraries(BerserkTestsCore ${ENGINE_LIBS})