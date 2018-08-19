#version 410 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;
layout (location = 3) in vec3 VertexBitangent;
layout (location = 4) in vec3 VertexTangent;

out VS_OUT
{
    vec3 FragPos;
    vec3 FragNorm;
    vec2 FragTexCoords;
    mat3 TBN;

} vs_out;

uniform mat4 Model;
uniform mat4 PVM;

subroutine void RenderPassType();
subroutine uniform RenderPassType VertPass;

subroutine(RenderPassType)
void VertPass_PN()
{
    vs_out.FragNorm = vec3(Model * vec4(VertexNormal, 0.0));
    vs_out.FragTexCoords = vec2(0);
}

subroutine(RenderPassType)
void VertPass_PNT()
{
    vs_out.FragNorm = vec3(Model * vec4(VertexNormal, 0.0));
    vs_out.FragTexCoords = VertexTexCoords;
}

subroutine(RenderPassType)
void VertPass_PNBTT()
{
    vec3 T = normalize(vec3(Model * vec4(VertexTangent, 0.0)));
    vec3 B = normalize(vec3(Model * vec4(VertexBitangent, 0.0)));
    vec3 N = normalize(vec3(Model * vec4(VertexNormal, 0.0)));

    vs_out.TBN = mat3(T,B,N);
    vs_out.FragTexCoords = VertexTexCoords;
}

void main()
{
    VertPass();

    vs_out.FragPos = vec3(Model * vec4(VertexPosition, 1.0));
	gl_Position = PVM * vec4(VertexPosition, 1.0);
}