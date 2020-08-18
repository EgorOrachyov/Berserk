#version 410 core

// Defines default shading algorithms for the material:
// - Phong based model
// - todo: PBR based on Lamber,Cook-Torrance, and Schlick-GGX

layout (location = 0) out vec4 outAlbedoAlpha;
layout (location = 1) out vec4 outSpecularAO;
layout (location = 2) out vec3 outNormal;

in MaterialInterfaceBlock {
    vec3 Position;
    vec3 PositionViewSpace;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Color;
    vec2 TexCoords;
} fs_in;

uniform sampler2D TextureAlbedo;
uniform sampler2D TextureSpecular;
uniform sampler2D TextureMetallic;
uniform sampler2D TextureRoughness;
uniform sampler2D TextureNormal;
uniform sampler2D TextureDinsplacement;
uniform sampler2D TextureAmbient;
uniform sampler2D TextureEmission;

layout (std140) uniform MaterialData {
    vec3 albedoColor;
    vec3 emissionColor;
    float specular;
    float metallic;
    float roughness;
    float ambientScale;
    float inverseGamma;
    bool isAlbedoSRGB;
} material;

layout (std140) uniform CameraData {
    vec3 position;      // World-Space
    vec3 direction;     // World-Space
    vec3 up;            // World-Space
} camera;

// Defined by compiler
 #define LIGHT_DIRECTIONAL 0
 #define LIGHT_SPOT        1
 #define LIGHT_POINT       2
 #define MAX_LIGHTS_PER_PIXEL 100
 #define MAX_LIGHTS_PER_SCENE 100

layout (std140) uniform SceneLightData {
    vec3 positions[MAX_LIGHTS_PER_SCENE];
    vec3 directions[MAX_LIGHTS_PER_SCENE];
    vec3 intensities[MAX_LIGHTS_PER_SCENE];
    int types[MAX_LIGHTS_PER_SCENE];
} sceneLigts;

layout (std140) uniform AffectingLights {
    int indices[MAX_LIGHTS_PER_PIXEL];
    int count;
} affectingLights;

// Material features flags defined by compiler
// #define FEATURE_ALBEDO
// #define FEATURE_SPECULAR
// #define FEATURE_METALLIC
// #define FEATURE_ROUGHNESS
// #define FEATURE_NORMAL
// #define FEATURE_DISPLACEMENT
// #define FEATURE_AMBIENT_OCCLUSION
// #define FEATURE_EMISSION
// #define FEATURE_TANGET_SPACE

vec3 srgbToLinear(in vec3 color) {
    return pow(color, vec3(material.inverseGamma));
}

vec3 getAlbedo() {
    vec3 albedo;

#if defined(FEATURE_ALBEDO)
    albedo = texture(TextureAlbedo, fs_in.TexCoords).xyz;

    if (material.isAlbedoSRGB) {
        albedo = srgbToLinear(albedo);
    }

    albedo *= material.albedoColor;
#else
    albedo = material.albedoColor;
#endif

    return albedo;
}

float getSpecular() {
    float specular;

#if defined(FEATURE_SPECULAR)
    specular = texture(TextureSpecular, fs_in.TexCoords).r;
    specular *= material.specular;
#else
    specular = material.specular;
#endif

    return specular;
}

vec3 getNormal() {
    vec3 normal;

#ifdef defined(FEATURE_TANGET_SPACE) && defined(FEATURE_NORMAL)
    vec3 T = normalize(fs_in.Tangent);
    vec3 B = normalize(fs_in.Bitangent);
    vec3 N = normalize(fs_in.Normal);
    mat3 TBN = mat3(T,B,N);

    normal = texture(TextureNormal, fs_in.TexCoords).xyz;
    normal = nomalize(normal * 2.0f - 1.0f);
    normal = normalize(TBN * normal);
#else
    normal = normalize(fs_in.Normal);
#endif

    return normal;
}

float getMetallic() {
    float metallic;

#if defined(FEATURE_METALLIC)
    metallic = texture(TextureMetallic, fs_in.TexCoords).r;
    metallic *= material.metallic;
#else
    metallic = material.metallic;
#endif

    return metallic;
}

float getRoughness() {
    float roughness;

#if defined(FEATURE_ROUGHNESS)
    roughness = texture(TextureRoughness, fs_in.TexCoords).r;
    roughness *= material.roughness;
#else
    roughness = material.roughness;
#endif

    return roughness;
}

float getAmbientOcclusion() {
    float ambient;

#if defined(FEATURE_AMBIENT_OCCLUSION)
    ambient = texture(TextureAmbient, fs_in.TexCoords).r;
    ambient *= material.ambientScale;
#else
    ambient = material.ambientScale;
#endif

    return ambient;
}

vec3 getEmission() {
    vec3 emission;

#if defined(FEATURE_EMISSION)
    emission = texture(TextureEmission, fs_in.TexCoords).rgb;
    emission *= material.emissionColor;
#else
    emission = material.emissionColor;
#endif

    return emission;
}

// Geometry inputs defined by compiler
// #define ATTRIBUTE_POSITION
// #define ATTRIBUTE_NORMAL
// #define ATTRIBUTE_TANGENT
// #define ATTRIBUTE_COLOR
// #define ATTRIBUTE_TEXTURE_COORDS

// Material flags bits
// #define FLAG_ENABLE_EMISSION
// #define FLAG_DO_NOT_RECEIVE_SHADOWS

void shadingPhong() {

}

// Material shading mode defined by compiler
// #define SHADING_PHONG
// #define SHADING_PBR
// #define CUSTOM

void main() {
#if defined(SHADING_PHONG)
    shadingPhong();
#else
    outAlbedoAlpha = vec4(1.0f,1.0f,1.0f,1.0f);
    outSpecularAO = vec4(0.0f,0.0f,0.0f,0.0f);
    outNormal = vec3(0.0f,0.0f,0.0f);
#endif
}