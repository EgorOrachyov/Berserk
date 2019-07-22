cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(
        SOURCES

        Public/Rendering/VertexTypes.h
        Public/Rendering/RenderingCommon.h
        Public/Rendering/Definitions.h

        Public/Application/IWindow.h
        Public/Application/IWindowManager.h
        Public/Application/IApplication.h

        Public/Importers/IShaderImporter.h
        Private/Importers/XMLShaderImporter.cpp
        Public/Importers/XMLShaderImporter.h
        Public/Importers/IImageImporter.h
        Public/Importers/IFontImporter.h

        Public/System/ISystem.h

        Public/Resource/IResource.h
        Public/Config/IConfigTable.h Private/Config/ConfigTable.cpp Public/Config/ConfigTable.h Private/Config/XMLConfigLoader.cpp Public/Config/XMLConfigLoader.h Public/System/ICommandQueue.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCES})