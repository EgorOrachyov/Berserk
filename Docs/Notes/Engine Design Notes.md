# Engine Design Notes

Created by: Egor Orachyov  
Part of: Berserk Engine Project
Date: 26 Sept. 2020

This file is intended to store basic notes and fundamental design decisions
about the engine implementation. 

## Project purpose

Purpose of this project is to create a basic game engine - a c++ language powered
framework for creating the test game in 3d third-person genre. This engine must 
be simple, clean, not powerful or cutting-edge, but suitable for 3d game with 
physics, graphics, post-effects, shadows and visual effects.

In order to make game creation an easier process, I suppose that its worth implementing
a simple *Editor* on top of the engine infrastructure, with basic reflection and scripting bridge.

## Scope

Project scope goes here...

## Glossary

Fancy words and other stuff...

## Requirements

- Core functionality for every-day stuff
- Platform specific functionality abstraction
- JSON files support
- INI files support
- RHI device abstraction and OpenGL implementation
- Embedded dependencies (submodules in the future)
- Unit testing for basic modules
- Rich Shader program description, based on *Techniques*
- Dynamic materials loading
- Multiple scene and view support
- Concurrent drawing process 
- Scripting language support
- Scene hierarchy based on Entities and Entity Components
- Dynamic extension via Plugins and Registration
- Visual node-based scripting
- Basic GUI system
- 3D Audio support
- 3D physics integration
- Basic online editor 

> Since it is one-person hobby project, the requirements may be simplified or
> reinterpreted in order to speed-up implementation and prototyping process.

## Core

Core module is the central module, which provides basic functionality abstraction level
for various OS specific features. This module is the root in terms of the linkage,
all the other engine modules except *Thirdparty* inherit this module.

Basic features, provided by almost any game engine Core module are system specifics,
files, directories, window management, input, low-level rendering hardware interface,
math and vectorization instructions, IO, as well as strings, containers, and
various serializations tools, such as archives, json or ini file parsers.

Brief description for each module is provided bellow with some comments and development notes.

Main Core module must provide:
-[ ] OS full abstraction
-[ ] Raw window management interface
-[ ] Raw input management interface
-[ ] Rendering Hardware interface
-[ ] Math functions and objects
-[ ] Strings, hashed strings and localization strings
-[ ] Plugins manager
-[ ] Events manager
-[ ] Task manager
-[ ] IO containers, parsers and archives
-[ ] Logging and error tracking
-[ ] Application skeleton and access points

> The primary goal now is to implement minimal required functionality, needed for 
> creating the main rendering module in order to faster prototype graphics and
> visual aspects of the development.

### RHI

Rendering hardware interface is an abstract factory which provides access for low-level
rendering specific primitives, such as vertex buffer, gpu programs, textures, etc.

> Primary goal for an RHI device is to hide platform/implementation specific thing,
> related to the particular drawing API, which could be quite complex. For this 
> project, potential APIs are following: OpenGL (4.1 or 4.5), Vulkan (in theory),
> and DirectX 11 (if the engine will be adopted for Windows).

Required primitives:

-[ ] Gpu program (linked vertex and fragment shaders from sources)
-[ ] Vertex buffer (usage, stride, vertices count, and attributes info)
-[ ] Index buffer (usage, indices type and indices count)
-[ ] Uniform buffer (size in bytes with usage)
-[ ] Render target (size, set of color textures, and optional depth stencil texture)
-[ ] Sampler (compat sampling settings)
-[ ] Texture (width, height, depths, mips count, format, data, texture usage, sampling type)
-[ ] Render primitive (vertex buffers, optional index buffer, instances count, indices/vertices count)
-[ ] Command list (capturing draw commands)
-[ ] Driver (creating resources, submitting draw commands)

The process of the rendering will be represented as an CommandList allocation with
the following commands generations. In order to draw simple geometry on must
bind the rendering target for drawing, specify clear operation and values,
provide viewport settings. Provide Gpu draw pipeline state, which consists of 
Gpu program, depth/stencil states, blend operations and resterization settings.
Then one must provide set of textures and uniform buffers, which will be bound 
to the pipeline for rendering, bind the render primitive and call draw command.

> In order to simplify the implementation of the RHI and reduce number of dynamic
> memory allocations, the interface will rely on some *magic* constants, which are
> chosen from the point of view of the project needs.

Constants to fixed in compile time:
- Max number of the vertex attributes `const = 8`
- Max number of texture units `const = 16`
- Max number of uniform buffers `const = 16`
- Max number of color targets `const = 8`

Gpu program assembly must be done by the RHI Driver. In order to create Gpu 
program one must provide source code in form of the list of bytes for each
shader stage, provide compilation context (i.e. definitions, which will be added
by the compiler), and provide an Observer, used to track the compilation process.

### String

## Render

### Shader Core

### Material Core

### Scene Renderer

