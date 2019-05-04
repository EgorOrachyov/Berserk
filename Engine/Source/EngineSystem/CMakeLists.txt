cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(SOURCE_LIB

        Private/Foundation/IObject.cpp
        Private/Foundation/IObjectUpdater.cpp
        Private/Foundation/IObjectInitializer.cpp

        Public/Foundation/IObjectMacros.h
        Public/Foundation/Forward.h
        Public/Foundation/IObject.h
        Public/Foundation/IObjectUpdater.h
        Public/Foundation/IObjectInitializer.h

        Private/Components/IInputComponent.cpp
        Private/Components/SceneComponent.cpp
        Private/Components/SpotLightComponent.cpp
        Private/Components/PointLightComponent.cpp
        Private/Components/DirectionalLightComponent.cpp
        Private/Components/LightComponent.cpp
        Private/Components/IPrimitiveComponent.cpp
        Private/Components/StaticMeshComponent.cpp

        Public/Components/IEntityComponent.h
        Public/Components/SceneComponent.h
        Public/Components/IInputComponent.h
        Public/Components/SpotLightComponent.h
        Public/Components/PointLightComponent.h
        Public/Components/DirectionalLightComponent.h
        Public/Components/LightComponent.h
        Public/Components/IPrimitiveComponent.h
        Public/Components/StaticMeshComponent.h
        Public/Components/MeshComponent.h

        Private/GameFramework/IEntity.cpp
        Public/GameFramework/IEntity.h
        Public/GameFramework/Entity.h

        Private/Engine/World.cpp
        Private/Engine/WorldChunk.cpp
        Private/Engine/WorldManager.cpp
        Private/Engine/WorldChunkManager.cpp

        Public/Engine/World.h
        Public/Engine/WorldChunk.h
        Public/Engine/WorldManager.h
        Public/Engine/WorldChunkManager.h

        Private/Systems/ISystemInitializer.cpp
        Public/Systems/ISystemInitializer.h
        Public/Systems/ISystem.h

        Public/Delegates/DelegatesMacros.h


        Private/Components/CameraComponent.cpp Public/Components/CameraComponent.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCE_LIB})