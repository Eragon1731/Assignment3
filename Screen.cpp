#include "Screen.h"
#include "Line.h"
Cube * trial; 
Skybox * leftSky;
Skybox * rightSky; 

GLuint Window::tempfbo; 
using namespace std; 
using namespace glm;  
Screen::Screen(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc) {

	SWorld = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 20.0f, 20.0f)); 
	trial = new Cube(); 
	leftSky = new Skybox(); 
	rightSky = new Skybox(); 

	vector <const GLchar *> faces;
	vector <const GLchar *> leftFaces;
	vector <const GLchar*> rightFaces; 

	for (int i = 0; i < 6; i++) {
		faces.push_back("H:/Assignment3/Assignment2/textures/vr_test_pattern.ppm");
	}

	trial->loadCubemap(faces);

	//load left eye
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/px.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/nx.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/py.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/ny.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/pz.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/nz.ppm");

	leftSky->loadCubemap(leftFaces); 

	//load right eye
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/px.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/nx.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/py.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/ny.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/pz.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/nz.ppm");

	rightSky->loadCubemap(rightFaces); 

	//resize
	leftSky->skyWorld = leftSky->skyWorld * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f));
	rightSky->skyWorld = rightSky->skyWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));

	sa = pa;
	sb = pb;
	sc = pc; 

	//////////////////////////////////////////////////////////////////////////////


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
	
	///////////////////////////////
	////second framebuffer for stereo
	//glGenFramebuffers(1, &framebufferR);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebufferR);
	//// Create a color attachment texture
	//textureColorbufferR = generateAttachmentTexture(false, false);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbufferR, 0);
	///////////////////////////////


	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//SAME SIZE 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::width, Window::height); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
						
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorbuffer, 0);
																						  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
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
	delete(trial); 
	delete(leftSky); 
}

void Screen::render(GLuint shaderProgram, GLuint frameShader, GLuint skyShader, int mode) {

	//eye pose
	glm::vec3 d = -1.0f * glm::vec3(Window::V[3]); 
	glm::vec3 pos = d * glm::mat3(Window::V); 
	Screen::Projection = projection(sa, sb, sc, pos, 0.1f, 1000.0f);

	if (mode == 0) {
		//// First pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glViewport(0, 0, Window::width, Window::height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		glEnable(GL_DEPTH_TEST);
	
		
			//trial->update(); 
			trial->draw(shaderProgram);

			leftSky->draw(skyShader);

			glBindFramebuffer(GL_FRAMEBUFFER, Window::tempfbo); // back to default
		}
	if (mode == 1) {
		///////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferR);
		//glViewport(0, 0, Window::width, Window::height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		glEnable(GL_DEPTH_TEST);

		//trial->update(); 
		trial->draw(shaderProgram);
		rightSky->draw(skyShader);

		glBindFramebuffer(GL_FRAMEBUFFER, Window::tempfbo); // back to default
	}
		glDisable(GL_DEPTH_TEST);
	//////////////
	glUseProgram(frameShader);

	glm::mat4 modelview = Window::V * SWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(frameShader, "sprojection");
	uModelview = glGetUniformLocation(frameShader, "smodelview");
	// Now send these values to the shader program

	//////////////////////////////
	//NOTE:: changed the perspective to take in P calculated from Screen
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);


	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0); // diff 
	glUniform1i(glGetUniformLocation(frameShader, "screenTexture"), 0);//diff
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//cerr << "frame: " << framebuffer << endl; 
}

glm::mat4 Screen::projection(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pe, float n, float f) {

	glm::vec3 va, vb, vc; 
	glm::vec3 vr, vu, vn; 
	glm::mat4 SPerspective = glm::mat4(1.0f); 

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
	//SPerspective[0].x = vr[0]; SPerspective[1].x = vu[1]; SPerspective[2].x = vn[2];
	//SPerspective[0].y = vr[0]; SPerspective[1].y = vu[1]; SPerspective[2].y = vn[2];
	//SPerspective[0].z = vr[0]; SPerspective[1].z = vu[1]; SPerspective[2].z = vn[2];
	
	//cerr << "15: " << SPerspective[3].w << endl;

	SPerspective= glm::mat4( glm::vec4(vr, 0.0f), glm::vec4(vu,0.0f), glm::vec4(vn, 0.0f), glm::vec4(0.0f,0.0f,0.0f,1.0f)); 
	//SPerspective = glm::transpose(SPerspective);
	glm::mat4 temp =  tempP * SPerspective * glm::translate(glm::mat4(1.0f), -1.0f * pe);


	return temp; 
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint Screen::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::width, Window::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return textureID;
}


void Screen::debugMode(glm::mat4 headPose, GLint shaderProgram, int mode) {

	glm::vec3 sd = sb - sa + sc;
	glm::vec3 eyes = glm::vec3(headPose[3]);
	//cerr << "eyes " << eyes.x << " " << eyes.y << " " << eyes.z << endl;
	glm::mat4 temp;
	if (mode == 0) {
		temp = SWorld * glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, 0.0f, 0.0f)); 
	}
	else if (mode == 1) {
		temp = SWorld * glm::translate(glm::mat4(1.0f), glm::vec3(-5.5f, 0.0f, 0.0f));
	}

	vec4 upsd = vec4(sd, 1.0f);
	upsd =  temp  * upsd;

	Line *line1 = new Line(eyes, vec3(upsd));
	line1->draw(shaderProgram, headPose);
	//cerr << "sd: " << sd.x << " " << sd.y << " " << sd.z << endl;

	vec4 upsd2 = vec4(sa, 1.0f);
	upsd2 =  temp * upsd2 ;

	Line *line2 = new Line(eyes, vec3(upsd2));
	line2->draw(shaderProgram, headPose);
	//cerr << "sa: " << sa.x << " " << sa.y << " " << sa.z << endl;

	vec4 upsd3 = vec4(sc, 1.0f);
	upsd3 =  temp * upsd3;

	Line *line3 = new Line(eyes, vec3(upsd3));
	line3->draw(shaderProgram, headPose);
	//cerr << "sc: " << sc.x << " " << sc.y << " " << sc.z << endl;

	vec4 upsd4 = vec4(sb, 1.0f);
	upsd4 =  temp * upsd4 ;

	Line *line4 = new Line(eyes, upsd4);
	line4->draw(shaderProgram, headPose);
	//cerr << "sb: " << sb.x << " " << sb.y << " " << sb.z << endl;

}
