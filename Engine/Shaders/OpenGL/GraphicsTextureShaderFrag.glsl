#version 410 core
#include "GraphicsGlobalsInc.glsl"

layout (location = 0) out vec4 outColor;

//in vec4 fsColor;
in vec2 fsTexCoords;

// Actual texture image to draw
uniform sampler2D Texture;

layout (std140) uniform TextureInfo {
    vec4 baseColor;
    vec4 transparentColor;
    bool useTransparentColor;
    bool isSRGB;
};

void main() {
    // Sample actual color
    vec4 color = texture(Texture, fsTexCoords).rgba;
    // Check to discard transparent
    if (useTransparentColor && color.rgb == transparentColor.rgb) {
        discard;
    }
    // Convert to linear if needed
    if (isSRGB) {
        color = convertSRBGtoLinear(color);
    }
    // Multiply by 'general' color (usually it is white)
    vec4 multiplied = baseColor * color;
    // Blend automatically
    outColor = multiplied;
}
