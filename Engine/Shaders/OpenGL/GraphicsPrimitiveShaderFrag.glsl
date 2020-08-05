#version 410 core

layout (location = 0) out vec4 outColor;

in vec4 fsColor;

void main() {
    outColor = fsColor;
}