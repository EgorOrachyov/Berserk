add_library(TinyOBJ STATIC "tiny_obj_loader.h" "tiny_obj_loader.cpp")
target_include_directories(TinyOBJ PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

if (UNIX)
    # Force -fPIC on linux/mac, since we link static library with dynamic
    # to avoid `relocation R_X86_64_PC32 against symbol...` errors
    target_compile_options(TinyOBJ PRIVATE "-fPIC")
endif()