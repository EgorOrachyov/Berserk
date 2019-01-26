#version 410

in vec3 Color;

out vec4 FragColor;

uniform mat4 RotationMatrix;

void main()
{
    FragColor = RotationMatrix * vec4(Color, 1.0);
}