layout (location = 0) out vec4 outColor;

in vec2 fsTexCoords;
in vec3 fsColor;

// In linear space
uniform sampler2D fsTextureColor;

void main() {
    vec3 color = texture(fsTextureColor, fsTexCoords.xy).rgb;
    color = pow(color, vec3(1.0f / 2.2f));
    outColor = vec4(color, 1.0f);
}