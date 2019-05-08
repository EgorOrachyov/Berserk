#version 410 core

in vec2 ScreenTexCoords;

// Suppose we have default frame buffer
layout (location = 0) out vec4 FragmentColor;

// Glyphs map, which stores all the chars images
uniform sampler2D Texture0;

// Color of the text
uniform vec3 TextColor;

void main()
{
    float alpha = texture(Texture0, ScreenTexCoords).r;
    FragmentColor = vec4(TextColor.rgb, alpha);
}