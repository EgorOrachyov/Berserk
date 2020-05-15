#version 410 core
layout (location = 0) out vec4 fColor;

in vec3 fNormal;

void main()
{
   fColor = vec4(fNormal * vec3(0.5f) + vec3(0.5f),1.0f);
}