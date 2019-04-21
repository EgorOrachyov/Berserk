#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoords;

out vec2 ScreenTexCoords;

void main()
{
    ScreenTexCoords = VertexTexCoords;
    gl_Position = vec4(VertexPosition, 1.0);
}
