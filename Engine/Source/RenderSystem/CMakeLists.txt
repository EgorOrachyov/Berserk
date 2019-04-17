cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System Subsystem files

set(SOURCE_LIB

        # Queue submodule's files

        Private/Queue/RenderQueue.cpp
        Public/Queue/RenderQueue.h
        Public/Queue/SubmitQueueNode.h
        Public/Queue/RenderQueueType.h

        # Managers submodule's files

        Private/Managers/FontManager.cpp
        Private/Managers/DebugDrawManager.cpp
        Private/Managers/MaterialManager.cpp
        Public/Managers/FontManager.h
        Public/Managers/DebugDrawManager.h
        Public/Managers/MaterialManager.h

        # Foundation submodule's files

        Private/Foundation/RenderSystem.cpp
        Private/Foundation/Material.cpp
        Private/Foundation/PipelineScheduler.cpp
        Public/Foundation/RenderSystem.h
        Public/Foundation/Material.h
        Public/Foundation/PipelineScheduler.h
        Public/Foundation/Font.h

        # Pipeline submodule's files

        Private/Pipeline/GBuffer.cpp
        Public/Pipeline/GBuffer.h
        Private/Pipeline/DeferredLightning.cpp
        Public/Pipeline/DeferredLightning.h
        Private/Pipeline/ToneMapping.cpp
        Public/Pipeline/ToneMapping.h
        Public/Queue/RenderQueueNode.h

        # Culling submodule's files

        Private/Culling/ViewVolumeCulling.cpp
        Public/Culling/ViewVolumeCulling.h

        Private/Foundation/Font.cpp)

# Create static Render System library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})