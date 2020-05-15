#version 410 core

layout (location = 0) in vec3 vPositon;
layout (location = 1) in vec4 vCenterRadius;
layout (location = 2) in vec3 vColor;

flat out vec3 fColor;

layout (std140) uniform Transform
{
    mat4 ProjView;
    mat4 OrthoView;
};

void main() {
    fColor = vColor;
    float radius = vCenterRadius.w;
    vec3 center = vCenterRadius.xyz;
    vec3 position = center + radius * vPositon;
    gl_Position = ProjView * vec4(position, 1.0f);
}


