#version 410

in vec2 TexCoords;
layout(location = 0) out vec4 FragColor;

uniform sampler2D HDRTex;
uniform sampler2D BlurTex1;
uniform sampler2D BlurTex2;

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

uniform float LuminanceThresh = 0.75;
uniform float PixelOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

float luminance(in vec3 color)
{
    return dot(vec3(0.2126,0.7152,0.0722), color);
}

subroutine(RenderPassType)
vec4 pass1()
{
    vec4 val = texture(HDRTex, TexCoords);

    if (luminance(val.rgb) >= LuminanceThresh)
    {
        return val;
    }
    else
    {
        return vec4(0.0);
    }
}

subroutine(RenderPassType)
vec4 pass2()
{
    float dy = 1.0 / (textureSize(BlurTex1,0)).y;

    vec4 sum = texture(BlurTex1, TexCoords) * Weight[0];
    for( int i = 1; i < 5; i++ )
    {
         sum += texture(BlurTex1, TexCoords + vec2(0.0,dy * i)) * Weight[i];
         sum += texture(BlurTex1, TexCoords - vec2(0.0,dy * i)) * Weight[i];
    }
    return sum;
}

subroutine(RenderPassType)
vec4 pass3()
{
    float dx = 1.0 / (textureSize(BlurTex2,0)).x;

    vec4 sum = texture(BlurTex2, TexCoords) * Weight[0];
    for( int i = 1; i < 5; i++ )
    {
       sum += texture(BlurTex2, TexCoords + vec2(PixelOffset[i],0.0) * dx) * Weight[i];
       sum += texture(BlurTex2, TexCoords - vec2(PixelOffset[i],0.0) * dx) * Weight[i];
    }
    return sum;
}

subroutine(RenderPassType)
vec4 pass4()
{
    vec4 toneMapColor = texture(HDRTex, TexCoords);
    vec4 blurTex = texture(BlurTex1, TexCoords);

    return toneMapColor + blurTex;
}

void main()
{
    FragColor = RenderPass();
}