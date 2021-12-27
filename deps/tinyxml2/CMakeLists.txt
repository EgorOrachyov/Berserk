add_library(TinyXML2 STATIC tinyxml2.hpp tinyxml2.cpp)
target_include_directories(TinyXML2 PUBLIC "${CMAKE_CURRENT_LIST_DIR}")

if (UNIX)
    # Force -fPIC on linux/mac, since we link static library with dynamic
    # to avoid `relocation R_X86_64_PC32 against symbol...` errors
    target_compile_options(TinyXML2 PRIVATE "-fPIC")
endif()