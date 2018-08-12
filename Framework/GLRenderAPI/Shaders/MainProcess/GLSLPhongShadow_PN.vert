#version 410

#define DIR_SHADOW_LIGHTS 4
#define SPOT_SHADOW_LIGHTS 4
#define POINT_SHADOW_LIGHTS 4

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 eyeNorm;
out vec3 eyePosition;

out vec4 fpls_dir[DIR_SHADOW_LIGHTS];
out vec4 fpls_spot[SPOT_SHADOW_LIGHTS];
out vec4 fpls_point[POINT_SHADOW_LIGHTS];

uniform mat4 ModelView;
uniform mat4 MVP;

uniform uint NUM_OF_DIR_SHADOW_LIGHTS; // new
uniform uint NUM_OF_SPOT_SHADOW_LIGHTS;
uniform uint NUM_OF_POINT_SHADOW_LIGHTS;

uniform mat4 light_MVP_dir[DIR_SHADOW_LIGHTS]; // new
uniform mat4 light_MVP_spot[DIR_SHADOW_LIGHTS]; // new
uniform mat4 light_MVP_point[DIR_SHADOW_LIGHTS]; // new

void main()
{
    eyeNorm = normalize(vec3((ModelView * vec4(VertexNormal, 0.0))));
    eyePosition = vec3(ModelView * vec4(VertexPosition, 1.0));

    for(int i = 0; i < NUM_OF_DIR_SHADOW_LIGHTS; i++)
    {
        fpls_dir[i] = light_MVP_dir[i] * vec4(eyePosition, 1.0);
    }

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}