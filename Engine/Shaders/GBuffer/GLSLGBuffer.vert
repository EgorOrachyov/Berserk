//
// Created by Egor Orachyov on 09.04.2019.
//

#version 410 core

// All the vertex input data. If concrete vertex set
// does not have defined input variable, it won't be used in the shader

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTangent;
layout (location = 3) in vec3 VertexBitangent;
layout (location = 4) in vec2 VertexTexCoords;

out VS_OUT
{
    vec3 WorldPosition;
    vec3 ViewPosition;
    vec3 WorldNormal;
    vec2 TextureCoords;
    mat3 TBN;
}
vs_out;


layout (std140) uniform SystemVert
{
    mat4 Model;                                 // Current model transformation matrix
    mat4 ViewModel;                             // View * Model
    mat4 PVM;                                   // Projection * View * Model
}
System;

layout (std140) uniform CameraVert
{
    vec4 Up;                                    // World space Up vector
    vec4 Direction;                             // World space direction vector
    vec4 Position;                              // World space position
    mat4 View;                                  // Game view matrix (camera view)
    mat4 Projection;                            // Perspective or orthographic matrix for projection
}
Camera;

subroutine void VERTEX_PASS_TYPE();
subroutine uniform VERTEX_PASS_TYPE VERTEX_PASS;

subroutine(RenderPassType)
void VERTEX_PASS_P()
{
    /* Nothing */
}

subroutine(RenderPassType)
void VERTEX_PASS_PN()
{
    vs_out.FragNorm = vec3(Model * vec4(VertexNormal, 0.0));
}

subroutine(RenderPassType)
void VERTEX_PASS_PNT()
{
    vs_out.FragNorm = vec3(Model * vec4(VertexNormal, 0.0));
    vs_out.TextureCoords = VertexTexCoords;
}

subroutine(RenderPassType)
void VERTEX_PASS_PNTBT()
{
    vec3 T = normalize(vec3(Model * vec4(VertexTangent, 0.0)));
    vec3 B = normalize(vec3(Model * vec4(VertexBitangent, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(VertexNormal, 0.0)));

    vs_out.TBN = mat3(T,B,N);
    vs_out.TextureCoords = VertexTexCoords;
}

void main()
{
    VertPass();

    vs_out.WorldPosition = vec3(System.Model * vec4(VertexPosition, 1.0));
    vs_out.ViewPosition  = vec3(System.ViewModel * VertexPosition);
	gl_Position          = System.PVM * vec4(VertexPosition, 1.0);
}