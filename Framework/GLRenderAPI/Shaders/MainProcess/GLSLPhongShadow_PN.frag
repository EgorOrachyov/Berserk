#version 410

#define DIR_LIGHTS 16
#define SPOT_LIGHTS 16
#define POINT_LIGHTS 16

#define DIR_SHADOW_LIGHTS 4
#define SPOT_SHADOW_LIGHTS 4
#define POINT_SHADOW_LIGHTS 4

in vec3 eyeNorm;
in vec3 eyePosition;

in vec4 fpls_dir[DIR_SHADOW_LIGHTS];
in vec4 fpls_spot[SPOT_SHADOW_LIGHTS];
in vec4 fpls_point[POINT_SHADOW_LIGHTS];

layout(location = 0) out vec4 FragColor;

// Materials

struct BaseMaterial
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

// Lights

struct DirectionalLight
{
    vec4 Direction;
    vec3 Intensity;
};

struct DirectionalShadowLight
{
    vec4 Direction;
    vec3 Intensity;

    sampler2D map;
};

struct PointLight
{
    vec4 Position;
    vec3 Intensity;

    float constant;
    float linear;
    float quadratic;

    float radius;
};

struct SpotLight
{
    vec4 Position;
    vec4 Direction;
    vec3 Intensity;

    float cutoff;
    float outerCutoff;
    float epsilon;
    float exponent;
};

// Uniform light info

uniform uint NUM_OF_DIR_LIGHTS;
uniform uint NUM_OF_SPOT_LIGHTS;
uniform uint NUM_OF_POINT_LIGHTS;

uniform DirectionalLight directionalLights[DIR_LIGHTS];
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];

uniform uint NUM_DIR_SHADOWS;
uniform uint NUM_SPOT_SHADOWS;
uniform uint NUM_POINT_SHADOWS;

uniform DirectionalShadowLight dirShadowLights[DIR_SHADOW_LIGHTS]; // new

uniform vec3 ambientLight = vec3(0);

// Uniform material info

uniform BaseMaterial Material;

// Phong model for Base Material

vec3 phongDirLight(in int index)
{
    vec3 s = normalize(-vec3(directionalLights[index].Direction));
    vec3 v = normalize(-eyePosition);
    vec3 h = normalize(v + s);

    return directionalLights[index].Intensity * (
        Material.Diffuse * max(dot(s, eyeNorm), 0.0) +
        Material.Specular * pow(max(dot(h, eyeNorm), 0.0), Material.Shininess)
    );
}

vec3 phongDirShadowLight(in int index)
{
    vec3 projCoords = fpls_dir[index].xyz / fpls_dir[index].w;
    projCoords = projCoords * 0.5 + 0.5;

    float closetDepth = texture(dirShadowLights[index].map, projCoords.xy).r;
    float currentDepth = projCoords.z;

    const float bias = 0.005;
    float light = currentDepth - bias > closetDepth ? 0.0 : 1.0;

    if (currentDepth - bias > closetDepth)
    {
        return vec3(0);
    }
    else
    {
        vec3 s = normalize(-vec3(directionalLights[index].Direction));
        vec3 v = normalize(-eyePosition);
        vec3 h = normalize(v + s);

        return light * directionalLights[index].Intensity * (
                Material.Diffuse * max(dot(s, eyeNorm), 0.0) +
                Material.Specular * pow(max(dot(h, eyeNorm), 0.0), Material.Shininess)
        );
    }

}

vec3 phongPointlLight(in int index)
{
    float distance = length(vec3(pointLights[index].Position) - eyePosition);

    if (distance >= pointLights[index].radius)
    {
        return vec3(0);
    }
    else
    {
        vec3 s = normalize(vec3(pointLights[index].Position) - eyePosition);
        vec3 v = normalize(-eyePosition);
        vec3 h = normalize(v + s);

        float attenuation = 1.0 / (
            pointLights[index].constant +
            pointLights[index].linear * distance +
            pointLights[index].quadratic * (distance * distance)
        );

        return pointLights[index].Intensity * attenuation * (
            Material.Diffuse * max(dot(s, eyeNorm), 0.0) +
            Material.Specular * pow(max(dot(h, eyeNorm), 0.0), Material.Shininess)
        );
    }
}

vec3 phongSpotLight(in int index)
{
    vec3 s = normalize(vec3(spotLights[index].Position) - eyePosition);
    float cosine = dot(-s, vec3(spotLights[index].Direction)); // normalize outside

    if (cosine >= spotLights[index].cutoff)
    {
        float factor = pow(cosine, spotLights[index].exponent) *
                       clamp((cosine - spotLights[index].outerCutoff) / spotLights[index].epsilon, 0.0, 1.0);

        vec3 v = normalize(-eyePosition);
        vec3 h = normalize(v + s);

        return spotLights[index].Intensity * factor * (
            Material.Diffuse * max(dot(s, eyeNorm), 0.0) +
            Material.Specular * pow(max(dot(h, eyeNorm), 0.0), Material.Shininess)
        );
    }
    else
    {
        return vec3(0);
    }
}

vec3 phongModel()
{
    vec3 result = ambientLight;

    for(int i = 0; i < NUM_OF_DIR_LIGHTS; i++)
    {
        result += phongDirLight(i);
    }

    for(int i = 0; i < NUM_OF_POINT_LIGHTS; i++)
    {
        result += phongPointlLight(i);
    }

    for(int i = 0; i < NUM_OF_SPOT_LIGHTS; i++)
    {
        result += phongSpotLight(i);
    }

    for(int i = 0; i < NUM_DIR_SHADOWS; i++)
    {
        result += phongDirShadowLight(i);
    }

    return result;
}

// Main subroutine process

void main()
{
    FragColor = vec4(phongModel(), 1.0);
}