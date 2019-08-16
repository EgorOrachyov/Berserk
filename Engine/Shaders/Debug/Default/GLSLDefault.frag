#version 410 core

layout (location = 0) out vec4 FragColor;

in vec2 FragTexCoords;

uniform sampler2D Texture0;

void main()
{
    float alpha = texture(Texture0, FragTexCoords).r;
    FragColor = vec4(vec3(1,0,0), alpha);
}