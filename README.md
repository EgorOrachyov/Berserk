![Project logo](https://github.com/EgorOrachyov/Berserk/blob/master/docs/images/logo-main.png)

# BERSERK

[![build](https://github.com/EgorOrachyov/Berserk/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/build.yml)
[![clang format](https://github.com/EgorOrachyov/Berserk/actions/workflows/clang-format.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/clang-format.yml)
[![pages](https://github.com/EgorOrachyov/Berserk/actions/workflows/pages.yml/badge.svg?branch=master)](https://github.com/EgorOrachyov/Berserk/actions/workflows/pages.yml)
![codacy badge](https://app.codacy.com/project/badge/Grade/674eff47dbfa45e38c5fd3765f3256ba)
[![license](https://img.shields.io/badge/license-MIT-orange)](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md)

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

### Platforms

- Windows 10 (tested on Windows 10)
- macOS (tested on 10.14 Mojave)
- Linux-based OS (tested on Ubuntu 20.04)

### Third-party projects

* [glew](https://github.com/Perlmint/glew-cmake) for OpenGL functions and extensions loading
* [glfw](https://github.com/glfw/glfw) for cross-platform window and input management
* [glslang](https://github.com/KhronosGroup/glslang) for runtime glsl to spir-v shaders compilation
* [googletest](https://github.com/google/googletest) for Unit-testing of the engine source code modules
* [portablefiledialogs](https://github.com/samhocevar/portable-file-dialogs) for native OS dialogs access
* [stbimage](https://github.com/nothings/stb) image utilities for images loading, writing and resizing
* [vma](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) vulkan memory allocation library
* [whereami](https://github.com/gpakosz/whereami) for executable location extracting
* [yamc](https://github.com/yohhoy/yamc) for C++ locks implementation

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

* Common:
    * Git (to get source code)
    * CMake (the latest version)
    * Ninja (as build files generator)
    * Python 3.7+
* Windows 10:
    * Microsoft Visual C++ Compiler (MSVC) with C++11 support
    * x64 Native Tools Command Prompt for VS
* Ubuntu 20.04:
    * GNU C++ Compiler with C++11 support
* MaсOS Mojave 10.14:
    * Clang Compiler with C++11 support

### Get source code

The following code snippet downloads project source code repository, enters project root folder
and runs submodules init in order to get dependencies source code initialized.
Must be executed from the folder where you want to locate project.

```shell
$ git clone https://github.com/EgorOrachyov/Berserk.git
$ cd Berserk
```

### Configure and run build

The following code snippet runs cmake build configuration process
with output into `build` directory, in `Release` mode with tests `BERSERK_BUILD_TESTS=ON` enabled.
Then runs build process for `build` directory in verbose mode with `-j 4` four system threads.
Must be executed from project root folder.

```shell
$ cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBERSERK_BUILD_TESTS=ON
$ cmake --build build --target all --verbose -j 4
```

On macOS, you can optionally specify target binaries architecture to build.
Pass option `-DCMAKE_OSX_ARCHITECTURES` with `x86_64` or `arm64` respectively.
By default, build falls back to `CMAKE_SYSTEM_PROCESSOR` specified architecture.
See example bellow, replace `<arch>` with desired architecture for your build.

```shell
$ cmake . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBERSERK_BUILD_TESTS=ON -DCMAKE_OSX_ARCHITECTURES=<arch>
$ cmake --build build --target all --verbose -j 4
```

### Run

The following code snippet executed python script, which allows to
run all native C++ library unit-tests, located in build directory,
specified in `--build-dir=build`. Must be executed from project root folder.

```shell
python ./scripts/run_tests.py --build-dir=build
```

## Directory structure
 
```ignorelang
Berserk
├── code - Engine source code and dependencies
│   └── runtime - runtime engine infrastructure
│       ├── core - primary core module
│       ├── platform - platform specific code
│       ├── rhi - rendering hardware interface
│       ├── vk - rhi implementation backend for Vulkan rendering 
│       └── opengl - rhi implementation backend for OpenGL rendering
├── engine - engine config, textures, shaders, resources and etc.
├── tests - tests for engine modules
├── templates - game project templates for engine
├── scripts - script utilities
├── docs - documents, text files and various helpful stuff
├── deps - project dependencies
│   ├── glew - for OpenGL functions and extensions loading
│   ├── glfw - for cross-platform window and input management
│   ├── glslang - for runtime glsl to spir-v shaders compilation
│   ├── googletest - for Unit-testing of the engine source code modules
│   ├── pfd - portable file dialogs for native OS dialogs access
│   ├── stbimage - image utilities for images loading, writing and resizing
│   ├── vma - vulkan memory allocation library
│   ├── whereami - for executable location extracting
│   └── yamc - for C++ locks implementation
└── CMakeLists.txt - project cmake config
```

## License

This project licensed under MIT license. License text can be found in 
[license file](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md).

## Also

If you have any questions, ideas, how to improve/implement something, or you want to 
contribute to the project, please, feel free to contact me at egororachyov@gmail.com.
Also, you can create an issue or pull request in the repository.