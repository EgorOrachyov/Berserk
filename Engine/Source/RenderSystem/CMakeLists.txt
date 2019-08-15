cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System files

set(
        SOURCES

        Private/RenderScene.cpp
        Private/PipelineScheduler.cpp
        Private/RenderUtils.cpp

        Public/RenderScene.h
        Public/RenderSceneInfo.h
        Public/ShaderData.h
        Public/IPipelineStage.h
        Public/PipelineScheduler.h
        Public/RenderUtils.h
)

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCES})