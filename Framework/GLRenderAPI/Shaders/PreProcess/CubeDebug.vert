#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 FragPos;
out vec3 FragNorm;

uniform mat4 Model;
uniform mat4 MVP;

void main()
{
    FragPos = vec3(Model * vec4(VertexPosition,1));
    FragNorm = vec3(Model * vec4(VertexNormal, 0));
    gl_Position = MVP * vec4(VertexPosition, 1);
}
