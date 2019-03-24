cmake_minimum_required(VERSION 3.9)
project(BerserkRenderSystem)

# Render System Subsystem files

set(SOURCE_LIB

        # Queue submodule's files

        Private/Queue/RenderQueue.cpp
        Public/Queue/RenderQueue.h

        # System submodule's files

        Private/System/RenderSystem.cpp
        Public/System/RenderSystem.h

        # Debug submodule's files

        Private/Debug/DebugRenderManager.cpp
        Public/Debug/DebugRenderManager.h

        Public/Material/Material.cpp
        Public/Material/Material.h
        Public/Material/MaterialManager.cpp
        Public/Material/MaterialManager.h

        )

# Create static Render System library

add_library(BerserkRenderCore STATIC ${SOURCE_LIB})