#version 410 core

layout (location = 0) out vec4 FragColor;

in VS_OUT
{
    vec2 WorldTexCoords;
    vec2 ScreenPosition;
}
fs_in;

uniform sampler2D Texture0;

void main()
{
    /*

    vec2 texelSize   = 1.0 / vec2(textureSize(Texture0, 0));
    const int side   = 1;
    const int range  = (2 * side + 1) * (2 * side + 1);
    vec3 result      = vec3(0.0);

    for (int x = -side; x <= side; ++x)
    {
        for (int y = -side; y <= side; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(Texture0, fs_in.WorldTexCoords + offset).rgb;
        }
    }

    result /= range;
    FragColor = vec4(vec3(1.0) - exp(-1.3 * result), 1.0);

    */
    vec2  texelSize = 1.0 / vec2(textureSize(Texture0, 0));
    ivec2 pos       = ivec2(fs_in.ScreenPosition);
    vec3  result    = texture(Texture0, fs_in.WorldTexCoords).xyz;
    ivec2 last      = ivec2(0);
    const float samples = 1.0 / 4.0;
    const float weights[3] = float[3](0.9, 0.9, 0.8);

    if (pos.x % 2 == 0)
    {
        last.x = -1;
        vec2 offset = vec2(float(-1), float(0)) * texelSize;
        result += weights[0] * texture(Texture0, fs_in.WorldTexCoords + offset).xyz;
    }
    else
    {
        last.x = 1;
        vec2 offset = vec2(float(1), float(0)) * texelSize;
        result += weights[0] * texture(Texture0, fs_in.WorldTexCoords + offset).xyz;
    }

    if (pos.y % 2 == 0)
    {
        last.y = -1;
        vec2 offset = vec2(float(0), float(-1)) * texelSize;
        result += weights[1] * texture(Texture0, fs_in.WorldTexCoords + offset).xyz;
    }
    else
    {
        last.y = 1;
        vec2 offset = vec2(float(0), float(1)) * texelSize;
        result += weights[1] * texture(Texture0, fs_in.WorldTexCoords + offset).xyz;
    }

    result += weights[2] * texture(Texture0, fs_in.WorldTexCoords + last).xyz;

    FragColor = vec4(vec3(1.0) - exp(-2.5 * result * samples), 1.0);
    //FragColor = vec4(vec3(1.0) - exp(-1.3 * texture(Texture0, fs_in.WorldTexCoords).xyz), 1.0);

}
