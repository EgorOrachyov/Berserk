# Render System

Graphics core and the most powerful and important part of the engine. It provides features for rendering objects and applying effects (post process effects) in soft real time mode. Basically, it is created on top of the OpenGL graphics driver, however, it could be expanded on the other drivers and libraries (DirectX, Vulkan, ...). 
 
## Shaders

## Shaders uniform variables

Name for uniform shader variables with associated fixed types

| Variable                    | Type        | Description                                       |
|-----------------------------|-------------|---------------------------------------------------|
| System 																						|
| SystemModel 				  | mat4		|													|
| SystemView 			 	  | mat4		|													|
| SystemProjection            | mat4		|													|
| SystemModelView             | mat4		|													|
| SystemMVP                   | mat4		|													|
| Camera  																						|
| CameraPosition              | vec4		|													|
| CameraDirection             | vec4		|													|
| CameraUp                    | vec4		|													|
| Deferred 																						|
| DeferredPosition            | vec3		|													|
| DeferredNormal              | vec3		|													|
| DeferredDiffuse             | vec3		|													|
| DefferedAlbedo              | vec4		|													|

Table content is not exhaustive and it will be updated and filled later.