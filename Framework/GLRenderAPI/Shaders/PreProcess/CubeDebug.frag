#version 410

in vec3 FragPos;
in vec3 FragNorm;
out vec4 FragColor;

struct Data
{
    float radius;
    vec4 WorldPos;
    samplerCube map;
};

uniform Data Depth[20];

void main()
{
    vec3 FragToLight = FragPos - Depth[0].WorldPos.xyz;
    //float closestDept = texture(Depth[0].map, FragToLight).r;

    //closestDept *= Depth[0].radius;
    float currentDepth = length(FragToLight);

    float bias = 0.0011;
    float shadow = 0.0;
    float samples = 4.0;
    float offset = 0.1;

    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(Depth[0].map, FragToLight + vec3(x, y, z)).r;
                closestDepth *= Depth[0].radius;
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);

    FragColor = (1 - shadow) * vec4(0.5,0.5,0.5,1.0);
}
