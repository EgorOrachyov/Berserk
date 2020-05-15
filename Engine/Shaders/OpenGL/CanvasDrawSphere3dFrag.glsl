#version 410 core

layout (location = 0) out vec4 outColor;

flat in vec3 fColor;

void main()
{
    outColor = vec4(fColor, 1.0f);
}