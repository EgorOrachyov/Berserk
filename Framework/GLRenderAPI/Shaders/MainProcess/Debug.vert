#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 eyeNorm;
out vec3 eyePosition;
out vec4 lightSpacePos[20];

uniform mat4 Model;
uniform mat4 ModelView;
uniform mat4 MVP;
uniform mat4 LightPV[20];

void main()
{
	eyeNorm = normalize(vec3((ModelView * vec4(VertexNormal, 0.0))));
    eyePosition = vec3(ModelView * vec4(VertexPosition, 1.0));
    lightSpacePos[0] = LightPV[0] * Model * vec4(VertexPosition, 1.0);

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
