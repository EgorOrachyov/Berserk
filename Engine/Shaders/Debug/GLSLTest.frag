#version 410

in VS_OUT
{
    vec3 WorldPosition;
    vec3 WorldNormal;
}
fs_in;

void main()
{
    const vec4 Color = vec4(0.3, 0.3, 0.3, 1.0);
    const vec3 LightPosition = vec3(3.0, 2.0, 1.0);

    const vec3 s = normalize(LightPosition - WorldPosition);

	gl_FragColor = max(dot(s, WorldNormal), 0.0) * Color;
}
