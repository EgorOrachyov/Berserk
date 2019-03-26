#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoords;

out VS_OUT
{
    vec2 WorldTexCoords;
}
vs_out;

void main()
{
    vs_out.WorldTexCoords = VertexTexCoords;
    gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);
}
