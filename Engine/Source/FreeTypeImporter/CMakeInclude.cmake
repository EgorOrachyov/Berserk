cmake_minimum_required (VERSION 3.9)

find_package (FreeType REQUIRED)
message      (STATUS "Found FreeType: headers at ${FREETYPE_INCLUDE_DIRS}, libraries at ${FREETYPE_LIBRARIES}")
set          (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lFREETYPE")
set          (BERSERK_LINK_THIRD_PARTY ${BERSERK_LINK_THIRD_PARTY} ${FREETYPE_LIBRARIES})
