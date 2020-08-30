// Version added by compiler

layout (location = 0) out vec4 outDiffuseAlpha;     // RGBA16F
layout (location = 1) out vec4 outSpecularAO;       // RGBA16F
layout (location = 2) out vec3 outViewNormal;       // RGBA16F

in vec4 vertexWorldPosition;

#if defined(ATTRIBUTE_NORMAL)
in vec3 vertexWorldNormal;
#endif

#if defined(ATTRIBUTE_TANGENT)
in vec3 vertexWorldTangent;
#endif

#if defined(TANGENT_SPACE)
in vec3 vertexWorldBitangent;
#endif

#if defined(ATTRIBUTE_COLOR)
in vec3 vertexColor;
#endif

#if defined(ATTRIBUTE_TEXCOORDS)
in vec2 vertexTexCoords;
#endif

struct MaterialFragmentInputs {
    vec3 normal;
    vec3 baseColor;
    vec3 emissionColor;
    float metallic;
    float roughness;
    float alphaBlend;
    float ambientOcclusion;
};

struct ShadingParams {
    mat3 TBN;
};

void initMaterialInputs(out MaterialFragmentInputs inputs) {
    inputs.baseColor = vec3(0.8f);
    inputs.emissionColor = vec3(0);
    inputs.metallic = 0.0f;
    inputs.roughness = 0.0f;
    inputs.alphaBlend = 1.0f;
    inputs.ambientOcclusion = 0.0f;
    inputs.normal = vec3(0.0f);
}

void prepateShadingParams(inout ShadingParams params) {
#if defined(ATTRIBUTE_NORMAL)
    vec3 N = normalize(vertexWorldNormal);
#endif

#if defined(ATTRIBUTE_TANGENT)
    vec3 T = normalize(vertexWorldTangent);
#endif

#if defined(TANGENT_SPACE)
    vec3 B = normalize(vertexWorldBitangent);
#endif

#if define(DOUBLE_SIDED)
    #if defined(ATTRIBUTE_NORMAL)
    N = gl_FrontFacing? N: -N;
    #endif

    #if defined(ATTRIBUTE_TANGENT)
    T = gl_FrontFacing? T: -T;
    #endif

    #if defined(TANGENT_SPACE)
    T = gl_FrontFacing? T: -T;
    #endif
#endif

    params.TBN = mat3(T,B,N);
}

void main() {
    ShadingParams params;
    MaterialFragmentInputs inputs;

    initMaterialInputs(inputs);
    prepateShadingParams(params);

    // Run user fragment code here.
    // if empty - ok, then the lightning will be evaluated with params from initMaterialInputs
    fragment(inputs);
}
