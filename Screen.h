#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream> 
#include "Cube.h"
#include "Window.h"

class Screen {

public: 

	Screen(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc);
	~Screen(); 

	void render(GLuint shaderProgram, GLuint frameShader); 
	void projection(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pe, float n, float f); 
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil); 

	glm::mat4 SWorld; 
	GLuint quadVAO, quadVBO; 
	GLuint textureColorbuffer; 
	GLuint framebuffer;
	GLuint rbo;
	GLuint textureID;
	GLuint sProjection, sModelview;

	static glm::mat4 Perspective; 
	glm::mat4 SPerspective; 
};

const GLfloat screenVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
									 // Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};