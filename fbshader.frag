#version 330 core


in vec2 TexCoords;
//layout(location = 0) out vec3 color;
out vec3 color; 

uniform sampler2D screenTexture;

void main()
{ 
   color = vec3(texture(screenTexture, TexCoords));
//	color = vec4(1.0, 0.0,0.0, 1.0); 
}