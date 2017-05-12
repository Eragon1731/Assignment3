#version 330 core


in vec3 TexCoords;
out vec4 color;

uniform samplerCube skycube;

void main()
{
	color = texture (skycube, TexCoords);

}