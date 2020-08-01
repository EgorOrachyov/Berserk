// Eval gradient color
// param[in] pixelCoords Screen coordinates of the pixel
vec4 getColor(in vec3 pixelCoords) {
    return vec4(pixelCoords, 1.0f);
}