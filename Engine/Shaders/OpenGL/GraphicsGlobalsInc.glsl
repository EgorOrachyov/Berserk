#define COLOR_GAMMA 2.2

vec4 convertSRBGtoLinear(in vec4 color) {
    float alpha = color.a;
    vec3 converted = pow(color.rgb,vec3(COLOR_GAMMA));
    return vec4(converted,alpha);
}