set(BERSERK_RUNTIME_MODULES_HEADER)
set(BERSERK_RUNTIME_MODULES_SRC)
set(BERSERK_RUNTIME_DEFINES_PUBLIC)
set(BERSERK_RUNTIME_DEFINES_PRIVATE)

function(berserk_configure_target target)
    berserk_target_arch(${target})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    target_link_libraries(${target} PUBLIC Yamc)
    target_link_libraries(${target} PRIVATE StbImage)
    target_link_libraries(${target} PRIVATE portable_file_dialogs)
    target_link_libraries(${target} PRIVATE WhereAmI)
    target_link_libraries(${target} PRIVATE TinyXML2)

    if (BERSERK_TARGET_WINDOWS)
        target_link_libraries(${target} PRIVATE Shlwapi.lib)
    endif ()

    if (BERSERK_WITH_GLFW)
        target_link_libraries(${target} PRIVATE glfw)
    endif ()

    if (BERSERK_WITH_GLEW)
        target_link_libraries(${target} PRIVATE libglew_static)
    endif ()

    if (BERSERK_WITH_VULKAN)
        target_link_libraries(${target} PRIVATE Vulkan)
        target_link_libraries(${target} PRIVATE Vma)
        target_link_libraries(${target} PRIVATE glslang)
        target_link_libraries(${target} PRIVATE SPIRV)
    endif ()

    if (BERSERK_WITH_GLEW)
        target_link_libraries(${target} PRIVATE StbImage)
    endif ()

    foreach (DEFINITION ${BERSERK_RUNTIME_DEFINES_PUBLIC})
        target_compile_definitions(${target} PUBLIC ${DEFINITION})
    endforeach ()

    foreach (DEFINITION ${BERSERK_RUNTIME_DEFINES_PRIVATE})
        target_compile_definitions(${target} PRIVATE ${DEFINITION})
    endforeach ()

    berserk_target_compile_definitions(${target})
    berserk_target_compile_options(${target})
    berserk_target_link_options(${target})
endfunction()

include(core/CMakeLists.txt)
include(platform/CMakeLists.txt)
include(rhi/CMakeLists.txt)
include(render/CMakeLists.txt)

set(BERSERK_RUNTIME_HEADER
        Berserk.hpp
        ${BERSERK_CORE_HEADER}
        ${BERSERK_PLATFORM_HEADER}
        ${BERSERK_RHI_HEADER}
        ${BERSERK_RENDER_HEADER})

set(BERSERK_RUNTIME_SRC
        ${BERSERK_CORE_SRC}
        ${BERSERK_PLATFORM_SRC}
        ${BERSERK_RHI_SRC}
        ${BERSERK_RENDER_SRC})

if (BERSERK_TARGET_WINDOWS)
    list(APPEND BERSERK_RUNTIME_SRC ${BERSERK_PLATFORM_WINDOWS_SRC})
endif ()

if (BERSERK_TARGET_LINUX)
    list(APPEND BERSERK_RUNTIME_SRC ${BERSERK_PLATFORM_LINUX_SRC})
endif ()

if (BERSERK_TARGET_MACOS)
    list(APPEND BERSERK_RUNTIME_SRC ${BERSERK_PLATFORM_MACOS_SRC})
endif ()

if (BERSERK_WITH_GLFW)
    list(APPEND BERSERK_RUNTIME_SRC ${BERSERK_PLATFORM_GLFW_SRC})
endif ()

if (BERSERK_WITH_OPENGL)
    list(APPEND BERSERK_RUNTIME_SRC ${BERSERK_RHI_OPENGL_SRC})
endif ()

if (BERSERK_STATIC_BUILD)
    message(STATUS "Build Berserk Runtime library (static mode)")
    list(APPEND BERSERK_RUNTIME_DEFINES_PUBLIC BERSERK_STATIC_BUILD)
    add_library(berserk_runtime_static STATIC ${BERSERK_RUNTIME_HEADER} ${BERSERK_RUNTIME_SRC})
    berserk_configure_target(berserk_runtime_static)
endif ()

if (BERSERK_DYNAMIC_BUILD)
    message(STATUS "Build Berserk Runtime library (dynamic mode)")
    list(APPEND BERSERK_RUNTIME_DEFINES_PUBLIC BERSERK_DYNAMIC_BUILD)
    list(APPEND BERSERK_RUNTIME_DEFINES_PRIVATE BERSERK_EXPORTS)
    add_library(berserk_runtime_dynamic SHARED ${BERSERK_RUNTIME_HEADER} ${BERSERK_RUNTIME_SRC})
    berserk_configure_target(berserk_runtime_dynamic)

    if (BERSERK_BUILD_TESTS)
        berserk_target_copy_to(berserk_runtime_dynamic berserk_runtime_dynamic POST_BUILD code/runtime tests)
    endif ()

    if (BERSERK_BUILD_EXAMPLE)
        berserk_target_copy_to(berserk_runtime_dynamic berserk_runtime_dynamic POST_BUILD code/runtime example)
    endif ()
endif ()