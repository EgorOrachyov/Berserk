cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(SOURCE_LIB

        Private/Foundation/IObject.cpp
        Private/Foundation/IEntity.cpp
        Private/Foundation/IObjectUpdater.cpp
        Private/Foundation/IObjectInitializer.cpp

        Public/Foundation/IObjectMacros.h
        Public/Foundation/Forward.h
        Public/Foundation/IObject.h
        Public/Foundation/IEntity.h
        Public/Foundation/Entity.h
        Public/Foundation/IObjectUpdater.h
        Public/Foundation/IObjectInitializer.h

        Public/Systems/ISystem.h

        Private/Components/IInputComponent.cpp
        Private/Components/SceneComponent.cpp
        Private/Components/IRenderComponent.cpp
        Private/Components/ModelComponent.cpp
        Private/Components/SpotLightComponent.cpp
        Private/Components/PointLightComponent.cpp
        Private/Components/DirectionalLightComponent.cpp
        Private/Components/LightSourceComponent.cpp

        Public/Components/IEntityComponent.h
        Public/Components/SceneComponent.h
        Public/Components/IRenderComponent.h
        Public/Components/ModelComponent.h
        Public/Components/IInputComponent.h
        Public/Components/SpotLightComponent.h
        Public/Components/PointLightComponent.h
        Public/Components/DirectionalLightComponent.h
        Public/Components/LightSourceComponent.h

        Private/Factory/IFactory.cpp
        Private/Factory/IFactoryInitializer.cpp
        Private/Factory/FactoryEntity.cpp
        Public/Factory/IFactory.h
        Public/Factory/IFactoryInitializer.h
        Public/Factory/FactoryEntity.h

        Private/Engine/World.cpp
        Private/Engine/WorldChunk.cpp
        Private/Engine/WorldManager.cpp
        Private/Engine/WorldChunkManager.cpp

        Public/Engine/World.h
        Public/Engine/WorldChunk.h
        Public/Engine/WorldManager.h
        Public/Engine/WorldChunkManager.h

        Private/Factory/FactorySceneComponent.cpp Public/Factory/FactorySceneComponent.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCE_LIB})