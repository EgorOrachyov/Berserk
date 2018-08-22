#version 410 core

#define SHININESS_LEVELS 16

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 mDiffuse;
layout (location = 3) out vec4 mSpecularSh;
layout (location = 4) out vec3 gViewPosition;

in VS_OUT
{
    vec3 FragPos;
    vec3 FragNorm;
    vec2 FragTexCoords;
    mat3 TBN;
    vec3 FragViewPos;

} fs_in;

uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform float Shininess;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

subroutine void RenderPassType();
subroutine uniform RenderPassType GeomPass;
subroutine uniform RenderPassType MatPass;

subroutine(RenderPassType)
void FragPass_PN()
{
    gPosition = fs_in.FragPos;
    gViewPosition = fs_in.FragViewPos;
    gNormal = normalize(fs_in.FragNorm);
}

subroutine(RenderPassType)
void FragPass_PNT()
{
    gPosition = fs_in.FragPos;
    gViewPosition = fs_in.FragViewPos;
    gNormal = normalize(fs_in.FragNorm);
}

subroutine(RenderPassType)
void FragPass_PNBTT()
{
    gPosition = fs_in.FragPos;
    gViewPosition = fs_in.FragViewPos;

    vec3 normal = texture(NormalMap, fs_in.FragTexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);

    gNormal = normal;
}

subroutine(RenderPassType)
void MatPass_Default()
{
    mDiffuse = vec3(1);
    mSpecularSh = vec4(0);
}

subroutine(RenderPassType)
void MatPass_Basic()
{
    mDiffuse = DiffuseColor;
    mSpecularSh.rgb = SpecularColor;
    mSpecularSh.a = Shininess / SHININESS_LEVELS;
}

subroutine(RenderPassType)
void MatPass_DS_map()
{
    mDiffuse = DiffuseColor * texture(DiffuseMap, fs_in.FragTexCoords).rgb;
    mSpecularSh.rgb = SpecularColor * texture(SpecularMap, fs_in.FragTexCoords).rgb;
    mSpecularSh.a = Shininess / SHININESS_LEVELS;
}

void main()
{
    GeomPass();
    MatPass();
}