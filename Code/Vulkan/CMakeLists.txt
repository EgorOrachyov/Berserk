cmake_minimum_required(VERSION 3.11)
project(BerserkVulkan LANGUAGES CXX)

set(BerserkVulkan_SOURCES
        BerserkVulkan/VulkanCmdList.hpp
        BerserkVulkan/VulkanContext.cpp
        BerserkVulkan/VulkanContext.hpp
        BerserkVulkan/VulkanDeferredResources.cpp
        BerserkVulkan/VulkanDeferredResources.hpp
        BerserkVulkan/VulkanDefs.hpp
        BerserkVulkan/VulkanDevice.cpp
        BerserkVulkan/VulkanDevice.hpp
        BerserkVulkan/VulkanDriver.cpp
        BerserkVulkan/VulkanDriver.hpp
        )

add_library(BerserkVulkan STATIC ${BerserkVulkan_SOURCES})
target_include_directories(BerserkVulkan PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkVulkan PUBLIC BerserkCore)
target_link_libraries(BerserkVulkan PUBLIC BerserkRHI)
target_link_libraries(BerserkVulkan PRIVATE Vma)

target_compile_features(BerserkVulkan PUBLIC cxx_std_11)

set_target_properties(BerserkVulkan PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkVulkan PROPERTIES CXX_STANDARD_REQUIRED YES)