add_library(TinyXML2 STATIC tinyxml2.hpp tinyxml2.cpp)
target_include_directories(TinyXML2 PUBLIC "${CMAKE_CURRENT_LIST_DIR}")