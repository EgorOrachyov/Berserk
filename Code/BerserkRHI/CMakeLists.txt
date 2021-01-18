cmake_minimum_required(VERSION 3.11)
project(BerserkRHI VERSION 1.0 LANGUAGES CXX)

set(BerserkRHI_SOURCES
        BerserkRHI/RHIDriver.cpp
        BerserkRHI/RHIDriver.hpp
        BerserkRHI/RHIResource.hpp
        BerserkRHI/RHIDefs.hpp
        BerserkRHI/RHIVertexBuffer.hpp
        BerserkRHI/RHIIndexBuffer.hpp
        BerserkRHI/RHIUniformBuffer.hpp
        BerserkRHI/RHIVertexDeclaration.hpp
        BerserkRHI/RHISampler.hpp
        BerserkRHI/RHIProgram.hpp
        BerserkRHI/RHITexture.hpp
        BerserkRHI/RHIRenderTarget.hpp
        BerserkRHI/RHIPipelineState.hpp
        BerserkRHI/RHIContext.hpp BerserkRHI/RHIRenderPass.hpp)

add_library(BerserkRHI SHARED ${BerserkRHI_SOURCES})
target_include_directories(BerserkRHI PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkRHI PUBLIC BerserkCore)

target_compile_features(BerserkRHI PUBLIC cxx_std_11)

set_target_properties(BerserkRHI PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkRHI PROPERTIES CXX_STANDARD_REQUIRED YES)