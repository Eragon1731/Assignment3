#include "Screen.h"

Cube * trial; 

Screen::Screen(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc) {

	SWorld = glm::mat4(1.0f); 
	SWorld = SWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f,0.05f,0.05f)); 
	trial = new Cube(); 
	std::vector <const GLchar *> faces;

	for (int i = 0; i < 6; i++) {
		faces.push_back("H:/CSE167StarterCode2-master/textures/vr_test_pattern.ppm");
	}

	trial->loadCubemap(faces);

	projection(pa ,pb ,pc , glm::vec3(0.0f, 0.0f, 100.0f), 0.1f, 1000.0f);

	// Setup screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Framebuffers

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	 textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//SAME SIZE 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 640, 480); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Screen::~Screen()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteFramebuffers(1, &framebuffer);
}

void Screen::render(GLuint shaderProgram, GLuint frameShader) {

	// First pass
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	glEnable(GL_DEPTH_TEST);

	trial->update(); 
	trial->draw(shaderProgram);

	// Second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(frameShader); 
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}

//FOR NOW: manually calculate 
void Screen::projection(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pe, float n, float f) {

	glm::vec3 va, vb, vc; 
	glm::vec3 vr, vu, vn; 

	float l, r, b, t, d;

	vr = pb - pa;
	vu = pc - pa; 

	vn = glm::normalize(glm::cross(glm::normalize(vr), glm::normalize(vu))); 
	
	va = pa - pe; 
	vb = pb - pe;
	vc = pc - pe;

	d = -1.0f * glm::dot(va, vn); 

	l = glm::dot(vr, va) * n / d;
	r = glm::dot(vr, vb) * n / d;
	b = glm::dot(vu, va) * n / d;
	t = glm::dot(vu, vc) * n / d;


	glm::mat4 tempP = glm::mat4(1.0f); 

	tempP = glm::frustum(l,r,b,t,n,f); 

	//set matrix
	SPerspective[0].x = vr[0]; SPerspective[1].x = vr[1]; SPerspective[2].x = vr[2];
	SPerspective[0].y = vu[0]; SPerspective[1].y = vu[1]; SPerspective[2].y = vu[2];
	SPerspective[0].z = vn[0]; SPerspective[1].z = vn[1]; SPerspective[2].z = vn[2];


	Perspective = tempP * SPerspective * glm::translate(glm::mat4(1.0f), -pe);
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint Screen::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
