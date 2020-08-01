#version 410 core
#include "TestFragmentInclude.glsl"

layout (location = 0) out vec4 outColor;

in vec2 fsColor;

void main() {
    outColor = getColor(vec3(fsColor,0.0f));
}