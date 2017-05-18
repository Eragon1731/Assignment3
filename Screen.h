#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream> 
#include "Cube.h"
#include "Skybox.h"
#include "Window.h"

class Screen {

public:

	Screen(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pe);
	~Screen();
	Cube * trial;
	void render(GLuint shaderProgram, GLuint frameShader, GLuint skyShader, int mode);
	glm::mat4 projection(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 e, float n, float f);
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

	GLuint quadVAO, quadVBO;
	GLuint textureColorbuffer;
	GLuint textureColorbufferR;
	GLuint framebuffer;
	GLuint framebufferR;

	GLuint rbo;
	GLuint textureID;
	GLuint sProjection, sModelview;
	void debugMode(glm::mat4 headPose, GLint shaderProgram, int mode );

	glm::mat4 SWorld;
	GLuint uProjection, uModelview;

	glm::vec3 sa, sb, sc;
	static glm::mat4 Projection;
};

const GLfloat screenVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
									 // Positions   // TexCoords
	-1.2f,  1.2f,  1.0f, 1.0f, //0,1 //
	-1.2f, -1.2f,  1.0f, 0.0f, //0,0 //
	1.2f, -1.2f,   0.0f, 0.0f, //1.0 //

	-1.2f,  1.2f,  1.0f, 1.0f, //0,1 //
	1.2f, -1.2f,   0.0f, 0.0f, //1,0 
	1.2f,  1.2f,   0.0f, 1.0f //1,1 //
};