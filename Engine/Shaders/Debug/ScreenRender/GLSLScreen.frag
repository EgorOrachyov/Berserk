#version 410 core

layout (location = 0) out vec4 FragColor;

in VS_OUT
{
    vec2 WorldTexCoords;
}
fs_in;

uniform sampler2D Texture0;

void main()
{
    FragColor = vec4(texture(Texture0, fs_in.WorldTexCoords).rgb * vec3(0.1, 0.7, 0.99), 1.0);
}
