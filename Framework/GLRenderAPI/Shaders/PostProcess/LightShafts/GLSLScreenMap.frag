#version 410 core

layout (location = 0) out vec3 FragColor;

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

uniform sampler2D   Depth;
uniform vec3        LightColor;
uniform vec3        LightPosition;
uniform vec3        LightBackColor;
uniform float       LightRadius;
uniform float       Upscale; // todo: remove

void main()
{
    vec2  delta = LightPosition.xy - fs_in.FragTexCoords;
          delta.y *= Upscale;
    float dist = length(delta);
    float depth = texture(Depth, fs_in.FragTexCoords).r;

    FragColor = LightBackColor;

    if (dist <= LightRadius)
    {
        FragColor = LightColor;

        if (depth < 1.0 && depth < LightPosition.z)
        {
            FragColor = vec3(0.0);
        }
    }
    else
    {
        if (depth < 1.0)
        {
            FragColor = vec3(0.0);
        }
    }
}
