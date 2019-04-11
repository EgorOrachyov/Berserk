//
// Created by Egor Orachyov on 09.04.2019.
//

#version 410 core

// Name:
// Binding point[s]:
// Description:

// Name: System
// Binding point[s]: 1
// Description: Default cross-shader uniform block which contains 
//              current mesh trasformations with needed matricies
layout (std140) uniform System[Vert|Frag]
{
    mat4 Model;                                 // Current model transformation matrix
    mat4 ViewModel;                             // View * Model    
    mat4 PVM;                                   // Projection * View * Model
}
System;

// Name: Camera
// Binding point[s]: 2
// Description: scene active camera info 
//              (could be used for shadow mapping purposes)
layout (std140) uniform Camera[Vert|Frag]
{
    vec4 Up;                                    // World space Up vector
    vec4 Direction;                             // World space direction vector
    vec4 Position;                              // World space position
    mat4 View;                                  // Game view matrix (camera view)
    mat4 Projection;                            // Perspective or orthographic matrix for projection
}
Camera;

// Name: SpotLightSource
// Binding point[s]: 5
// Description: Arrays with data about active spot light sources in the render pass
layout (std140) uniform SpotLightSource[Vert|Frag]
{
    vec4 Position[];                            // World space position
    vec4 Direction[];                           // World space direction
    vec4 Intensity[];                           // Light source color
    vec4 InnerOuterCutoff[];                    // Inner and outer cutodd for soft edges of the light
    vec4 Distance_Epsilon_Flags_Sampler[];      // Packed: Distance of action, epsilon for soft shadows, flags and shadow map sampler id
}
SpotLight;

// Name: DirectionalLightSource
// Binding point[s]: 7
// Description: Arrays with data about active directional light sources in the render pass
layout (std140) uniform DirectionalLightSource[Vert|Frag]
{
    vec4 Direction[];                           // World space direction
    vec4 Intensity[];                           // Light source color
    vec4 Flags_Sampler[];                       // Packed: flags and shadow map sampler id
}
DirectionalLight;
