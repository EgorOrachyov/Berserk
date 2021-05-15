![Project logo](https://github.com/EgorOrachyov/Berserk/blob/master/Docs/Images/logo-main.png)

# BERSERK

[![License](https://img.shields.io/badge/license-MIT-orange)](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md)

**Berserk engine** is designed as a powerful and compact tool for creating high-performance applications.
It will be written in C++11 with support for OpenGL graphics, OpenAL audio, Bullet Physics, 
Assimp importer for models, FreeImage texture loader and FreeType font loader.

**The engine will provide** functionality in memory management, math, SIMD instructions set utilisation, 
working with strings, multi-threading, exporting/importing various kinds of resources (such as textures, 
geometry, audio files and etc.), displaying fonts and texts, and 3d graphics.

**The idea** behind this hobby project is to implement a simple, compact, friendly, and at the same 
time powerful enough engine in order to create a 3d third-person platformer game, which is fully 
featured with modern graphics, visual effects, audio, physics, and scripting features. Since I am 
a big fan of 3d-graphics, games, and programming, this project will allow me to practice my skills 
as well as have a lot of fun with fancy programming tasks and design decisions. 

> **At this moment** the primary goal is to create core structure with an usable stuff,
> which will be expanded later by the engine modules/components. 
> Project under heavy development. 

### Features

- [X] Platform basic abstraction
- [X] Templates, containers and custom strings
- [X] Logging and basic debug features
- [X] File I/O abstraction
- [X] Basic image I/O
- [X] 2d/3d Math 
- [X] Window management abstraction
- [ ] Mouse and keyboard input abstraction
- [ ] RHI abstraction layer
- [ ] OpenGL 4.1+ support for Linux, Windows and macOS
- [ ] Vulkan 1.0 support for Linux, Windows and macOS
- [ ] Templates for development
- [ ] GUI Application structure

### Platforms

- [X] Linux based OS (tested on Ubuntu 20.04)
- [X] macOS (tested on Mojave)
- [ ] Windows 10 

### Thirdparty projects

* [glfw](https://github.com/glfw/glfw) for cross-platform window and input management
* [glew](https://github.com/Perlmint/glew-cmake) for OpenGL functions and extensions loading
* [stb](https://github.com/nothings/stb) Image utilities for images loading, writing and resizing
* [whereami](https://github.com/gpakosz/whereami) for executable location extracting
* [yamc](https://github.com/yohhoy/yamc) for C++ locks implementation
* [tinyfiledialogs](https://github.com/native-toolkit/tinyfiledialogs) for native OS file dialogs access
* [googletest](https://github.com/google/googletest) for Unit-testing of the engine source code modules

## Getting started

### Requirements

* CMake version 3.11 or higher
* C++ compiler 
* Git (to get source code)

### Dependencies

The project uses cross-platform windowing and input management library glfw. 
This library uses native windowing APIs and built-in OS window frameworks, 
however it may require additional setup step for linux users. 
Follow the official glfw guide to setup this dependencies by `apt-get` tool.

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
│   ├── Core - primary core module
│   ├── Glfw - wrapper for glfw window management
│   ├── Unix - system features wrapper for *nix based OS
│   ├── RHI - rendering hardware interface
│   ├── OpenGL - rhi implementation backend for OpenGL rendering 
│   └── Thirdparty - project dependencies
│       ├── glew - for OpenGL functions and extensions loading
│       ├── glfw - for cross-platform window and input management
│       ├── googletest - for Unit-testing of the engine source code modules
│       ├── stbimage - image utilities for images loading, writing and resizing
│       ├── tinyfiledialogs - for native OS file dialogs access
│       ├── whereami - for executable location extracting
│       └── yamc - for C++ locks implementation
├── Docs - documents, text files and various helpful stuff
├── Engine - engine config, textures, shaders, resources and etc.
├── Ide - docs and templates for development and ide setup
├── Tests - tests for engine modules
└── CMakeLists.txt - project cmake config
```

## License

This project is licensed under MIT license. License text could be found in 
[license file](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md).

## Also

If you have any questions, ideas, how to improve/implement something, or you want to 
contribute to the project, please, feel free to contact me at egororachyov@gmail.com.
Also you can create an issue or pull request in the repository.