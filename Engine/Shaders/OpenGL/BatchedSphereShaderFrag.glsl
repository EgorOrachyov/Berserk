#version 410 core

layout (location = 0) out vec4 outColor;

in vec3 fsColor;

void main() {
    // Gamma correct color (linear to srgb space)
    vec3 color = pow(fsColor, vec3(1.0f / 2.2f));
    outColor = vec4(color, 1.0f);

}