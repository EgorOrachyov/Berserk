# Render System

Graphics core and the most powerful and important part of the engine. 
It provides features for rendering objects and applying effects (post process effects) 
in soft real time mode. Basically, it is created on top of the OpenGL graphics driver, 
however, it could be expanded on the other drivers and libraries (DirectX, Vulkan, ...). 
 
## Pipeline stages 
 
Rendering for each frame is explicitly divided in the separate stages for increasing
performance and the ability to configure rendering process of the engine. Rendering on GPU
is performed in and only in Main execution thread of the application. The following stages
are implemented in the Rendering engine (should be):

* Reflection maps generation pass 
* Shadow maps generation pass
* GBuffer generation pass
* Screen space ambient occlusion pass
* Volumetric lightning pass 
* Deferred lightning pass
* Forward rendering pass
* Alpha blending pass
* Overlay pass
* Depth disabled rendering pass
* Gamma correction pass 
 
## Rendering Queue
 
Rendering queue provides model (mesh composed) submitting for rendering in different 
pipeline and shader modes:
 
* Background - Objects rendered before any geometry on the scene. After that stage the depth buffer will be cleared.
* Geometry - Main geometry pass - models, sorted in the order of used materials.
* Alpha blend - Geometry with materials that rendering involves alpha blending with previous color data in the currently rendered buffer. 
* Overlay - Everything what should be rendered on top the result image with cleared depth buffer. It could be camera effects or UI elements.
 
## Shader Core

### Shader xml 

For importing shader programs into the rendering system uses XML meta-info files of a 
special format that provide additional and common information about:

* Shader program name
* Shader files, used by program, and their types
* Paths to the files with shader source code
* Names of the uniform variables used in this shader program
* Uniform blocks used in this shader and variables names, defined it these uniform blocks

### Shader xml specification

XML file with predefined nodes and attributes value is commonly used for specifying shader program
data needed for interconnection between CPU and GPU in time of rendering. 

```
<!-- Specify one (or more) program(s) properties for loading     -->
<!-- Name attribute allows to find program in runtime via string -->
<program name="Shadow map generation">
    
    <!-- Shader type for compilation               -->
    <!-- Relative or full path to the file on disk -->
    <shader type="Geometry">
        <path> "../Shaders/OpenGL/ShadowMap.geom" </path>
    </shader>
    <shader type="Vertex">
        <path> "../Shaders/OpenGL/ShadowMap.vert" </path>
    </shader>
    <shader type="Fragment">
        <path> "../Shaders/OpenGL/ShadowMap.frag" </path>
    </shader>

    <!-- Specify uniform variables used in the shader -->
    <!-- Count - explicitly shows number of uniforms  -->
    <uniform count="4">
        <variable> "SystemModel"   </variable>
        <variable> "LigthView"     </variable>
        <variable> "LigthPosition" </variable>
        <variable> "LightFarPlane" </variable>
    </uniform>
    
</program>
```

### Shader input registers

| Name                   | Type        | Location | Description                                 |
|------------------------|-------------|----------|---------------------------------------------|
| Group: Vertex
| VertexPosition         | vec3        | 0        | Position of the mesh vertex in the mesh world space |
| VertexNormal           | vec3        | 1        | Mesh vertex normal vector |
| VertexTangent          | vec3        | 2        | Mesh vertex tangent vector |
| VertexBitangent        | vec3        | 3        | Mesh vertex bitangent vector (could be calculated manually via n and t vectors) |
| VertexTexCoords        | vec2        | 4        | Mesh vertex texture coordinates for specified material (or not) |

### Shader uniform variables

Info about uniform shader variables with associated fixed types and brief description.
Note, that this names and types are defined in assumption that the render uses OpenGL Shading Language.
However, this information could be explicitly expanded for other shading languages.  
This uniform variables could be accessed from engine defined or user defined shaders for
specific rendering stages. Uniform variables of the concrete shader program should be defined explicitly
is special meta-info xml file. Format of that file will be specified later.

| Name                        | Type        | Description                                       |
|-----------------------------|-------------|---------------------------------------------------|
| Group: System |
| SystemModel                 | mat4        | Transformation of the rendered mesh |
| SystemView                  | mat4        | View (or camera) transformation |
| SystemProjection            | mat4        | Perspective or orthographic projection matrix |
| SystemModelView             | mat4        | View * Model matrix|
| SystemMVP                   | mat4        | Projection * Model * View matrix |
| Group: Camera |
| CameraPosition              | vec4        | Camera world space position |
| CameraDirection             | vec4        | Camera world space normalized direction |
| CameraUp                    | vec4        | Camera normalized up vector |
| Group: Deferred |
| DeferredPosition            | vec3        | Position of the pixel fragment in the world space |
| DeferredNormal              | vec3        | Normal of the pixel fragment in the world space |
| DeferredDiffuse             | vec3        | Diffuse color of the pixel fragment |
| DeferredSpecular            | vec4        | Specular color of the pixel fragment |
| Group: Map |
| DiffuseMap                  | sampler2D   | Diffuse color of the material |
| NormalMap                   | sampler2D   | Normal map of the material |
| BumpMap                     | sampler2D   | Info for parallax occlusion mapping for the material surface |
| SpecularMap                 | sampler2D   | Specular map of the material |               
| AmbientMap                  | sampler2D   | Large scale occluded light surface info |
| ReflectionMap               | sampler3D   | Reflection map for the material at the point of view of object |
| Group: Shadow Map |
| LigthView                   | mat4[6]     | Point light axis aligned view matrices |
| LigthPosition               | vec4        | Light position in the world space |
| LightFarPlane               | float       | Far cut plane for omnidirectional map generation |

Table content is not exhaustive and it will be updated and filled later.
