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

        Private/Base/Material.cpp
        Public/Base/Material.h

        Private/Managers/MaterialManager.cpp
        Public/Managers/MaterialManager.h

        )

# Create static Render System library

add_library(BerserkRenderSystem STATIC ${SOURCE_LIB})