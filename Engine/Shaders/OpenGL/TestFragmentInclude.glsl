// Eval gradient color
// param[in] pixelCoords Screen coordinates of the pixel
vec4 getColor(in vec2 pixelCoords) {
    return vec4(pixelCoords, 0.0f, 1.0f);
}
