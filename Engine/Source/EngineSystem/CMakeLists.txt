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
        Public/Foundation/IObjectUpdater.h
        Public/Foundation/IObjectInitializer.h

        Public/Systems/ISystem.h

        Private/Components/IInputComponent.cpp
        Private/Components/SceneComponet.cpp
        Private/Components/IRenderComponent.cpp
        Private/Components/LightComponent.cpp
        Private/Components/ModelComponent.cpp
        Public/Components/IEntityComponent.h

        Public/Components/SceneComponet.h
        Public/Components/IRenderComponent.h
        Public/Components/LightComponent.h
        Public/Components/ModelComponent.h
        Public/Components/IInputComponent.h

        Private/Factory/IFactory.cpp
        Private/Factory/IFactoryInitializer.cpp
        Private/Factory/FactoryEntity.cpp
        Public/Factory/IFactory.h
        Public/Factory/IFactoryInitializer.h
        Public/Factory/FactoryEntity.h

        Private/Engine/World.cpp
        Private/Engine/WorldChunk.cpp
        Private/Engine/WorldManager.cpp
        Public/Engine/World.h
        Public/Engine/WorldChunk.h
        Public/Engine/WorldManager.h

        Private/Engine/WorldChunkManager.cpp Public/Engine/WorldChunkManager.h)

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCE_LIB})