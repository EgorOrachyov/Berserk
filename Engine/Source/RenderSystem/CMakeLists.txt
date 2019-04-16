cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System Subsystem files

set(SOURCE_LIB

        # Queue submodule's files

        Private/Queue/RenderQueue.cpp
        Public/Queue/RenderQueue.h

        # Debug submodule's files

        Private/Debug/DebugRenderManager.cpp
        Public/Debug/DebugRenderManager.h

        # Foundation submodule's files

        Private/Foundation/RenderSystem.cpp
        Private/Foundation/Material.cpp
        Public/Foundation/RenderSystem.h
        Public/Foundation/Material.h

        # Managers submodule's files

        Private/Managers/MaterialManager.cpp
        Public/Managers/MaterialManager.h

        Private/Pipeline/GBuffer.cpp Public/Pipeline/GBuffer.h Private/Pipeline/DeferredLightning.cpp Public/Pipeline/DeferredLightning.h Private/Pipeline/ToneMapping.cpp Public/Pipeline/ToneMapping.h Private/Foundation/PipelineScheduler.cpp Public/Foundation/PipelineScheduler.h Private/Managers/DebugDrawManager.cpp Public/Managers/DebugDrawManager.h Public/Queue/RenderQueueNode.h Private/Culling/ViewVolumeCulling.cpp Public/Culling/ViewVolumeCulling.h Public/Queue/SubmitQueueNode.h Public/Queue/RenderQueueType.h)

# Create static Render System library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})