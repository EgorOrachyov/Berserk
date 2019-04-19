#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoords;

out VS_OUT
{
    vec2 ScreenTexCoords;
}
vs_out;

uniform mat4 Projection;

void main()
{
    vs_out.ScreenTexCoords = VertexTexCoords;
    gl_Position = Projection * vec4(VertexPosition.xyz, 1.0);
}