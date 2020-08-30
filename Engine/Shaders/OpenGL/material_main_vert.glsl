// Version added by compiler

layout (location = LOCATION_POSITION) in vec3 inPosition;

#if defined(ATTRIBUTE_NORMAL)
layout (location = LOCATION_NORMAL) in vec3 inNormal;
#endif

#if defined(ATTRIBUTE_TANGENT)
layout (location = LOCATION_TANGENT) in vec3 inTangent;
#endif

#if defined(ATTRIBUTE_COLOR)
layout (location = LOCATION_COLOR) in vec3 inColor;
#endif

#if defined(ATTRIBUTE_TEXCOORDS)
layout (location = LOCATION_TEXCOORDS) in vec2 inTexCoords;
#endif

out vec4 vertexWorldPosition;

#if defined(ATTRIBUTE_NORMAL)
out vec3 vertexWorldNormal;
#endif

#if defined(ATTRIBUTE_TANGENT)
out vec3 vertexWorldTangent;
#endif

#if defined(TANGENT_SPACE)
out vec3 vertexWorldBitangent;
#endif

#if defined(ATTRIBUTE_COLOR)
out vec3 vertexColor;
#endif

#if defined(ATTRIBUTE_TEXCOORDS)
out vec2 vertexTexCoords;
#endif

struct MaterialVertexInputs {
    vec2 texCoords;
    vec3 color;
    vec3 worldNormal;
    vec3 worldTangent;
    vec4 worldPosition;
};

void initMaterialInputs(out MaterialVertexInputs inputs) {
#if defined(ATTRIBUTE_NORMAL)
    inputs.worldNormal = normalize(getModelToWorldNormallMatrix() * (inNormal));
#endif

#if defined(ATTRIBUTE_TANGENT)
    inputs.worldTangent = normalize(getModelToWorldNormallMatrix() * (inTangent));
#endif

#if defined(ATTRIBUTE_COLOR)
    inputs.color = inColor;
#endif

#if defined(ATTRIBUTE_TEXCOORDS)
    #if defined(FLIP_UV)
    inputs.texCoords = vec2(inTexCoords.x, 1.0f - inTexCoords.y);
    #else
    inputs.texCoords = inTexCoords;
    #endif
#endif

#if defined(DOMAIN_MODEL)
    inputs.worldPosition = getModelToWorldMatrix() * vec4(inPosition, 1.0f);
#endif
}

void main() {
    // Initialize material inputs
    MaterialVertexInputs inputs;
    initMaterialInputs(inputs);

    // Execute user code provided in the method
    // If empty - ok, since everything is explicitly was setup before
    vertex(inputs);

    // Pass inputs to the fragment shader via interface
    vertexWorldPosition = inputs.worldPosition;

#if defined(ATTRIBUTE_NORMAL)
    vertexWorldNormal = inputs.worldNormal;
#endif

#if defined(ATTRIBUTE_TANGENT)
    vertexWorldTangent = inputs.worldTangent;
#endif

#if defined(TANGENT_SPACE)
    vertexWorldBitangent = cross(vertexWorldNormal, vertexWorldTangent);
#endif

#if defined(ATTRIBUTE_COLOR)
    vertexColor = inputs.color;
#endif

#if defined(ATTRIBUTE_TEXCOORDS)
    vertexTexCoords = inputs.texCoords;
#endif

    // Setup gl position since it is acctually will be taken into account by gl
    gl_Position = inputs.worldPosition;
}
