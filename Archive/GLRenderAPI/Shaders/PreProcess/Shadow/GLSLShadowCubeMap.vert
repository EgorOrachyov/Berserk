#version 410

layout(location = 0) in vec3 VertexPosition;

uniform mat4 Model;

void main()
{
	gl_Position = Model * vec4(VertexPosition, 1.0);
}
