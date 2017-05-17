#version 330 core


in vec3 TexCoords;
out vec4 color;

uniform samplerCube skycube;

void main()
{
	color = vec4(1.f, 0.f, 0.f, 1.f);

}