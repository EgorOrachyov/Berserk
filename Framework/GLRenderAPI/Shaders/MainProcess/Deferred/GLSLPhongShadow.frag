#version 410 core

#define DIR_LIGHTS 16
#define SPOT_LIGHTS 16
#define POINT_LIGHTS 16

#define DIR_SHADOW_LIGHTS 2
#define SPOT_SHADOW_LIGHTS 2
#define POINT_SHADOW_LIGHTS 2

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

//uniform sampler2D           dirMap      [DIR_SHADOW_LIGHTS];
//uniform sampler2D           spotMap     [SPOT_SHADOW_LIGHTS];
//uniform samplerCube         pointMap    [POINT_SHADOW_LIGHTS];

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

vec3 phongPointlLight(in uint index)
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

vec3 phong()
{
    vec3 result = AmbientLight;

    for(uint i = 0; i < NUM_DIR_L; i++)
    {
        result += phongDirLight(i);
    }

    for(uint i = 0; i < NUM_SPOT_L; i++)
    {
        result += phongSpotLight(i);
    }

    for(uint i = 0; i < NUM_POINT_L; i++)
    {
        result += phongPointlLight(i);
    }

    return result;
}

void main()
{
    Position    = texture(gPosition,    fs_in.FragTexCoords).xyz;
    Normal      = texture(gNormal,      fs_in.FragTexCoords).xyz;
    Diffuse     = texture(gDiffuse,     fs_in.FragTexCoords).rgb;
    Specular    = texture(gSpecularSh,  fs_in.FragTexCoords).rgb;
    Shininess   = texture(gSpecularSh,  fs_in.FragTexCoords).a;

	FragColor = vec4(phong(), 1.0);
}