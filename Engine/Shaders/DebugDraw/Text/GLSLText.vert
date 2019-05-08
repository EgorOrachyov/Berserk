#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoords;

out vec2 ScreenTexCoords;

// Orthographic projection, with the aspect of the main display
uniform mat4 Projection;

void main()
{
    ScreenTexCoords = VertexTexCoords;
    gl_Position = Projection * vec4(VertexPosition.xyz, 1.0);
}