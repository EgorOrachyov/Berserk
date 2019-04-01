cmake_minimum_required(VERSION 3.9)
project(BerserkEntitySystem)

# Entity System files

set(SOURCE_LIB

        Private/Objects/IObject.cpp
        Public/Objects/IObject.h
        Public/Objects/IEntity.h
        Public/Objects/IEntityComponent.h
        Public/Objects/ISystem.h

        )

# Create static Entity Subsystem library

add_library(BerserkEntitySystem STATIC ${SOURCE_LIB})