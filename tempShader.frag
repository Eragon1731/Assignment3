#version 330 core


in vec3 colorval;
out vec4 color;

uniform samplerCube skycube;

void main()
{
	color = vec4(colorval, 1.f);

}