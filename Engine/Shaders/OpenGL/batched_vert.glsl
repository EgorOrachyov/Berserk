#version 410 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec3 fsColor;

layout (std140) uniform Transform {
    mat4 projView;
};

void main() {
    fsColor = inColor;
    gl_Position = projView * vec4(inPos, 1.0f);
}