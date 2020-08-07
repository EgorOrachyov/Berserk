#version 410 core

layout (location = 0) out vec4 outColor;

in vec4 fsColor;
in vec2 fsTexCoords;

uniform sampler2D Bitmap;

void main() {
    vec3 base = texture(Bitmap, fsTexCoords.xy).rrr;
    if (base.r == 0) discard;
    vec3 color = base * fsColor.rgb;
    outColor = vec4(color, fsColor.a);
}