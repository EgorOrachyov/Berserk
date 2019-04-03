cmake_minimum_required(VERSION 3.9)
project(BerserkEngineSystem)

# Engine System files

set(SOURCE_LIB

        Private/Foundation/IObject.cpp
        Private/Foundation/IEntity.cpp
        Private/Foundation/IFactory.cpp
        Private/Foundation/IObjectUpdater.cpp
        Private/Foundation/IObjectInitializer.cpp
        Private/Foundation/IFactoryInitializer.cpp

        Public/Foundation/IObjectMacros.h
        Public/Foundation/Forward.h
        Public/Foundation/IObject.h
        Public/Foundation/IEntity.h
        Public/Foundation/IFactory.h
        Public/Foundation/IObjectUpdater.h
        Public/Foundation/IObjectInitializer.h
        Public/Foundation/IFactoryInitializer.h

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

        )

# Create static Engine Subsystem library

add_library(BerserkEngineSystem STATIC ${SOURCE_LIB})