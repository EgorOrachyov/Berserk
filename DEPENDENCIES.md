# Project dependencies and third-party libraries 

## Dependencies required for build

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

## Third-party projects

### [glfw](https://github.com/glfw/glfw) 
For cross-platform window and input management.
Licensed under zlib License.

### [glew](https://github.com/Perlmint/glew-cmake) 
For OpenGL functions and extensions loading.
Licensed under custom License.

### [stb](https://github.com/nothings/stb) 
Image utilities for images loading, writing and resizing.
Licensed under MIT and Public Domain License.

### [vma](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) 
Vulkan memory allocation library.
Licensed under MIT License.

### [glslang](https://github.com/KhronosGroup/glslang) 
For runtime glsl to spir-v shaders compilation.
Licensed under  License.

### [whereami](https://github.com/gpakosz/whereami) 
For executable location extracting.
Licensed under MIT adn WTFPLv2 License.

### [yamc](https://github.com/yohhoy/yamc) 
For C++ locks implementation.
Licensed under MIT License.

### [portablefiledialogs](https://github.com/samhocevar/portable-file-dialogs) 
For native OS dialogs access.
Licensed under WTFPL License.

### [googletest](https://github.com/google/googletest) 
For Unit-testing of the engine source code modules.
Licensed under BSD 3-Clause License.