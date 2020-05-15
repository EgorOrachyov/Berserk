#version 410 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fNormal;

layout (std140) uniform Transform
{
   mat4 Proj;
   mat4 View;
   mat4 Model;
};

void main()
{
   fNormal = vec3(Model * vec4(vNormal,0.0f));
   gl_Position = Proj * View * Model * vec4(vPosition, 1.0f);
}