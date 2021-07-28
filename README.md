![Project logo](https://github.com/EgorOrachyov/Berserk/blob/master/Docs/Images/logo-main.png)

# BERSERK

[![Windows](https://github.com/EgorOrachyov/Berserk/actions/workflows/windows.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/windows.yml)
[![MacOS](https://github.com/EgorOrachyov/Berserk/actions/workflows/macos.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/macos.yml)
[![Ubuntu](https://github.com/EgorOrachyov/Berserk/actions/workflows/ubuntu.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/ubuntu.yml)
![Codacy Badge](https://app.codacy.com/project/badge/Grade/674eff47dbfa45e38c5fd3765f3256ba)
[![License](https://img.shields.io/badge/license-MIT-orange)](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md)

**Berserk engine** is designed as a powerful and compact tool for creating high-performance applications.
It will be written in C++11 with support for Vulkan and OpenGL graphics, OpenAL audio, Bullet Physics, 
Assimp importer for models, FreeImage texture loader and FreeType font loader.

**The engine will provide** functionality in memory management, math, SIMD instructions set utilisation, 
working with strings, multi-threading, exporting/importing various kinds of resources (such as textures, 
geometry, audio files etc.), displaying fonts, text and 3d graphics.

**The idea** behind this hobby project is to implement a simple, compact, friendly, and at the same 
time powerful enough engine in order to create a 3d third-person platformer game, which is fully 
featured with modern graphics, visual effects, audio, physics, and scripting features. Since I am 
a big fan of 3d-graphics, games, and programming, this project will allow me to practice my skills 
as well as have a lot of fun with fancy programming tasks and design decisions. 

> **At this moment** the primary goal is to create core structure with an usable stuff,
> which will be expanded later by the engine modules/components. 
> Project under heavy development. 

### Features

- [X] Platform abstraction
- [X] Templates, containers and custom strings
- [X] Logging and basic debug features
- [X] File I/O abstraction
- [X] Basic image I/O
- [X] 2d/3d Math 
- [X] Window management abstraction
- [ ] Mouse and keyboard input abstraction
- [X] RHI abstraction layer
- [X] OpenGL 4.1+ support for Linux, Windows and macOS
- [X] Vulkan 1.1+ support for Linux, Windows and macOS
- [ ] Mesh generate/import tools
- [ ] Templates for development
- [ ] GUI Application structure
- [ ] Example applications

### Platforms

- [X] Windows 10 (tested on Windows 10 Pro 19043.1110)
- [X] macOS (tested on 10.14 Mojave)
- [X] Linux-based OS (tested on Ubuntu 20.04)

### Thirdparty projects

* [glfw](https://github.com/glfw/glfw) for cross-platform window and input management
* [glew](https://github.com/Perlmint/glew-cmake) for OpenGL functions and extensions loading
* [stb](https://github.com/nothings/stb) image utilities for images loading, writing and resizing
* [vma](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) vulkan memory allocation library
* [glslang](https://github.com/KhronosGroup/glslang) for runtime glsl to spir-v shaders compilation
* [whereami](https://github.com/gpakosz/whereami) for executable location extracting
* [yamc](https://github.com/yohhoy/yamc) for C++ locks implementation
* [portablefiledialogs](https://github.com/samhocevar/portable-file-dialogs) for native OS dialogs access
* [googletest](https://github.com/google/googletest) for Unit-testing of the engine source code modules

## Getting started

### Dependencies

**Glfw library dependencies setup for Linux-based OS.**
Glfw uses native windowing APIs and built-in OS window frameworks, 
however it may require additional setup step for linux users. 
Follow the official glfw guide to setup this dependencies by `apt-get` tool.

**To build library OpenGL-based low-level renderer** on Linux-based OS 
sometimes you need make sure, that `glew` dependencies are installed.
So follow glew installation dependencies requirements.

**To build library Vulkan-based low-level renderer** you need to download and install
official Vulkan SDK from LunarG [website](https://www.lunarg.com/vulkan-sdk/). Follow
instructions in order to install this SDK. Note, that this project uses CMake
to locate SDK location, so remember to expose environment variable `VULKAN_SDK` if it is required.

### Requirements

* CMake version 3.11 or higher
* C++ compiler 
* Git (to get source code)

### Source code

Get the source code and initialize dependencies of the project.

```shell script
$ git clone https://github.com/EgorOrachyov/Berserk.git
$ cd Berserk
```

### Build

Configure build directory and run the build process.
Pass `-j N` option in the last cmake command to issue build on N threads.

```shell script
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . -j 4
```

### Run

Inside build directory exec the following command to run core unit-tests.

```shell script
$ bash Scripts/run_tests.sh
```

## Directory structure
 
```ignorelang
Berserk
├── Code - Engine source code and dependencies
│   ├── Editor - engine editor source code
│   ├── Plugins - engine optional modules
│   └── Runtime - runtime engine infrastructure
│       ├── BerserkCore - primary core module
│       ├── BerserkRHI - rendering hardware interface
│       ├── BerserkVulkan - rhi implementation backend for Vulkan rendering 
│       ├── BerserkOpenGL - rhi implementation backend for OpenGL rendering
│       └── BerserkPlatform - platform specific code
├── Docs - documents, text files and various helpful stuff
├── Engine - engine config, textures, shaders, resources and etc.
├── Ide - docs and templates for development and ide setup
├── Tests - tests for engine modules
├── Thirdparty - project dependencies
│   ├── glew - for OpenGL functions and extensions loading
│   ├── glfw - for cross-platform window and input management
│   ├── glslang - for runtime glsl to spir-v shaders compilation
│   ├── googletest - for Unit-testing of the engine source code modules
│   ├── portablefiledialogs - for native OS dialogs access
│   ├── stbimage - image utilities for images loading, writing and resizing
│   ├── vma - vulkan memory allocation library
│   ├── whereami - for executable location extracting
│   └── yamc - for C++ locks implementation
└── CMakeLists.txt - project cmake config
```

## License

This project licensed under MIT license. License text cam be found in 
[license file](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md).

## Also

If you have any questions, ideas, how to improve/implement something, or you want to 
contribute to the project, please, feel free to contact me at egororachyov@gmail.com.
Also, you can create an issue or pull request in the repository.