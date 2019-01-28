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

vec3 phongModel(vec4 position, vec3 norm)
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect(-s, norm);

    float sDotn = max(dot(s, norm), 0.0);

    vec3 ambient = Light.La * Material.Ka;
    vec3 diffuse = Light.Ld * Material.Kd * sDotn;

    vec3 specular = vec3(0.0);

    if (sDotn > 0.0)
    {
        specular = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.Shininess);
    }

    return ambient + diffuse + specular;
}

void main()
{
    FragColor = vec4(phongModel(eyePosition, eyeNorm), 1.0);
}