#version 410 core

layout (location = 0) out vec3 FragColor;

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

uniform float Exposure  = 0.1;
uniform float Decay     = 1.01; // in [1; +inf]
uniform float Density   = 1.0;
uniform float Weight    = 0.08;
uniform vec2  Position;

uniform sampler2D   LightMap;

const int NUM_SAMPLES = 100 ;

void main()
{
    vec2 textureCoords = fs_in.FragTexCoords;
    vec2 deltaTexCoords = Position - textureCoords;
         deltaTexCoords *= Density / float(NUM_SAMPLES);

    float illuminationDecay = 1.0;
    vec3 result = vec3(0);

    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        textureCoords += deltaTexCoords;

        vec3 color = texture(LightMap, textureCoords).rgb;
             color *= illuminationDecay;

        result += color;
        illuminationDecay *= Decay;
    }

    FragColor = result * Exposure * Weight;
}
