#version 410

in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D HDRTex;
uniform float Exposure = 5.0;
uniform float Gamma = 0.67;

void main()
{
    vec3 hdrColor = texture(HDRTex,TexCoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * Exposure);
    mapped = pow(mapped, vec3(Gamma));

    FragColor = vec4(mapped, 1.0);
}