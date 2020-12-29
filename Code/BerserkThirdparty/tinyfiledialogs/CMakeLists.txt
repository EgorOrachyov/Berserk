add_library(TinyFileDialogs SHARED tfd/tinyfiledialogs.h tfd/tinyfiledialogs.c)
target_include_directories(TinyFileDialogs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)