#version 410 core

layout (location = 0) out vec4 outColor;

in vec3 fsColor;

void main() {
    // Out linear color (gamma correction is done later)
    outColor = vec4(fsColor, 1.0f);

}