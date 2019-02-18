#version 410

layout (location = 0) VertexPosition;
layout (location = 1) VertexNormal;

out VS_OUT
{
    vec3 WorldPosition;
    vec3 WorldNormal;
}
vs_out;

uniform mat4 ModelView;
uniform mat4 MVP;

void main()
{
    vs_out.WorldPosition = ModelView * vec4(VertexPosition.xyz, 1.0);
    vs_out.WorldNormal = ModelView * vec4(VertexNormal.xyz, 0.0);

	gl_Position = MVP * vec4(VertexPosition.xyz, 1.0);
}
