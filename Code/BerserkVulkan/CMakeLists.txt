cmake_minimum_required(VERSION 3.11)
project(BerserkVulkan LANGUAGES CXX)

set(BerserkVulkan_SOURCES
        BerserkVulkan/VulkanBuffer.cpp
        BerserkVulkan/VulkanBuffer.hpp
        BerserkVulkan/VulkanCmdBufferManager.cpp
        BerserkVulkan/VulkanCmdBufferManager.hpp
        BerserkVulkan/VulkanCmdBufferPool.cpp
        BerserkVulkan/VulkanCmdBufferPool.hpp
        BerserkVulkan/VulkanCmdList.hpp
        BerserkVulkan/VulkanContext.cpp
        BerserkVulkan/VulkanContext.hpp
        BerserkVulkan/VulkanDebug.cpp
        BerserkVulkan/VulkanDebug.hpp
        BerserkVulkan/VulkanDeferredResources.cpp
        BerserkVulkan/VulkanDeferredResources.hpp
        BerserkVulkan/VulkanDefs.hpp
        BerserkVulkan/VulkanDescriptorSetManager.cpp
        BerserkVulkan/VulkanDescriptorSetManager.hpp
        BerserkVulkan/VulkanDevice.cpp
        BerserkVulkan/VulkanDevice.hpp
        BerserkVulkan/VulkanDriver.cpp
        BerserkVulkan/VulkanDriver.hpp
        BerserkVulkan/VulkanFramebuffer.cpp
        BerserkVulkan/VulkanFramebuffer.hpp
        BerserkVulkan/VulkanFramebufferCache.cpp
        BerserkVulkan/VulkanFramebufferCache.hpp
        BerserkVulkan/VulkanIndexBuffer.hpp
        BerserkVulkan/VulkanMemoryManager.cpp
        BerserkVulkan/VulkanMemoryManager.hpp
        BerserkVulkan/VulkanPhysicalDevice.cpp
        BerserkVulkan/VulkanPhysicalDevice.hpp
        BerserkVulkan/VulkanPipelineCache.cpp
        BerserkVulkan/VulkanPipelineCache.hpp
        BerserkVulkan/VulkanProgram.cpp
        BerserkVulkan/VulkanProgram.hpp
        BerserkVulkan/VulkanProgramCompiler.cpp
        BerserkVulkan/VulkanProgramCompiler.hpp
        BerserkVulkan/VulkanProxy.hpp
        BerserkVulkan/VulkanQueues.cpp
        BerserkVulkan/VulkanQueues.hpp
        BerserkVulkan/VulkanResource.cpp
        BerserkVulkan/VulkanResource.hpp
        BerserkVulkan/VulkanSampler.cpp
        BerserkVulkan/VulkanSampler.hpp
        BerserkVulkan/VulkanStagePool.cpp
        BerserkVulkan/VulkanStagePool.hpp
        BerserkVulkan/VulkanSurface.cpp
        BerserkVulkan/VulkanSurface.hpp
        BerserkVulkan/VulkanSurfaceManager.cpp
        BerserkVulkan/VulkanSurfaceManager.hpp
        BerserkVulkan/VulkanTexture.cpp
        BerserkVulkan/VulkanTexture.hpp
        BerserkVulkan/VulkanUniformBuffer.hpp
        BerserkVulkan/VulkanUtils.cpp
        BerserkVulkan/VulkanUtils.hpp
        BerserkVulkan/VulkanVertexBuffer.hpp
        BerserkVulkan/VulkanVertexDeclaration.hpp
        )

add_library(BerserkVulkan STATIC ${BerserkVulkan_SOURCES})
target_include_directories(BerserkVulkan PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkVulkan PUBLIC BerserkCore)
target_link_libraries(BerserkVulkan PUBLIC BerserkRHI)
target_link_libraries(BerserkVulkan PUBLIC Vulkan)
target_link_libraries(BerserkVulkan PRIVATE Vma)
target_link_libraries(BerserkVulkan PRIVATE glslang)
target_link_libraries(BerserkVulkan PRIVATE SPIRV)

target_compile_features(BerserkVulkan PUBLIC cxx_std_11)

set_target_properties(BerserkVulkan PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkVulkan PROPERTIES CXX_STANDARD_REQUIRED YES)