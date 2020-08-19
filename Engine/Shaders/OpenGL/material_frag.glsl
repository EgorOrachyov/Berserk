#version 410 core

// Defines default shading algorithms for the material:
// - Phong based model
// - todo: PBR based on Lamber,Cook-Torrance, and Schlick-GGX

layout (location = 0) out vec4 outDiffuseAlpha;
layout (location = 1) out vec4 outSpecularAO;
layout (location = 2) out vec3 outViewNormal;

in MaterialInterfaceBlock {
    vec3 Position;
    vec3 PositionViewSpace;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Color;
    vec2 TexCoords;
} fs_in;

layout (std140) uniform CameraData {
    vec3 position;      // World-Space
    vec3 direction;     // World-Space
    vec3 up;            // World-Space
} camera;

// Defined by compiler
// #define MAX_LIGHTS_PER_PIXEL
// #define MAX_LIGHTS_PER_SCENE

layout (std140) uniform SceneLightData {
    vec3 positions[MAX_LIGHTS_PER_SCENE];
    vec3 directions[MAX_LIGHTS_PER_SCENE];
    vec3 distances[MAX_LIGHTS_PER_SCENE];
    vec3 energy[MAX_LIGHTS_PER_SCENE];
    vec4 custom[MAX_LIGHTS_PER_SCENE];
    int types[MAX_LIGHTS_PER_SCENE];
} sceneLights;

layout (std140) uniform AffectingLights {
    int indices[MAX_LIGHTS_PER_PIXEL];
    int count;
} affectingLights;

#include "material_uniform.glsl"

// Geometry inputs defined by compiler
// #define ATTRIBUTE_POSITION
// #define ATTRIBUTE_NORMAL
// #define ATTRIBUTE_TANGENT
// #define ATTRIBUTE_COLOR
// #define ATTRIBUTE_TEXTURE_COORDS

// Material flags bits
// #define FLAG_ENABLE_EMISSION
// #define FLAG_DO_NOT_RECEIVE_SHADOWS

// Defined by compiler
// #define LIGHT_DIRECTIONAL
// #define LIGHT_SPOT
// #define LIGHT_POINT

void getLightIEL(in int affectorIndex, in vec3 position, in vec3 normal, out float I, out vec3 E, out vec3 L) {
    int index = affectingLights.indices[affectorIndex];
    int type = sceneLights.types[index];

    if (type == LIGHT_DIRECTIONAL) {
        vec3 D = sceneLights.directions[index];

        I = max(dot(-D, normal),0.0f);
        E = sceneLights.energy[index];
        L = D;
    }
    else if (type == LIGHT_POINT) {
        vec3 P = sceneLights.positions[index];
        vec3 Ld = P - position;
        float R = sceneLights.custom[index].x;
        float D2 = dot(Ld,Ld);

        I = pow(max(0.0f, 1.0f - pow(D2 / (R * R), 2)), 2);
        E = sceneLights.energy[index];
        L = Ld;
    }
    else if (type == LIGHT_SPOT) {
        vec3 P = sceneLights.positions[index];
        vec3 D = sceneLights.directions[index];
        vec3 Ld = P - position;
        float cosRadInner = sceneLights.custom[index].x;
        float cosRadOuter = sceneLights.custom[index].y;
        float DdotLd = dot(D, -Ld);

        I = clamp((DdotLd - cosRadOuter) / (cosRadInner - cosRadOuter), 0.0f, 1.0f);
        E = sceneLights.energy[index];
        L = Ld;
    }

    I = 0.0f;
    E = vec3(0.0f);
    L = vec3(0.0f);
}

// Material shading mode defined by compiler
// #define SHADING_BLINN_PHONG
// #define SHADING_PBR
// #define CUSTOM

#if defined(SHADING_BLINN_PHONG)
void shadingBlinnPhong() {
#if defined(ATTRIBUTE_NORMAL) && defined(ATTRIBUTE_TEXTURE_COORDS)
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    vec3 normal = vec3(0.0f);
    float ao = getAmbientOcclusion();
    float alpha = getAlpha();

    vec3 P = fs_in.Position;
    vec3 N = getNormal();
    vec3 V = normalize(camera.position - fs_in.Position);

    float S = getSpecular();


    for (int i = 0; i < affectingLights.count; i++) {
        float I;
        vec3 E;
        vec3 L;

        getLightIEL(i,P,N,I,E,L);

        // Has non-zero intensity
        if (I > 0.0f) {
            L = normalize(L);

            vec3 H = normalize(V + L);

            float Kd = I;
            float Ks = pow(max(dot(H,N), 0.0f), S);

            diffuse += E * Kd;
            specular += E * Ks;
        }
    }

    diffuse = diffuse * getAlbedo() * fs_in.Color + getEmission();
    specular = specular * getAlbedo() * fs_in.Color;

    outDiffuseAlpha = vec4(diffuse, alpha);
    outSpecularAO = vec4(specular, ao);
    outViewNormal = normal;
#else
    vec3 intensity = fs_in.Color + getEmission();
    vec3 specular = fs_in.Color;
    float ao = getAmbientOcclusion();
    float alpha = getAlpha();

    outDiffuseAlpha = vec4(intensity, alpha);
    outSpecularAO = vec4(specular, ao);
    outViewNormal = vec3(0.0f);
#endif
}
#endif // SHADING_BLINN_PHONG

void main() {
#if defined(SHADING_BLINN_PHONG)
    shadingBlinnPhong();
#else
    outDiffuseAlpha = vec4(1.0f,1.0f,1.0f,1.0f);
    outSpecularAO = vec4(0.0f,0.0f,0.0f,0.0f);
    outViewNormal = vec3(0.0f);
#endif
}