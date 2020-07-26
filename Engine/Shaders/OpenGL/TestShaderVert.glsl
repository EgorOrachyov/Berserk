layout (location = 0) in vec3 vsPosition;
layout (location = 1) in vec2 vsColor;

out vec2 fsColor;

void main() {
    fsColor = vsColor;
    gl_Position = vec4(vsPosition, 1.0f);
}