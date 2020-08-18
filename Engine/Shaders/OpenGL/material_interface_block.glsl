// Material interface block structure, shared among all material shaders
//
// vec3 Position;
// vec3 Normal;
// vec3 Tangent;
// vec3 Bitangent;
// vec2 TexCoords;
//
// Possible usage:
//
// out MaterialInterfaceBlock {
//    vec3 Position;
//    vec3 Normal;
//    vec3 Tangent;
//    vec3 Bitangent;
//    vec2 TexCoords;
// } vs_out;
//
// in MaterialInterfaceBlock {
//    vec3 Position;
//    vec3 Normal;
//    vec3 Tangent;
//    vec3 Bitangent;
//    vec2 TexCoords;
// } fs_in;