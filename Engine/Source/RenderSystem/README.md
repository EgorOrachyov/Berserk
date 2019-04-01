# Render System

Graphics core and the most powerful and important part of the engine. 
It provides features for rendering objects and applying effects (post process effects) 
in soft real time mode. Basically, it is created on top of the OpenGL graphics driver, 
however, it could be expanded on the other drivers and libraries (DirectX, VulkanAPI). 
 
## Pipeline stages 
 
Rendering for each frame is explicitly divided in the separate stages for increasing
performance and the ability to configure rendering process of the engine. Rendering on GPU
is performed in and only in Main execution thread of the application. The following stages
are implemented in the Rendering engine (should be):

* Reflection maps generation  
* Shadow maps generation 
* GBuffer generation 
* Screen space ambient occlusion 
* Volumetric lightning  
* Deferred lightning 
* Forward rendering 
* Alpha blending 
* Overlay 
* Gamma correction  
 
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
* Uniform blocks used in this shader and variables names, defined in these uniform blocks

### Shader xml specification

XML file with predefined nodes and attributes values is commonly used for specifying shader program
data needed for interconnection between CPU and GPU in time of rendering. 

```
<?xml version="1.0" encoding="us-ascii"?>

<!-- Required block -->
<!-- Specify one (or more) program(s) properties for loading     -->
<!-- Name attribute allows to find program in runtime via string -->

<program name="Shadow map generation">
    
    <!-- Required block -->
    <!-- Specify paths, uniforms and etc. for chosen -->
    <!-- graphics driver. Loader will manually check -->
    <!-- needed one and load appropriate shaders     -->
    
    <driver name="OpenGL">
    
        <!-- Required block -->
        <!-- Specify shader type for code compilation and linking -->
        <!-- and relative or full path to the file on disk via    -->
        <!-- {SHADERS} - variable                                 -->

        <shaders>
            <shader type="GEOMETRY" path="{SHADERS}/ShadowMap/ShadowMap.geom"/>
            <shader type="VERTEX"   path="{SHADERS}/ShadowMap/ShadowMap.vert"/>
            <shader type="FRAGMENT" path="{SHADERS}/ShadowMap/ShadowMap.frag"/>        
        </shaders>
    
        <!-- Required block -->
        <!-- Specify uniform variables used in the shader -->
    
        <uniforms>
            <uniform name="SystemModel"/>
            <uniform name="LigthView"/>
            <uniform name="LigthPosition"/>
            <uniform name="LightFarPlane"/>
        </uniforms>
        
        <!-- Required block -->
        <!-- Specify uniform blocks, evolved in shader program.   -->
        <!-- Name will be used to find block index, binding point -->
        <!-- specifies the uniform buffers and shader uniform     -->
        <!-- blocks binding properties (association)              -->
    
        <uniformblocks>
            <uniformblock name="UniformBlock1" binding="0"/>
            <uniformblock name="UniformBlock2" binding="1"/>
            <uniformblock name="UniformBlock3" binding="2"/>
        </uniformblocks>        
        
        <!-- Optional block -->
        <!-- Subroutine allows to vary functionality, used in  -->
        <!-- shaders via binding functions implementations to  -->
        <!-- specified function pointer, called subroutine     -->
        
        <subroutine name="LightningPass" type="FRAGMENT">
            <function name="Phong"/>           
            <function name="PBR"/>           
            <function name="Wireframe"/>           
        </subroutine>
        
    </driver>    
</program>
```

> Note: shader system can import an arbitrary shader program in runtime mode only
> if programmer specify needed xml meta-info file. Therefore, every GPU program should be
> shipped with this kind of file. Importer does checks whether specified data suitable for
> loaded shader program. If it has some kind of mistake: loader will raise FATAL error.  

### Shader input registers

| Name                   | Type        | Location | Description                                 |
| ---------------------- | ----------- | -------- | ------------------------------------------- |
| Group: Input | | | |
| VertexPosition         | vec3        | 0        | Position of the mesh vertex in the mesh world space |
| VertexNormal           | vec3        | 1        | Mesh vertex normal vector |
| VertexTangent          | vec3        | 2        | Mesh vertex tangent vector |
| VertexBitangent        | vec3        | 3        | Mesh vertex bitangent vector (could be calculated manually via n and t vectors) |
| VertexTexCoords        | vec2        | 4        | Mesh vertex texture coordinates for specified material (or not) |

### Shader output registers

| Name                   | Type        | Location | Description                                 |
| ---------------------- | ----------- | -------- | ------------------------------------------- |
| Group: Deferred | | | |
| WorldPosition          | vec3        | 0        | Fragment world space position |
| WorldNormal            | vec3        | 1        | Fragment world space normal vector |
| WorldViewPosition      | vec3        | 2        | Fragment view space position for currently used rendering camera |
| MaterialDiffuse        | vec3        | 3        | Fragment surface diffuse color |
| MaterialSpecular       | vec3        | 4        | Fragment surface specular color |
| MaterialAmbient        | vec3        | 5        | Fragment surface large scale occluded light info |

### Shader uniform variables

Info about uniform shader variables with associated fixed types and brief description.
Note, that this names and types are defined in assumption that the render uses OpenGL Shading Language.
However, this information could be explicitly expanded for other shading languages. This uniform variables 
could be accessed from engine defined or user defined shaders for specific rendering stages. 
Uniform variables of the concrete shader program should be defined explicitly
is special meta-info xml file.

| Name                        | Type        | Description                                       |
| --------------------------- | ----------- | ------------------------------------------------- |
| Group: Common | | |
| SystemModel                 | mat4        | Transformation of the rendered mesh |
| SystemView                  | mat4        | View (or camera) transformation |
| SystemProjection            | mat4        | Perspective or orthographic projection matrix |
| SystemModelView             | mat4        | View * Model matrix|
| SystemMVP                   | mat4        | Projection * Model * View matrix |
| Group: Camera | | |
| CameraPosition              | vec4        | Camera world space position |
| CameraDirection             | vec4        | Camera world space normalized direction |
| CameraUp                    | vec4        | Camera normalized up vector |
| Group: Deferred | | |
| DeferredPosition            | vec3        | Position of the pixel fragment in the world space |
| DeferredNormal              | vec3        | Normal of the pixel fragment in the world space |
| DeferredDiffuse             | vec3        | Diffuse color of the pixel fragment |
| DeferredSpecular            | vec4        | Specular color of the pixel fragment |
| Group: Map | | |
| DiffuseMap                  | sampler2D   | Diffuse color of the material |
| NormalMap                   | sampler2D   | Normal map of the material |
| BumpMap                     | sampler2D   | Info for parallax occlusion mapping for the material surface |
| SpecularMap                 | sampler2D   | Specular map of the material |               
| AmbientMap                  | sampler2D   | Large scale occluded light surface info |
| ReflectionMap               | sampler3D   | Reflection map for the material at the point of view of object |
| Group: Shadow Map | | |
| LightView                   | mat4[6]     | Point light axis aligned view matrices |
| LightPosition               | vec4        | Light position in the world space |
| LightFarPlane               | float       | Far cut plane for omnidirectional map generation |

## Material System

Material object properties defines properties of 3D/2D space geometry, its appearance in 3D 
scene in time of rendering. Allows to configure shader technique, active maps, interaction 
with lights and shadows. Default material format should be serialized / represented accordingly
to the following xml material format file: 

```
<?xml version="1.0" encoding="us-ascii"?>

<!-- Required block -->
<!-- One file may contain description for one or many materials objects.    -->
<!-- Each one should be defined with tag 'material', field name is required -->

<material name="StoneWallWet">

    <!-- Required block -->
    <!-- Define material type via its options mask (see IMaterial::MaterialType) -->

    <technique mask="1010"/>

    <!-- Optional block -->
    <!-- Define colors (default is 0.0 for all) -->

    <colors>
        <default    r="0.45" g="0.121" b="0.56" a="1.0"/>
        <emissive   r="0.12" g="1.02"  b="0.01" a="1.0"/>
        <wireframe  r="1.0"  g="0.0"   b="0.0"  a="1.0"/>
    </colors>

    <!-- Optional block -->
    <!-- Specify all the textures, attached to this material via its path, type -->
    <!-- optional flag 'use', which allows to load and attach this texture,     -->
    <!-- actually do not use in time of rendering invocation.                   -->

    <textures>
        <texture type="ALBEDO" path="{TEXTURES}/StoneWallWet/albedo.png" use="1"/>
        <texture type="NORMAL" path="{TEXTURES}/StoneWallWet/normal.png" use="1"/>
        <texture type="METALLIC" path="{TEXTURES}/StoneWallWet/metallic.png" use="1"/>
        <texture type="ROUGHNESS" path="{TEXTURES}/StoneWallWet/roughness.png" use="1"/>
        <texture type="AMBIENT" path="{TEXTURES}/StoneWallWet/ambient.png" use="0"/>
        <texture type="DISPLACEMENT" path="{TEXTURES}/StoneWallWet/displacement.png" use="0/">
    </textures>
    
</material>
```

> Table content is not exhaustive and it will be updated and filled later.
