#version 410

in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D Depth;

void main()
{
    float value = texture(Depth, TexCoords).r;
    FragColor = vec4(vec3(value), 1.0);
}
