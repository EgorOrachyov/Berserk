layout (location = 0) out vec4 outColor;

in vec2 fsTextCoords;

void main() {
    outColor = getColor(fsTextCoords);
}