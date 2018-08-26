#version 410 core

layout (location = 0) out vec3 FragColor;

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

uniform sampler2D Input;
uniform sampler2D Blend;

void main()
{
    vec2 texelSize      = 1.0 / vec2(textureSize(Input, 0));
    const int side      = 2;
    const int range     = 4;
    vec3  result        = vec3(0.0);

    for (int x = -side; x < side; ++x)
    {
        for (int y = -side; y < side; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(Input, fs_in.FragTexCoords + offset).rgb;
        }
    }

    vec3 color = texture(Blend, fs_in.FragTexCoords).rgb;
    vec3 ranged = clamp(color, vec3(0.0), vec3(64.0));

    FragColor = (result / float(range * range)) + ranged;
}
