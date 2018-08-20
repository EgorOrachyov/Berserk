#version 410 core

#define DIR_LIGHTS 16
#define SPOT_LIGHTS 16
#define POINT_LIGHTS 16

#define DIR_SHADOW_LIGHTS 2
#define SPOT_SHADOW_LIGHTS 2
#define POINT_SHADOW_LIGHTS 2

#define SHININESS_LEVELS 16

struct DirectionalLight
{
    vec4 Direction;
    vec3 Intensity;
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

struct PointLight
{
    vec4 Position;
    vec3 Intensity;

    float constant;
    float linear;
    float quadratic;
    float radius;
};

in VS_OUT
{
    vec2 FragTexCoords;
}
fs_in;

vec3 Position;
vec3 Normal;
vec3 Diffuse;
vec3 Specular;
float Shininess;

layout (location = 0) out vec4 FragColor;

uniform vec3 CameraPosition;
uniform vec3 AmbientLight = vec3(0);

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecularSh;

uniform DirectionalLight    dirLight    [DIR_LIGHTS];
uniform SpotLight           spotLight   [SPOT_LIGHTS];
uniform PointLight          pointLight  [POINT_LIGHTS];

uniform DirectionalLight    dirSLight   [DIR_SHADOW_LIGHTS];
uniform SpotLight           spotSLight  [SPOT_SHADOW_LIGHTS];
uniform PointLight          pointSLight [POINT_SHADOW_LIGHTS];

uniform sampler2D           dirMap      [DIR_SHADOW_LIGHTS];
uniform sampler2D           spotMap     [SPOT_SHADOW_LIGHTS];
uniform samplerCube         pointMap    [POINT_SHADOW_LIGHTS];

uniform mat4                dirMat      [DIR_SHADOW_LIGHTS];
uniform mat4                spotMat     [SPOT_SHADOW_LIGHTS];

uniform uint NUM_DIR_L;
uniform uint NUM_SPOT_L;
uniform uint NUM_POINT_L;

uniform uint NUM_DIR_SL;
uniform uint NUM_SPOT_SL;
uniform uint NUM_POINT_SL;

vec3 phongDirLight(in uint index)
{
    vec3 s = normalize(-dirLight[index].Direction.xyz);
    vec3 v = normalize(CameraPosition - Position);
    vec3 h = normalize(v + s);

    return dirLight[index].Intensity * (
        Diffuse *  max(dot(s, Normal), 0.0) +
        Specular * pow(max(dot(h, Normal), 0.0), Shininess)
    );
}

vec3 phongSpotLight(in uint index)
{
    vec3 s = normalize(spotLight[index].Position.xyz - Position);
    float cosine = dot(-s, spotLight[index].Direction.xyz); // normalize outside

    if (cosine >= spotLight[index].cutoff)
    {
        float factor = pow(cosine, spotLight[index].exponent) *
                       clamp((cosine - spotLight[index].outerCutoff) / spotLight[index].epsilon, 0.0, 1.0);

        vec3 v = normalize(CameraPosition - Position);
        vec3 h = normalize(v + s);

        return spotLight[index].Intensity * factor * (
            Diffuse  * max(dot(s, Normal), 0.0) +
            Specular * pow(max(dot(h, Normal), 0.0), Shininess)
        );
    }
    else
    {
        return vec3(0);
    }
}

vec3 phongPointLight(in uint index)
{
    float distance = length(pointLight[index].Position.xyz - Position);

    if (distance >= pointLight[index].radius)
    {
        return vec3(0);
    }
    else
    {
        vec3 s = normalize(pointLight[index].Position.xyz - Position);
        vec3 v = normalize(CameraPosition - Position);
        vec3 h = normalize(v + s);

        float attenuation = 1.0 / (
            pointLight[index].constant +
            pointLight[index].linear * distance +
            pointLight[index].quadratic * (distance * distance)
        );

        return pointLight[index].Intensity * attenuation * (
            Diffuse  * max(dot(s, Normal), 0.0) +
            Specular * pow(max(dot(h, Normal), 0.0), Shininess)
        );
    }
}

vec3 phongDirShadowLight(in uint index)
{
    vec3 projCoords = (dirMat[index] * vec4(Position, 1.0)).xyz;
    projCoords = projCoords * 0.5 + 0.5;
    float shadow = 0.0;

    if (projCoords.z < 1.0)
    {
        float currentDepth = projCoords.z;
        const float bias = 0.0011;

        vec2 texelSize = 1.0 / textureSize(dirMap[index], 0);

        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(dirMap[index], projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);
            }
        }

        shadow /= 9.0;
    }

    vec3 s = normalize(-dirSLight[index].Direction.xyz);
    vec3 v = normalize(CameraPosition - Position);
    vec3 h = normalize(v + s);

    return (1.0 - shadow) * dirSLight[index].Intensity * (
        Diffuse *  max(dot(s, Normal), 0.0) +
        Specular * pow(max(dot(h, Normal), 0.0), Shininess)
    );
}

vec3 phongSpotShadowLight(in uint index)
{
    vec3 s = normalize(spotSLight[index].Position.xyz - Position);
    float cosine = dot(-s, spotSLight[index].Direction.xyz); // normalize outside

    if (cosine < spotSLight[index].cutoff)
    {
        return vec3(0);
    }
    else
    {
        vec4 lightSpacePos = spotMat[index] * vec4(Position, 1.0);
        vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
        projCoords = projCoords * 0.5 + 0.5;
        float shadow = 0.0;

        if (projCoords.z < 1.0)
        {
            float currentDepth = projCoords.z;
            const float bias = 0.0011;

            vec2 texelSize = 1.0 / textureSize(spotMap[index], 0);

            for(int x = -1; x <= 1; ++x)
            {
                for(int y = -1; y <= 1; ++y)
                {
                    float pcfDepth = texture(spotMap[index], projCoords.xy + vec2(x, y) * texelSize).r;
                    shadow += (currentDepth - bias > pcfDepth ? 1.0 : 0.0);
                }
            }

            shadow /= 9.0;
        }

        float factor = pow(cosine, spotSLight[index].exponent) *
                       clamp((cosine - spotSLight[index].outerCutoff) / spotSLight[index].epsilon, 0.0, 1.0);

        vec3 v = normalize(CameraPosition - Position);
        vec3 h = normalize(v + s);

        return (1.0 - shadow) * spotSLight[index].Intensity * factor * (
            Diffuse  * max(dot(s, Normal), 0.0) +
            Specular * pow(max(dot(h, Normal), 0.0), Shininess)
        );
    }
}

vec3 phongPointShadowLight(in uint index, samplerCube map)
{
    vec3 FragToLight = Position - pointSLight[index].Position.xyz;
    float currentDepth = length(FragToLight);
    float distance = currentDepth;

    if (distance >= pointSLight[index].radius)
    {
        return vec3(0);
    }
    else
    {
        float closestDept = texture(map, FragToLight).r;
        closestDept *= pointSLight[index].radius;

        const float bias = 0.0011;

        // todo: pcf

        if (currentDepth - bias > closestDept)
        {
            return vec3(0);
        }
        else
        {
            vec3 s = normalize(-FragToLight);
            vec3 v = normalize(CameraPosition - Position);
            vec3 h = normalize(v + s);

            float attenuation = 1.0 / (
                pointSLight[index].constant +
                pointSLight[index].linear * distance +
                pointSLight[index].quadratic * (distance * distance)
            );

            return pointSLight[index].Intensity * attenuation * (
                Diffuse  * max(dot(s, Normal), 0.0) +
                Specular * pow(max(dot(h, Normal), 0.0), Shininess)
            );
        }
    }
}

vec3 phong()
{
    vec3 result = AmbientLight;

    for(uint i = 0; i < NUM_DIR_L; ++i)
    {
        result += phongDirLight(i);
    }

    for(uint i = 0; i < NUM_SPOT_L; ++i)
    {
        result += phongSpotLight(i);
    }

    for(uint i = 0; i < NUM_POINT_L; ++i)
    {
        result += phongPointLight(i);
    }

    for(uint i = 0; i < NUM_DIR_SL; ++i)
    {
        result += phongDirShadowLight(i);
    }

    for(uint i = 0; i < NUM_SPOT_SL; ++i)
    {
        result += phongSpotShadowLight(i);
    }

    for(uint i = 0; i < NUM_POINT_SL; ++i)
    {
        if (i == 0)
        {
            result += phongPointShadowLight(i, pointMap[0]);
        }
        else if (i == 1)
        {
            result += phongPointShadowLight(i, pointMap[1]);
        }
    }

    return result;
}

void main()
{
    Position    = texture(gPosition,    fs_in.FragTexCoords).xyz;
    Normal      = texture(gNormal,      fs_in.FragTexCoords).xyz;
    Diffuse     = texture(gDiffuse,     fs_in.FragTexCoords).rgb;
    Specular    = texture(gSpecularSh,  fs_in.FragTexCoords).rgb;
    Shininess   = texture(gSpecularSh,  fs_in.FragTexCoords).a * SHININESS_LEVELS;

	FragColor = vec4(phong(), 1.0);
}