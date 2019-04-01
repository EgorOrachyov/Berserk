cmake_minimum_required(VERSION 3.9)
project(BerserkEntitySystem)

# Entity System files

set(SOURCE_LIB

        Private/Objects/IObject.cpp
        Public/Objects/IObject.h
        Public/Entities/IEntity.h
        Public/Components/IEntityComponent.h
        Public/Systems/ISystem.h

        Private/Objects/IObjectInitializer.cpp Public/Objects/IObjectInitializer.h)

# Create static Entity Subsystem library

add_library(BerserkEntitySystem STATIC ${SOURCE_LIB})