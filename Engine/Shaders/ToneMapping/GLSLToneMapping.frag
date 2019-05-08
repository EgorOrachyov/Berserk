#version 410 core

in vec2 ScreenTexCoords;

layout (location = 0) out vec4 FragmentColor;

uniform sampler2D Texture0;
uniform float Exposure;
uniform float GammaCorrection;

vec3 ToneMap(vec3 color)
{
    return vec3(1.0f) - exp(-Exposure * color);
}

vec3 GammaCorrect(vec3 color)
{
    return pow(color, vec3(GammaCorrection));
}

void main()
{
    vec3 sampled   = texture(Texture0, ScreenTexCoords).rgb;
    vec3 mapped    = ToneMap(sampled);
    vec3 corrected = GammaCorrect(mapped);

    FragmentColor = vec4(corrected, 1.0f);
}
