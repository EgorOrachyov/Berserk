layout (location = 0) in vec3 vsPosition;
layout (location = 1) in vec2 vsTexCoords;
layout (location = 2) in vec3 vsInstanceColor;
layout (location = 3) in vec3 vsInstancePosition;
layout (location = 4) in float vsInstanceRadius;

out vec2 fsTexCoords;
out vec3 fsColor;

layout (std140) uniform Transform
{
    mat4 View;
    mat4 Proj;
};

void main() {
    fsTexCoords = vsTexCoords;
    fsColor = vsInstanceColor;
    vec3 position = vsInstancePosition + vsPosition * vsInstanceRadius;
    gl_Position = Proj * View * vec4(position, 1.0f);
}