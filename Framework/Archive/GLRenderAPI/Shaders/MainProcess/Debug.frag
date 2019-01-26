#version 410

in vec3 eyeNorm;
in vec3 eyePosition;
in vec4 lightSpacePos[20];

out vec4 FragColor;

struct Data
{
    sampler2D map;
};

uniform Data Depth[20];

void main()
{
    vec3 projCoords = lightSpacePos[0].xyz / lightSpacePos[0].w;
    projCoords = projCoords * 0.5 + 0.5;

    float closetDepth = texture(Depth[0].map, projCoords.xy).r;
    float currentDepth = projCoords.z;

    const float bias = 0.05;

    if (currentDepth - bias > closetDepth)
    {
        FragColor = vec4(0.1,0.1,0.1,1.0);
    }
    else
    {
        FragColor = vec4(0.5,0.5,0.5,1.0);
    }
}
