
#version 330 core


layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 sprojection;
uniform mat4 smodelview;

out vec2 TexCoords;

void main()
{
    gl_Position = sprojection * smodelview * vec4(position.x, position.y, 0.0f, 1.0f); 
    TexCoords = texCoords;
}  