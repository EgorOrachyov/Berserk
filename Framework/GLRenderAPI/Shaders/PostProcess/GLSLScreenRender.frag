#version 410

in vec2 TexCoords;
uniform sampler2d Screen;

layout (loaction = 0) vec4 FragColor;

void main()
{
    mapped = texture(Screen, TexCoords).rgb;

    FragColor = vec4(mapped, 1.0);
}