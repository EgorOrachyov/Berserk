#version 410 core

layout (location = 0) out vec4 FragColor;

in VS_OUT
{
    vec4 WorldPosition;
    vec4 WorldNormal;
}
fs_in;

void main()
{
    const vec4 Color = vec4(0.6, 0.6, 0.6, 1.0);
    const vec3 LightPosition = vec3(0.7, 0.4, 0.3);

    vec3 s = normalize(LightPosition - fs_in.WorldPosition.xyz);

	FragColor = max(dot(s, fs_in.WorldNormal.xyz), 0.0) * Color;
}
