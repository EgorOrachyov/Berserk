cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System files

set(
        SOURCES

        Private/RenderScene.cpp
        Private/RenderUtils.cpp
        Private/RenderSystem.cpp
        Private/ShaderManager.cpp
        Private/RenderTargetManager.cpp

        Public/RenderScene.h
        Public/RenderSceneInfo.h
        Public/ShaderData.h
        Public/IPipelineStage.h
        Public/IPipelineScheduler.h
        Public/RenderUtils.h
        Public/RenderSystem.h
        Public/ShaderManager.h
        Private/PipelineStageManager.cpp
        Public/PipelineStageManager.h
        Public/PipelinePassContext.h
        Public/RenderTargetManager.h

)

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCES})