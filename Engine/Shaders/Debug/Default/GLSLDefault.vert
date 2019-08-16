#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoords;

out vec2 FragTexCoords;

layout (std140) uniform Transform
{
    mat4 Model;
};

void main()
{
    FragTexCoords = VertexTexCoords;
    gl_Position = Model * vec4(VertexPosition, 1.0);
}