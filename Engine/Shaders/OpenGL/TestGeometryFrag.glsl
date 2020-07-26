layout (location = 0) out vec4 outColor;

in vec3 fsColor;

void main() {
    outColor = vec4(fsColor, 1.0f);
}