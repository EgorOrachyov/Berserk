cmake_minimum_required(VERSION 3.9)
project(BerserkLayout)

# Core Subsystem files

set(SOURCE_LIB
        System/System.h
        System/RenderSystem.h
        Source/Cube.cpp
        Shapes/Cube.h
        Source/Plane.cpp
        Shapes/Plane.h
        Source/Sphere.cpp
        Shapes/Sphere.h
        Source/Torus.cpp
        Shapes/Torus.h
        Source/Object.cpp
        Objects/Object.h
        Source/Actor.cpp
        Objects/Actor.h
        Debug/LayoutClassTesting.h
        Source/ObjectManager.cpp
        Managers/ObjectManager.h
        Source/Light.cpp
        Objects/Lights/Light.h
        Source/PointLight.cpp
        Objects/Lights/PointLight.h
        Source/DirectionalLight.cpp
        Objects/Lights/DirectionalLight.h
        Source/SpotLight.cpp
        Objects/Lights/SpotLight.h
        Source/Camera.cpp
        Objects/Cameras/Camera.h
        Source/RenderManager.cpp
        Managers/RenderManager.h
        Source/SceneManager.cpp
        Managers/SceneManager.h
        Managers/Manager.h)

# Create static Core Subsystem library

add_library(BerserkLayout STATIC ${SOURCE_LIB})