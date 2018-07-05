#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 RotationMatrix;

uniform Light
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shinines;
};

void main()
{
    Color = Ka * Kd * Ks * VertexColor * shinines;
    gl_Position = RotationMatrix * vec4(VertexPosition, 1.0);
}