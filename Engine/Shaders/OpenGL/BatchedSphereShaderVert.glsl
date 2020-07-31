layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inWorldPos;
layout (location = 2) in vec3 inColor;
layout (location = 3) in float inRadius;

out vec3 fsColor;

layout (std140) uniform Camera {
    mat4 ProjView;
};

void main() {
    fsColor = inColor;
    vec3 pos = inWorldPos + inPos * inRadius;
    gl_Position = ProjView * vec4(pos, 1.0f);
}
