// Material uniform data structure
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
} materialData;

// Defines access to the material uniform data and textures
// Note: relies on the material interface block structure
//
// in MaterialInterfaceBlock {
//     vec3 Position;
//     vec3 Normal;
//     vec3 Tangent;
//     vec3 Bitangent;
//     vec2 TexCoords;
// } fs_in;

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