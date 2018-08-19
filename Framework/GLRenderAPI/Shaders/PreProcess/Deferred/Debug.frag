#version 410 core

in vec2 TexCoords;
uniform sampler2D Screen;

layout(location = 0) out vec4 FragColor;

void main()
{
    vec3 mapped = texture(Screen, TexCoords).rgb;
    //mapped = normalize(mapped);
    //mapped = mapped * 0.5 + 0.5;
    FragColor = vec4(mapped, 1.0);
    //float a = texture(Screen, TexCoords).a;
    //FragColor = vec4(vec3(a), 1.0);
    //FragColor = vec4(vec3(0.85), 1.0);
}