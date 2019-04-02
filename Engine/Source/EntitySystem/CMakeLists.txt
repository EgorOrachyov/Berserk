cmake_minimum_required(VERSION 3.9)
project(BerserkEntitySystem)

# Entity System files

set(SOURCE_LIB

        Private/Foundation/IObjectInitializer.cpp
        Private/Foundation/IObject.cpp
        Public/Foundation/IObjectInitializer.h
        Public/Foundation/Forward.h
        Public/Foundation/IObject.h

        Public/Foundation/IEntity.h
        Public/Systems/ISystem.h

        Private/Components/SceneComponet.cpp
        Private/Components/IRenderComponent.cpp
        Private/Components/LightComponent.cpp
        Private/Components/ModelComponent.cpp
        Public/Components/IEntityComponent.h
        Public/Components/SceneComponet.h
        Public/Components/IRenderComponent.h
        Public/Components/LightComponent.h
        Public/Components/ModelComponent.h

        Private/Components/IInputComponent.cpp Public/Components/IInputComponent.h Private/Foundation/IObjectUpdater.cpp Public/Foundation/IObjectUpdater.h Public/Foundation/IObjectMacros.h Private/Foundation/IFactoryInitializer.cpp Public/Foundation/IFactoryInitializer.h Private/Foundation/IFactory.cpp Public/Foundation/IFactory.h Private/Foundation/IEntity.cpp)

# Create static Entity Subsystem library

add_library(BerserkEntitySystem STATIC ${SOURCE_LIB})