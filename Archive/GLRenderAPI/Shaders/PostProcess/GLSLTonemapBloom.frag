#version 410

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

// Framebuffers

uniform sampler2d HDRTex;
uniform sampler2d BlurTex1;
uniform sampler2d BlurTex2;

// Curent pass

subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

// Blum and HDR params

uniform float LuminanceThresh = 0.75;
uniform float Exposure = 0.35;
uniform float White = 0.928;
uniform float AverageLuminance;

uniform float PixelOffset[10] = float[](0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
uniform float Weight[10];

// Converting matrices

uniform mat3 rgbTOxyz = mat3(
  0.4124564, 0.2126729, 0.0193339,
  0.3575761, 0.7151522, 0.1191920,
  0.1804375, 0.0721750, 0.9503041
  );

uniform mat3 xyzTOrgb = mat3(
  3.2404542, -0.9692660, 0.0556434,
  -1.5371385, 1.8760108, -0.2040259,
  -0.4985314, 0.0415560, 1.0572252
  );

float luminance(in vec3 color)
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

// Get overlighted parts of hdr
subroutine(RenderPassType)
vec4 pass1()
{
    vec4 val = texture(HDRTex, TexCoords);

    if (luminance(val.rgb) > LuminanceThresh)
    {
        return val;
    }
    else
    {
        return vec4(0.0);
    }
}

// Gaussian blur effect first pass
subroutine(RenderPassType)
vec4 pass2()
{
    float dy = 1.0 / (textureSize(BlurTex1,0)).y;

    vec4 sum = texture(BlurTex1, TexCoords) * Weight[0];
    for( int i = 1; i < 10; i++ )
    {
         sum += texture(BlurTex1, TexCoords + vec2(0.0,PixelOffset[i]) * dy) * Weight[i];
         sum += texture(BlurTex1, TexCoords - vec2(0.0,PixelOffset[i]) * dy) * Weight[i];
    }
    return sum;
}

// Gaussian blur effect second pass
subroutine(RenderPassType)
vec4 pass3()
{
    float dx = 1.0 / (textureSize(BlurTex2,0)).x;

    vec4 sum = texture(BlurTex2, TexCoords) * Weight[0];
    for( int i = 1; i < 10; i++ )
    {
       sum += texture(BlurTex2, TexCoords + vec2(PixelOffset[i],0.0) * dx) * Weight[i];
       sum += texture(BlurTex2, TexCoords - vec2(PixelOffset[i],0.0) * dx) * Weight[i];
    }
    return sum;
}

// Sum toneMapping and Gaussian blur effect
subroutine(RenderPassType)
vec4 pass4()
{
    vec4 color = texture(HDRTex, TexCoords);
    
    vec3 xyzCol = rgbTOxyz * vec3(color);

    float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
    vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

    float L = (Exposure * xyYCol.z) / AverageLuminance;
    L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

    xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
    xyzCol.y = L;
    xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

    vec4 toneMapColor = vec4(xyzTOrgb * xyzCol, 1.0);

    vec4 blurTex = texture(BlurTex1, TexCoords);
    
    return toneMapColor + blurTex;    
}

void main()
{
    FragColor = RenderPass();
}