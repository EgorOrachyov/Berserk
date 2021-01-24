add_library(TinyFileDialogs STATIC tfd/tinyfiledialogs.h tfd/tinyfiledialogs.c)
target_include_directories(TinyFileDialogs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
set_target_properties(TinyFileDialogs PROPERTIES POSITION_INDEPENDENT_CODE YES)
