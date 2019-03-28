cmake_minimum_required(VERSION 3.9)
project(BerserkEntitySystem)

# Entity System files

set(SOURCE_LIB

        Public/Base/IObject.h
        Public/Base/IEntity.h
        Public/Base/IComponent.h
        Public/Base/ISystem.h

        )

# Create static Entity Subsystem library

add_library(BerserkEntitySystem STATIC ${SOURCE_LIB})