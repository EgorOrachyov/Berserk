layout (location = 0) in vec2 vsPosition;
layout (location = 1) in vec2 vsTextCoords;

out vec2 fsTextCoords;

void main() {
    fsTextCoords = vsTextCoords;
    gl_Position = vec4(vsPosition, 0.0f, 1.0f);
}