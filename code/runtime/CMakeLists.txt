set(BERSERK_RUNTIME_MODULES_HEADER)
set(BERSERK_RUNTIME_MODULES_SRC)
set(BERSERK_RUNTIME_DEFINES)

function(berserk_configure_target target)
    berserk_target_arch(${target})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    target_link_libraries(${target} PUBLIC Yamc)
    target_link_libraries(${target} PRIVATE StbImage)
    target_link_libraries(${target} PRIVATE portable_file_dialogs)
    target_link_libraries(${target} PRIVATE WhereAmI)

    if (BERSERK_WITH_GLFW)
        target_link_libraries(${target} PRIVATE glfw)
    endif()

    if (BERSERK_WITH_OPENGL)
        target_link_libraries(${target} PRIVATE libglew_static)
    endif()

    if (BERSERK_WITH_VULKAN)
        target_link_libraries(${target} PRIVATE Vulkan)
        target_link_libraries(${target} PRIVATE Vma)
        target_link_libraries(${target} PRIVATE glslang)
        target_link_libraries(${target} PRIVATE SPIRV)
    endif()

    foreach(DEFINITION ${BERSERK_RUNTIME_DEFINES})
        target_compile_definitions(${target} PUBLIC ${DEFINITION})
    endforeach()

    berserk_target_compile_definitions(${target})
    berserk_target_compile_options(${target})
    berserk_target_link_options(${target})
endfunction()

include(core/CMakeLists.txt)

set(BERSERK_RUNTIME_HEADER
        Berserk.hpp
        ${BERSERK_CORE_HEADER})

set(BERSERK_RUNTIME_SRC
        ${BERSERK_CORE_SRC})

if (BERSERK_STATIC_BUILD)
    message(STATUS "Build Berserk Runtime library (static mode)")
    list(APPEND BERSERK_RUNTIME_DEFINES BERSERK_STATIC_BUILD)
    add_library(berserk_runtime_static STATIC ${BERSERK_RUNTIME_HEADER} ${BERSERK_RUNTIME_SRC})
    berserk_configure_target(berserk_runtime_static)
endif()

if (BERSERK_DYNAMIC_BUILD)
    message(STATUS "Build Berserk Runtime library (dynamic mode)")
    list(APPEND BERSERK_RUNTIME_DEFINES BERSERK_DYNAMIC_BUILD)
    list(APPEND BERSERK_RUNTIME_DEFINES BERSERK_EXPORTS)
    add_library(berserk_runtime_dynamic SHARED ${BERSERK_RUNTIME_HEADER} ${BERSERK_RUNTIME_SRC})
    berserk_configure_target(berserk_runtime_dynamic)
    berserk_target_copy_to_test_dir(berserk_runtime_dynamic code/runtime)
endif()