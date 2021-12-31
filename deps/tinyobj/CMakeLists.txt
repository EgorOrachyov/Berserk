add_library(TinyOBJ STATIC "tiny_obj_loader.h" "tiny_obj_loader.cpp")
target_include_directories(TinyOBJ PUBLIC "${CMAKE_CURRENT_LIST_DIR}")