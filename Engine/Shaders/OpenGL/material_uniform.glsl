// Defines access to the material uniform data and textures
// Note: relies on the material interface block structure

#if defined(FEATURE_ALBEDO)
uniform sampler2D TextureAlbedo;
#endif

#if defined(FEATURE_SPECULAR)
uniform sampler2D TextureSpecular;
#endif

#if defined(FEATURE_METALLIC)
uniform sampler2D TextureMetallic;
#endif

#if defined(FEATURE_ROUGHNESS)
uniform sampler2D TextureRoughness;
#endif

#if defined(FEATURE_NORMAL)
uniform sampler2D TextureNormal;
#endif

#if defined(FEATURE_DISPLACEMENT)
uniform sampler2D TextureDinsplacement;
#endif

#if defined(FEATURE_AMBIENT_OCCLUSION)
uniform sampler2D TextureAmbient;
#endif

#if defined(FEATURE_EMISSION)
uniform sampler2D TextureEmission;
#endif

layout (std140) uniform MaterialData {
    vec3 albedoColor;
    vec3 emissionColor;
    float alpha;
    float specular;
    float metallic;
    float roughness;
    float ambientScale;
    float inverseGamma;
    bool isAlbedoSRGB;
} materialData;

// Material features flags defined by compiler
// #define FEATURE_ALBEDO
// #define FEATURE_SPECULAR
// #define FEATURE_METALLIC
// #define FEATURE_ROUGHNESS
// #define FEATURE_NORMAL
// #define FEATURE_DISPLACEMENT
// #define FEATURE_AMBIENT_OCCLUSION
// #define FEATURE_EMISSION

vec3 srgbToLinear(in vec3 color) {
    return pow(color, vec3(materialData.inverseGamma));
}

vec3 getAlbedo() {
    vec3 albedo;

#if defined(FEATURE_ALBEDO)
    albedo = texture(TextureAlbedo, fs_in.TexCoords).xyz;

    if (materialData.isAlbedoSRGB) {
        albedo = srgbToLinear(albedo);
    }

    albedo *= materialData.albedoColor;
#else
    albedo = materialData.albedoColor;
#endif

    return albedo;
}

float getSpecular() {
    float specular;

#if defined(FEATURE_SPECULAR)
    specular = texture(TextureSpecular, fs_in.TexCoords).r;
    specular *= materialData.specular;
#else
    specular = materialData.specular;
#endif

    return specular;
}

vec3 getNormal() {
    vec3 normal;

#if defined(FEATURE_NORMAL)
    vec3 T = normalize(fs_in.Tangent);
    vec3 B = normalize(fs_in.Bitangent);
    vec3 N = normalize(fs_in.Normal);
    mat3 TBN = mat3(T,B,N);

    normal = texture(TextureNormal, fs_in.TexCoords).xyz;
    normal = normalize(normal * 2.0f - 1.0f);
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
    metallic *= materialData.metallic;
#else
    metallic = materialData.metallic;
#endif

    return metallic;
}

float getRoughness() {
    float roughness;

#if defined(FEATURE_ROUGHNESS)
    roughness = texture(TextureRoughness, fs_in.TexCoords).r;
    roughness *= materialData.roughness;
#else
    roughness = materialData.roughness;
#endif

    return roughness;
}

float getAmbientOcclusion() {
    float ambient;

#if defined(FEATURE_AMBIENT_OCCLUSION)
    ambient = texture(TextureAmbient, fs_in.TexCoords).r;
    ambient *= materialData.ambientScale;
#else
    ambient = materialData.ambientScale;
#endif

    return ambient;
}

vec3 getEmission() {
    vec3 emission;

#if defined(FEATURE_EMISSION)
    emission = texture(TextureEmission, fs_in.TexCoords).rgb;
    emission *= materialData.emissionColor;
#else
    emission = materialData.emissionColor;
#endif

    return emission;
}

float getAlpha() {
    return materialData.alpha;
}