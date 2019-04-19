#version 410 core

// Suppose we have default frame buffer
layout (location = 0) out vec4 FragmentColor;

in VS_OUT
{
    vec2 ScreenTexCoords;
}
fs_in;

// Glyphs map, which stores all the chars images
uniform sampler2D Texture0;

// Color of the text
uniform vec3 TextColor;

void main()
{
    float alpha = texture(Texture0, fs_in.ScreenTexCoords).r;
    FragmentColor = vec4(TextColor.rgb, alpha);
}