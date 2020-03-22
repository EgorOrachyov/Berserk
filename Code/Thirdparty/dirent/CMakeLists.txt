cmake_minimum_required(VERSION 3.11)
project(WindowsDirent)

add_library(WindowsDirent INTERFACE)
target_include_directories(WindowsDirent INTERFACE "windows_dirent.h")