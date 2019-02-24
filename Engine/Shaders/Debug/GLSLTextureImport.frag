#version 410 core

in vec2 TexCoords;
uniform sampler2D screen;

layout(location = 0) out vec4 FragColor;

void main()
{
	vec3 mapped = texture(screen, TexCoords).rgb;
    FragColor = vec4(mapped, 1.0);
}
