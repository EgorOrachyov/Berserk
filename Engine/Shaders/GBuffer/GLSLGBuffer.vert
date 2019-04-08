//
// Created by Egor Orachyov on 09.04.2019.
//

#version 410 core

// All the vertex input data. If concrete vertex set
// does not have defined input variable, it won't be used in the shader

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexBitangent;
layout (location = 3) in vec3 VertexTangent;
layout (location = 4) in vec2 VertexTexCoords;

//
//

out VS_OUT
{
    vec3 WorldPosition;
    vec3 ViewPosition;
    vec3 WorldNormal;
    vec2 TextureCoords;
    mat3 TBN;
}
vs_out;

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

//
//

subroutine void VERTEX_PASS_TYPE();
subroutine uniform VERTEX_PASS_TYPE VERTEX_PASS;

void main()
{
	gl_Position = vec4(vec3(0.0), 1.0);
}
