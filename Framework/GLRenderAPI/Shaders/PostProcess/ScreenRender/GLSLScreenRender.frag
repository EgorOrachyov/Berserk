#version 410 core

in vec2 TexCoords;
uniform sampler2D Screen;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec3 mapped = texture(Screen, TexCoords).rgb;

    FragColor = vec4(mapped, 1.0);
}