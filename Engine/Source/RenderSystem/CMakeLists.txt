cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System files

set(
        SOURCES

        Private/RenderScene.cpp
        Public/RenderScene.h
        Public/RenderSceneInfo.h

)

# Create static Render Subsystem library

add_library(BerserkRenderSystem STATIC ${SOURCES})