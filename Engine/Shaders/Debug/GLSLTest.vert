#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

out VS_OUT
{
    vec4 WorldPosition;
    vec4 WorldNormal;
    vec2 WorldTexCoords;
}
vs_out;

uniform mat4 ModelView;
uniform mat4 MVP;

void main()
{
    vs_out.WorldPosition = ModelView * vec4(VertexPosition.xyz, 1.0);
    vs_out.WorldNormal = ModelView * vec4(VertexNormal.xyz, 0.0);
    vs_out.WorldTexCoords = VertexTexCoords;

    gl_Position = MVP * vec4(VertexPosition.xyz, 1.0);
}
