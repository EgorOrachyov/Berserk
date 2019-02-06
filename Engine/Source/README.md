# Engine Structure

Engine architecture follows sub system modeling pattern, which allows to split framework's functionality on 
separated modules with related features. The source code is structured by folders which contains associated
engine modules definition and implementation parts.  

## Modules

Modules' design employs the following rule: 

* Public folder contains header files only which could be included by user.  
* Private folder contains source (cpp) files, compiled in static lib.

## Core

Basic functionality, foundation and common tools for creating and implementing other engine module parts.
Evolves as subset of features for working with math, strings, XML, time, SIMD instructions, platforms and drivers 
configuration, containers and threads. Provides sufficient functionality in memory managing, allows to omit platform 
malloc / free and use engine build-in specialized pool, stack, linear and list allocators. 

## Engine

## Application

## Render

Graphics core and the most powerful and important part of the engine. 
It provides features for rendering objects and applying effects (post process effects) 
in soft real time mode. Basically, it is created on top of the OpenGL graphics driver, 
however, it could be expanded on the other drivers and libraries (DirectX, VulkanAPI). 

## Physics

## Audio

## AI

## UI

Complete definition and description of mentioned modules could be found in the related README.md files.
