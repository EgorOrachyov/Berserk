#version 410 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 fsTexCoords;

layout (std140) uniform Transform {
    mat4 proj;
};

void main() {
    //fsColor = inColor;
    fsTexCoords = inTexCoords;
    gl_Position = proj * vec4(inPos.xy, 0.0f, 1.0f);
}
