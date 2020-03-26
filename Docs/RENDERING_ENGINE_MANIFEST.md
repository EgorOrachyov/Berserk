# Rendering Engine Manifest

**This document** is intended to describe main architectural considerations
for the rendering subsystem of the Berserk game engine. This document
will be used in time of the development as some kind of technical note
for better system structure understanding. 

**Node:** used only for personal usage.

## Preliminaries

One of possible variants for game engine design is splitting of the
whole working process in somehow sequenced parts, which are executed in may be
one or several working threads. Since moder hardware is highly parallelized,
using of several threads for processing is required for better performance.

Thus, engine must have clear structure where each system has responsibility to
process single piece of some data, and then synchronize its state with other systems,
in order to avoid *race conditions* and pure performance of unnecessary locks.

## Engine Architecture

From the architectural perspective we can split the flow of engine execution for
three parts:
* *Game thread* - main thread, processes all the game/scenes relevant data
* *Job thread* - executes single job, which process some engine system update.

Therefore, it is only required to communicate between different systems, which
are possibly working in the different threads in different or the same time, and
share some data between systems in order to avoid lock and unintended data modifications.

**Render thread** - is a thread, where rendering system does rendering and all the
required side operations for that. As a real thread - it could be some thread in thread
pool. From frame to frame, thread could differ, but the system always works only on one thread.

## Rendering System

Rendering system consists of the three main parts, which are spread out among engine systems.

* *Rendering Device* or *Rendering Hardware Interface* (RHI) - interface class, which 
provides access for underlying rendering API such as OpenGL, Vulkan, DirecX. [*Render Thread*]
* *Rendering Engine* - interface class, which registers scenes, light sources, objects 
and effects and does sequential rendering for each frame of the scenes. [*Render Thread*]
* *Rendering Components* - components and classes, which are attached to the game logic 
and processed by  the game thread. This components communicates with rendering system and 
sends request for rendering to it. [*Game Thread*]

**Implementation** of such engine structure could be done from the first system to the next, 
since it only has vertical dependencies quite clear threading structure.

In order to avoid unnecessary complexity of such structures, it is preferred to implement RHI 
system working  on render thread, since it is enough for computational tasks of such project.

**Note:** this implementation is inspired by UE4 threaded renderer, described 
[here](https://docs.unrealengine.com/en-US/Programming/Rendering/ThreadedRendering/index.html).

## Rendering Device (RHI)

An interface class which provides access for low level rendering API via unified 
compatibility layer with predefined set of attributes. For simple purposes functionality 
of such object could be bound by the following number of primitives:

* Index buffer
* Vertex buffer
* Uniform buffer
* Sampler
* Texture
* Shader program
* Graphics pipeline
* Framebuffer
* Framebuffer format
* Command buffer
* Time query
* Occlusion query

Because of the RHI device is working only on render thread, reference counting via shared pointer
can be employed in order to track life time of the primitives. Overhead of such solution
is 8 bytes for *virtual table* pointer and 16 bytes for shared pointer meta info for each primitive instance.






