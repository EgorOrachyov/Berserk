cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(
        SOURCES

        Public/Rendering/VertexTypes.h
        Public/Rendering/RenderingCommon.h
        Public/Application/IWindow.h
        Public/Application/IWindowManager.h
        Public/Application/IApplication.h
)

# Create static Engine Subsystem library

#add_library(BerserkEngineSystem STATIC ${SOURCE_LIB})