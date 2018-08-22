#version 410 core

#define SAMPLES_COUNT 64

layout (location = 0) out float AmbientOcclusion;

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

uniform sampler2D gViewPosition;
uniform sampler2D gNormal;
uniform sampler2D Noise;

uniform uint    samplesCount;
uniform vec3    samples[SAMPLES_COUNT];
uniform vec2    noiseScale;
uniform float   radius = 0.5;

uniform mat4    View;
uniform mat4    Proj;

void main()
{
    vec3 position   = texture(gViewPosition, fs_in.FragTexCoords).xyz;
    vec3 normal     = vec3(View * vec4(texture(gNormal, fs_in.FragTexCoords).rgb, 0.0));

    vec3 randomVec  = texture(Noise,        fs_in.FragTexCoords * noiseScale).xyz;
    vec3 tangent    = normalize(randomVec - normal * dot(normal, randomVec));
    vec3 bitangent  = cross(normal, tangent);

    mat3 TBN        = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    const float bias = 0.0016;

    for (uint i = 0; i < samplesCount; ++i)
    {
        vec3 current = TBN * samples[i];
             current = position + current * radius;

        vec4 offset = vec4(current, 1.0);
             offset = Proj * offset;
             offset.xy /= offset.w;
             offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = texture(gViewPosition, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));

        occlusion += (current.z + bias >= sampleDepth ? 1.0 : 0.0) * rangeCheck;
    }

	AmbientOcclusion = occlusion / float(samplesCount);
}
