![Project logo](https://github.com/EgorOrachyov/Berserk/blob/master/Docs/pictures/logo-main.png)

# BERSERK

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

**At this moment** the primary goal is to create core structure with an usable stuff,
which will be expanded later by the engine modules/components. 

**Note**: project under heavy development. 

## Dependencies

* GLFW for cross-platform window and input management
* GLEW for OpenGL functions and extensions loading
* STB Image utilities
* Tiny obj loader for simple geometry import
* Dirent for directory access on windows
* FreeType2 for ttf fonts import
* Whereami for executable location extracting

## License

This project is licensed under MIT license. License text could be found in 
[license file](https://github.com/EgorOrachyov/Berserk/blob/master/LICENSE.md).

## Also

If you have any questions, ideas, how to improve/implement something, or you want to 
contribute to the project, please, feel free to contact me at egororachyov@gmail.com.
Also you can create an issue or pull request in the repository.