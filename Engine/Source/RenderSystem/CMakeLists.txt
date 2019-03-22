cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render Core Subsystem files

set(SOURCE_LIB

        # Queue submodule's files

        Private/Queue/RenderQueue.cpp
        Public/Queue/RenderQueue.h

        # System submodule's files

        Private/System/RenderSystem.cpp
        Public/System/RenderSystem.h

        )

# Create static Render Core library

add_library(BerserkRenderCore STATIC ${SOURCE_LIB})