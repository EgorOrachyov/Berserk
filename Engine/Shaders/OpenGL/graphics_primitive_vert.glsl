#version 410 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;

out vec4 fsColor;

layout (std140) uniform Transform {
    mat4 proj;
    ivec2 areaSize;
};

void main() {
    fsColor = inColor;
    gl_Position = proj * vec4(inPos.x, areaSize.y - inPos.y, inPos.z, 1.0f);
}