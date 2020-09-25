# Engine Design Notes

Created by: Egor Orachyov  
Part of: Berserk Engine Project

This file is intended to store basic notes and fundamental design decisions
about the engine implementation. 

## Project purpose

Purpose of this project is to create a basic game engine - a c++ language powered
framework for creating the test game in 3d third-person genre. This engine must 
be simple, clean, not powerful or cutting-edge, but suitable for 3d game with 
physics, graphics, post-effects, shadows and visual effects.

In order to make game creation an easier process, I suppose that its worth implementing
a simple *Editor* on top of the engine infrastructure, with basic reflection and scripting bridge.

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
- OS full abstraction
- Raw window management interface
- Raw input management interface
- Rendering Hardware interface
- Math functions and objects
- Strings, hashed strings and localization strings
- Plugins manager
- Events manager
- Task manager
- IO containers, parsers and archives
- Logging and error tracking
- Application skeleton and access points

> The primary goal now is to implement minimal required functionality, needed for 
> creating the main rendering module in order to faster prototype graphics and
> visual aspects of the development.

### RHI

Rendering hardware interface is an abstract factory which provides access for low-level
rendering specific primitives, such as vertex buffer, gpu programs, textures, etc.

Required primitives:

- Gpu program (linked vertex and fragment shaders from sources)
- Vertex buffer (usage, stride, vertices count, and attributes info)
- Index buffer (usage, indices type and indices count)
- Uniform buffer (size in bytes with usage)
- Render target (size, set of color textures, and optional depth stencil texture)
- Sampler (compat sampling settings)
- Texture (width, height, depths, mips count, format, data, texture usage, sampling type)
- RenderPrimitive (vertex buffers, optional index buffer, instances count, indices/vertices count)
- CommandList (capturing draw commands)
- Driver (creating resources, submitting draw commands)

### String

## Render



