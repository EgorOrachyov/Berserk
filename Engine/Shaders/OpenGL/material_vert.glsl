#version 410 core

// Generic material vertex shader.
// Compiles separatelly for each type of the vertex shader input

// Inputs, will be inserted automatically
// layout (location = 0) in vec3 inPosition;
// layout (location = 1) in vec3 inNormal;
// layout (location = 2) in vec3 inTangent;
// layout (location = 3) in vec3 inColor;
// layout (location = 4) in vec2 inTexCoords;

// Geometry inputs defined by compiler
// #define ATTRIBUTE_POSITION
// #define ATTRIBUTE_NORMAL
// #define ATTRIBUTE_TANGENT
// #define ATTRIBUTE_COLOR
// #define ATTRIBUTE_TEXTURE_COORDS

// Per object transformation data
layout (std140) uniform TransformData {
    mat4 model;
    mat4 normal;
    mat4 viewModel;
    mat4 projViewModel;
} transform;

out MaterialInterfaceBlock {
    vec3 Position;
    vec3 PositionViewSpace;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Color;
    vec2 TexCoords;
} vs_out;

void main() {

#if defined(ATTRIBUTE_COLOR)
    vs_out.Color = inColor;
#else
    vs_out.Color = vec3(1.0f,1.0f,1.0f);
#endif

#if defined(ATTRIBUTE_TEXTURE_COORDS)
    vs_out.TexCoords = inTexCoords;
#endif

#if defined(ATTRIBUTE_NORMAL)
    #if defined(ATTRIBUTE_TANGENT)

    vec3 T = (transform.normal * vec4(inTangent, 0.0f)).xyz;
    vec3 N = (transform.normal * vec4(inNormal, 0.0f)).xyz;
    vec3 B;

    T = normalize(T);
    N = normalize(N);
    B = cross(N,T);

    vs_out.Tangent = T;
    vs_out.Bitangent = B;
    vs_out.Normal = N;

    #else

    vec3 N = (transform.normal * vec4(inNormal, 0.0f)).xyz;
    N = normalize(N);
    vs_out.Normal = N;

    #endif
#endif

#if defined(ATTRIBUTE_POSITION)
    vec4 position = vec4(inPosition, 1.0f);
    vs_out.Position = vec3(transform.model * position);
    vs_out.PositionViewSpace = vec3(transform.viewModel * position);
    gl_Position = transform.projViewModel * position;
#endif

}