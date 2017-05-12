#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


class Screen
{
public:
	Screen();
	~Screen(); 
	void render(GLuint shaderProgram); 
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	
	
	glm::mat4 SWorld; 


	GLuint textureColorbuffer; 
	GLuint quadVAO, quadVBO;
	GLuint framebuffer;

};

//const GLfloat floorVertices[] = {
//
//	// Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
//	5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
//	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//
//	5.0f,  -0.5f,  5.0f,  2.0f, 0.0f,
//	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
//	5.0f,  -0.5f, -5.0f,  2.0f, 2.0f
//};

const GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
									   // Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};