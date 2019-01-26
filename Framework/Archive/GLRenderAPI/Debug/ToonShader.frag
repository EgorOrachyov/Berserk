#version 410

in vec3 eyeNorm;
in vec4 eyePosition;

out vec4 FragColor;

struct LightInfo
{
    vec4 Position;      // light position in view coordinates
    vec3 La;            // Light ambient component
    vec3 Ld;            // Light diffuse component
    vec3 Ls;            // Light specular component
};

uniform LightInfo Light;

struct MaterialInfo
{
    vec3 Ka;            // ambient component
    vec3 Kd;            // diffuse component
    vec3 Ks;            // specular component
    float Shininess;    // exponent of specular component
};

uniform MaterialInfo Material;

uniform int levels;

vec3 toon(vec4 position, vec3 norm)
{
    float scaleFactor = 1.0 / levels;

    vec3 s = normalize(vec3(Light.Position - position));
    float sDotn = max(dot(s, norm), 0.0);

    vec3 ambient = Light.La * Material.Ka;
    vec3 diffuse = Light.Ld * Material.Kd * floor(sDotn * levels) * scaleFactor;

    return ambient + diffuse;
}

void main()
{
    FragColor = vec4(toon(eyePosition, eyeNorm), 1.0);
}