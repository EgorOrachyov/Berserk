#version 410 core
#include "graphics_globals.glsl"

layout (location = 0) out vec4 outColor;

in vec2 fsTexCoords;

uniform sampler2D Texture;

layout (std140) uniform TextureInfo {
    vec4 baseColor;
    vec4 transparentColor;
    bool useTransparentColor;
    bool isSRGB;
    bool useAlpha;
};

void main() {
    vec4 color = texture(Texture, fsTexCoords).rgba;

    if (useTransparentColor && color.rgb == transparentColor.rgb) {
        discard;
    }
    if (isSRGB) {
        color = convertSRBGtoLinear(color);
    }

    vec4 multiplied = baseColor * color;

    float alpha = 1.0f;
    if (useAlpha) {
        alpha = multiplied.a;
    }

    outColor = vec4(multiplied.rgb, alpha);
}
