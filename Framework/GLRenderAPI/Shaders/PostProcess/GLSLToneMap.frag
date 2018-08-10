#version 410

in vec2 TexCoords;

uniform sampler2d HDRTex;
uniform float Exposure = 5.0;
uniform float Gamma = 0.67;

layout (loaction = 0) vec4 FragColor;

void main()
{
    vec3 hdrColor = texture(HDRTex,TexCoords).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * Exposure);
    mapped = pow(mapped, vec3(Gamma));

    FragColor = vec4(mapped, 1.0);
}