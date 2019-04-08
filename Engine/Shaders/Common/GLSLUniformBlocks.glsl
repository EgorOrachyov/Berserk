//
// Created by Egor Orachyov on 09.04.2019.
//

#version 410 core

// Default cross-shader uniform block which contains
// current mesh trasformations with needed matricies

layout (std140) uniform System
{
    mat4 Proj;
    mat4 View;
    mat4 Model;
    mat4 ViewModel;
    mat4 PVM;
}
System;

// Camera info unifirm block - scene info of active camera
// (could be used for shadow mapping purposes)

layout (std140) uniform Camera
{
    vec4 Up;
    vec4 Direction;
    vec4 Position;
    mat4 View;
    mat4 Proj;
}
Camera;

