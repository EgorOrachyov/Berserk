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
- [ ] OS full abstraction
- [ ] Raw window management interface
- [ ] Raw input management interface
- [ ] Rendering Hardware interface
- [ ] Math functions and objects
- [ ] Strings, hashed strings and localization strings
- [ ] Plugins manager
- [ ] Events manager
- [ ] Task manager
- [ ] IO containers, parsers and archives
- [ ] Logging and error tracking
- [ ] Application skeleton and access points

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

- [ ] Gpu program (linked vertex and fragment shaders from sources)
- [ ] Vertex buffer (usage, stride, vertices count, and attributes info)
- [ ] Index buffer (usage, indices type and indices count)
- [ ] Uniform buffer (size in bytes with usage)
- [ ] Render target (size, set of color textures, and optional depth stencil texture)
- [ ] Sampler (compat sampling settings)
- [ ] Texture (width, height, depths, mips count, format, data, texture usage, sampling type)
- [ ] Render primitive (vertex buffers, optional index buffer, instances count, indices/vertices count)
- [ ] Command list (capturing draw commands)
- [ ] Driver (creating resources, submitting draw commands)

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

There is another thing to consider: shall we do the RHI module resources in the immutable 
style in order to provide safe concurrent execution flow. This leads to the fact,
that all the buffer resources will be recreated each time we need to update them.
However, there are some resources, that already not frequently updated, such as
index buffers, gpu programs, samplers, textures (for materials), draw primitives.
Vertex buffers may be updated (for immediate mode drawing operations), textures
are also updated when used as Render Targets, uniform buffers updated nearly each frame.

Also there is the question, how to update this things. In order to implement
concurrent RHI execution (when gl commands are executed only on single worker thread),
all updates must be captured as command list commands.

```c++
Ptr<CommandList> Cmd = mDriver.AllocateCmdList();
Cmd.Begin();
// Some operations
Cmd.UpdateBuffer(mVertexBuffer, offset, range, mMemoryPtr);
// After that point update is visible for all upcoming commands
// Some operations
Cmd.End();
```

### String

Strings to be supported:
- Dynamically allocated strings based on templates
- Hashed string names for strings with immutable shader buffer state
- Unicode strings (iterable containers of code points in UTF-32, UTF-8 formats)

### Logging

Log features:
- Generic platform log
- Log listeners
- Log dump
- Log save on disk in html format for user friendly view

## Render

### Render targets

Render target is a drawing object with a property, that it can receive the 
results of the drawing operations on the Gpu. Typical render targets examples
are OS windows, 2D textures, G-Buffers and etc.

Each target must have the following features:
- Drawing area size
- Ability to be bound to the drawing list for drawing
- Params passed on drawing: viewport (within the draw area)  

### Shader Core

Infrastructure, required for loading, compiling, instancing, and storing gpu programs,
pipeline states and meta flags, required for drawing with high-eng RHI devices. 
At this point, shader - is a complete set actions, required to perform some drawing 
logic on the Gpu. The shader with the source data, uniform bindings and render targets 
define the complete drawing flow.

Shader consists of:
- Name, which identifies shader for the engine
- Description, optional text for user
- Compile time flags, which could be set or not
- Set of techniques

Where Technique:
- Has Name, unique among other techniques of the current shader
- Has list of tags, which define, where the technique is available for the engine
- Has a priority level to sort the techniques (when available more than one)
- Has the shading language type specification (GLSL)
- Set of passes

Where Pass:
- Has Name, unique among other passes of the current technique
- Has Raster state
- Has Depth/Stencil state
- Has Blend state
- Source include look-up paths
- Source code per vertex/fragment shader stages 

In runtime, shader represents some kind of template, which can be used to instantiate
concrete ready-for-use shader objects by providing set of compile time flags.

Each shader instance is identified by shader name and set of compile flags.

In order to support multiple platforms with multiple shader code sources, one
must provided techniques for each platform, where each technique will encapsulate
all the rendering specifics.

In order to communicate with shaders, each shader exposes params and attributes,
which could be accessed via unified shader interface.

Shaders must be implemented in the way, that the user/programmer can access a
fully featured and ready for use program for drawing via instantiating the
shader with name and compile flags.

Shader techniques may consists of the several drawing passes,
which may be required for rendering some post-process effects or transparent geometry.

How to specify compilation flags:
- Each flag has unique name
- Each flag has default value (?)

How to specify exposed params:
- An exposed param has a unique name 
- A type, which is one of: texture, matrix or vector 
- A visibility flags (?)
- A mapping rules for concrete shader stage

How to specify exposed attributes:
- Each attribute has a name, type and semantic (?)
- Attributes locations will be queried by introspection tool 
- Attributes must be passed in the form of the drawing primitives
- All primitives must be specified before the actual draw

```c++
ShaderPtr mShader = mShaderMan.InstantiateShader(mShaderName, mFlagsSet);
mShader.SetAttributes(...);
mShader.SetParams(...);
mShader.Draw();
```

> The primary goal here is to design shader core, which will be reliable for the
> following material system implementation, where materials are something, that 
> build on top of the shaders with additional data/settings specification.

```
Shader 
    |_name
    |_flags
    |_description
    |_exposed params
        |_param0 (name, type)
        |_param2 (name, type)
        ...
        |_paramN (name, type)
    |_exposed attribures
        |_attribute set0 (?)
        |_attribute set1 (?)
        ...
        |_attribute setN (?)
    |_techniques
        |_technique0 
            |_name
            |_description
            |_tags
            |_language
            |_priority
            |_passes
                |_pass0 
                    |_name
                    |_raster state
                    |_depth/stencil state
                    |_blend state
                    |_params mappings
                    |_include paths
                    |_vertex code
                    |_fragment code
                |_pass1
                    ...
                ...
                |_passN
        ...
        |_techniqueN
```

### Immediate Drawing

In order to implement GUI engine system, the Render module must provide sufficient
immediate-drawing style rendering interface, which allows to draw basic primitives
in the provided drawing area/areas.

Features to be supported:
- Clear operation
- Text rendering
- Image rendering
    * With alpha
    * With rotation and scaling
    * With specified area
    * Instanced images
- Drawing primitives (wire and filled)
    * Rectangle
    * Circle
    * Ellipse
    * Triangle
    * Triangle mesh (or polygons list)
    
Dependencies, required to implement this features:
- Font family resource
- Font instance resource
- Shader object (loading, compiling and instancing)
- RHI primitives
- Render targets (bindable proxy targets)
- Image (and texture wrapper, which can be used for instanced icons drawing)

### Material Core

### Scene Renderer

