cmake_minimum_required(VERSION 3.9)
project(BerserkRHI)

# RHI Subsystem files

set(
        SORCES

        Private/Some.cpp
        Public/RHIDriver.h
        Public/RHITexture.h
        Public/RHITexture2D.h
        Public/RHITextureCube.h
        Public/RHIUniformBuffer.h
        Public/RHIFrameBuffer.h
        Public/RHIVertexBuffer.h
        Public/RHIIndexBuffer.h
        Public/RHIShader.h
)

# Create static RHI Subsystem library

add_library(BerserkRHI STATIC ${SORCES})