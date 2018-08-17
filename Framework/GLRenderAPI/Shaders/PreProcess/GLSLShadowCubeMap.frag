#version 410

in vec4 FragPos;

uniform vec4 LightPos;
uniform float FarPlane;

void main()
{
	float lightDistance = length(FragPos.xyz - LightPos.xyz);
	lightDistance = lightDistance / FarPlane;
	gl_FragDepth = lightDistance;
}
