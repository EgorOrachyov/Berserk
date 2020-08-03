#version 410 core

layout (location = 0) out vec4 outColor;

//in vec4 fsColor;
in vec2 fsTexCoords;

// Actual texture image to draw
uniform sampler2D Texture;

void main() {
    // Sample actual color
    vec4 color = texture(Texture, fsTexCoords).rgba;
    // Multiply by 'general' color (usually it is white)
    //outColor = color * fsColor;
    outColor = color;
}
