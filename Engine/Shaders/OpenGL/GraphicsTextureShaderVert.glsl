#version 410 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;
//layout (location = 2) in vec4 inColor;

//out vec4 fsColor;
out vec2 fsTexCoords;

layout (std140) uniform Transform {
    mat4 Proj;
};

void main() {
    //fsColor = inColor;
    fsTexCoords = inTexCoords;
    gl_Position = Proj * vec4(inPos, 1.0f);
}
