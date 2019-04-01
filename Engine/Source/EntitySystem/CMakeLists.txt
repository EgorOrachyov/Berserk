cmake_minimum_required(VERSION 3.9)
project(BerserkEntitySystem)

# Entity System files

set(SOURCE_LIB

        Private/Objects/IObjectInitializer.cpp
        Private/Objects/IObject.cpp
        Public/Objects/IObjectInitializer.h
        Public/Objects/Forward.h
        Public/Objects/IObject.h

        Public/Entities/IEntity.h
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

        )

# Create static Entity Subsystem library

add_library(BerserkEntitySystem STATIC ${SOURCE_LIB})