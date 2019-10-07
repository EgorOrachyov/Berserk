cmake_minimum_required (VERSION 3.9)

list         (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Engine/Source/FreeImageImporter")
find_package (FreeImage REQUIRED)
set          (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lFREEIMAGE")
set          (BERSERK_LINK_THIRD_PARTY ${BERSERK_LINK_THIRD_PARTY} ${FREEIMAGE_LIBRARIES})