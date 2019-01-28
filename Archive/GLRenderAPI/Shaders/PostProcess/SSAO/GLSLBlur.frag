#version 410 core

layout (location = 0) out float FragColor;

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

uniform sampler2D Input;

void main()
{
    vec2 texelSize      = 1.0 / vec2(textureSize(Input, 0));
    const int side      = 2;
    const int range     = 4;
    float result        = 0.0;

    for (int x = -side; x < side; ++x)
    {
        for (int y = -side; y < side; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(Input, fs_in.FragTexCoords + offset).r;
        }
    }

    FragColor = result / float(range * range);
}
