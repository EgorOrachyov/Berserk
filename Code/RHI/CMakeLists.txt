cmake_minimum_required(VERSION 3.11)
project(BerserkRHI LANGUAGES CXX)

set(BerserkRHI_SOURCES
        BerserkRHI/RHIDevice.cpp
        BerserkRHI/RHIDevice.hpp
        BerserkRHI/RHIContext.hpp
        BerserkRHI/RHICmdList.hpp
        BerserkRHI/RHIResource.hpp
        BerserkRHI/RHIDefs.hpp
        BerserkRHI/RHIVertexBuffer.hpp
        BerserkRHI/RHIIndexBuffer.hpp
        BerserkRHI/RHIUniformBuffer.hpp
        BerserkRHI/RHIVertexDeclaration.hpp
        BerserkRHI/RHISampler.hpp
        BerserkRHI/RHIProgram.hpp
        BerserkRHI/RHIProgramMeta.hpp
        BerserkRHI/RHITexture.hpp
        BerserkRHI/RHIRenderTarget.hpp
        BerserkRHI/RHIRenderPass.hpp
        BerserkRHI/RHIPipelineState.hpp
        BerserkRHI/RHIDriver.cpp
        BerserkRHI/RHIDriver.hpp
        )

add_library(BerserkRHI STATIC ${BerserkRHI_SOURCES})
target_include_directories(BerserkRHI PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkRHI PUBLIC BerserkCore)

target_compile_features(BerserkRHI PUBLIC cxx_std_11)

set_target_properties(BerserkRHI PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRHI PROPERTIES CXX_STANDARD_REQUIRED YES)