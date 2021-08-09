set(BERSERK_MODULES_HEADER)
set(BERSERK_MODULES_SRC)
set(BERSERK_RUNTIME_DEFINES)

include(BerserkCore/CMakeLists.txt)
include(BerserkRHI/CMakeLists.txt)
include(BerserkPlatform/CMakeLists.txt)

if (BERSERK_WITH_OPENGL)
    message(STATUS "Add Berserk OpenGL RHI backend")
    include(BerserkOpenGL/CMakeLists.txt)
    list(APPEND BERSERK_MODULES_HEADER ${BERSERK_OPENGL_HEADER})
    list(APPEND BERSERK_MODULES_SRC ${BERSERK_OPENGL_SRC})
    list(APPEND BERSERK_RUNTIME_DEFINES BERSERK_WITH_OPENGL)
endif()

if (BERSERK_WITH_VULKAN)
    message(STATUS "Add Berserk Vulkan RHI backend")
    include(BerserkVulkan/CMakeLists.txt)
    list(APPEND BERSERK_MODULES_HEADER ${BERSERK_VULKAN_HEADER})
    list(APPEND BERSERK_MODULES_SRC ${BERSERK_VULKAN_SRC})
    list(APPEND BERSERK_RUNTIME_DEFINES BERSERK_WITH_VULKAN)
endif()

set(BERSERK_RUNTIME_HEADER
        ${BERSERK_CORE_HEADER}
        ${BERSERK_PLATFORM_HEADER}
        ${BERSERK_RHI_HEADER}
        ${BERSERK_MODULES_HEADER})

set(BERSERK_RUNTIME_SRC
        ${BERSERK_CORE_SRC}
        ${BERSERK_RHI_SRC}
        ${BERSERK_PLATFORM_SRC}
        ${BERSERK_MODULES_SRC})

add_library(BerserkRuntime STATIC
        ${BERSERK_RUNTIME_HEADER}
        ${BERSERK_RUNTIME_SRC})

set_target_properties(BerserkRuntime PROPERTIES OUTPUT_NAME "BerserkRuntime_${BERSERK_ARCH}")
target_include_directories(BerserkRuntime PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkRuntime PUBLIC Yamc)
target_link_libraries(BerserkRuntime PRIVATE StbImage)
target_link_libraries(BerserkRuntime PRIVATE portable_file_dialogs)
target_link_libraries(BerserkRuntime PRIVATE WhereAmI)

if (BERSERK_WITH_GLFW)
    target_link_libraries(BerserkRuntime PRIVATE glfw)
endif()

if (BERSERK_WITH_OPENGL)
    target_link_libraries(BerserkRuntime PRIVATE libglew_static)
endif()

if (BERSERK_WITH_VULKAN)
    target_link_libraries(BerserkRuntime PRIVATE Vulkan)
    target_link_libraries(BerserkRuntime PRIVATE Vma)
    target_link_libraries(BerserkRuntime PRIVATE glslang)
    target_link_libraries(BerserkRuntime PRIVATE SPIRV)
endif()

foreach(DEFINITION ${BERSERK_RUNTIME_DEFINES})
    target_compile_definitions(BerserkRuntime PUBLIC ${DEFINITION})
endforeach()

berserk_target_compile_definitions(BerserkRuntime)
berserk_target_compile_options(BerserkRuntime)
berserk_target_link_options(BerserkRuntime)