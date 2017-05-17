#include "Window.h"

GLint shaderProgram;
GLint skyShader; 
GLint fbshader; 
GLint lineShader;

Screen * leftscreen; 
Screen * rightscreen; 
Screen * floorscreen; 

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.vert"
#define FRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.frag"

#define SKYVERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.vert"
#define SKYFRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.frag"

#define SVERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/fbshader.vert"
#define SFRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/fbshader.frag"

#define LINEFRAG_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/tempShader.frag"
#define LINEVERT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/tempShader.vert"


glm::mat4 Window::P;
glm::mat4 Window::V;
int Window::width;
int Window::height; 

glm::vec3 Window::eyePose;
bool Window::debugStatus;
using namespace std;

void Window::initialize_objects()
{

	leftscreen = new Screen(glm::vec3(-1.2f, -1.2f, 0.0f),
		glm::vec3(1.2f, -1.2f, 0.0f),
		glm::vec3(-1.2f, 1.20f, 0.0f));

	leftscreen->SWorld = leftscreen->SWorld * glm::translate(glm::mat4(1.0f), glm::vec3(-0.845f, -0.0f, -2.2f));
	leftscreen->SWorld = leftscreen->SWorld * glm::rotate(glm::mat4(1.0f), 0.785398f, glm::vec3(0.0f, 1.0f, 0.0f));

	rightscreen = new Screen(glm::vec3(-1.2f, -1.2f, 0.0f),
		glm::vec3(1.2f, -1.2f, 0.0f),
		glm::vec3(-1.2f, 1.20f, 0.0f));

	rightscreen->SWorld = rightscreen->SWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.845f, 0.0f, -2.2f));
	rightscreen->SWorld = rightscreen->SWorld * glm::rotate(glm::mat4(1.0f), -1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	
	floorscreen = new Screen(glm::vec3(-1.2f, -1.2f, 0.0f),
		glm::vec3(1.2f, -1.2f, 0.0f),
		glm::vec3(-1.2f, 1.20f, 0.0f));

	floorscreen->SWorld = floorscreen->SWorld * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.2f, -1.355f));
	floorscreen->SWorld = floorscreen->SWorld * glm::rotate(glm::mat4(1.0f), -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
	floorscreen->SWorld = floorscreen->SWorld * glm::rotate(glm::mat4(1.0f), 0.785398f +1.5708f, glm::vec3(0.0f, 0.0f, 1.0f));



	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyShader = LoadShaders(SKYVERTEX_SHADER_PATH, SKYFRAGMENT_SHADER_PATH); 
	fbshader = LoadShaders(SVERTEX_SHADER_PATH, SFRAGMENT_SHADER_PATH);
	lineShader = LoadShaders(LINEVERT_SHADER_PATH, LINEFRAG_SHADER_PATH);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyShader);
	glDeleteProgram(fbshader); 
	glDeleteProgram(lineShader);
}


void Window::idle_callback(float direction)
{
	// Call the update function the cube
	leftscreen->trial->update(direction);
	rightscreen->trial->update(direction);
	floorscreen->trial->update(direction);

}

void Window::transCube(float direction, int mode) {

	if (mode == 0) {
		leftscreen->trial->translateX(direction);
		rightscreen->trial->translateX(direction);
		floorscreen->trial->translateX(direction);
	}
	 if (mode == 1) {
		 leftscreen->trial->translateY(direction);
		 rightscreen->trial->translateY(direction);
		 floorscreen->trial->translateY(direction);
	}
	 if (mode == 2) {
		 leftscreen->trial->translateZ(direction); 
		 rightscreen->trial->translateZ(direction);
		 floorscreen->trial->translateZ(direction);
	 }
}

void Window::display_callback(glm::mat4 headPose, int mode)
{
		leftscreen->render(shaderProgram, fbshader, skyShader, mode);
	
		//rightscreen->render(shaderProgram, fbshader, skyShader, mode);
		floorscreen->render(shaderProgram, fbshader, skyShader, mode);
		

			if (debugStatus) {
				leftscreen->debugMode(headPose , lineShader,  mode);
				rightscreen->debugMode(headPose, lineShader,  mode);
				floorscreen->debugMode(headPose, lineShader,  mode);
			}


}

void Window::reset() {
	leftscreen->trial->reset(); 
	rightscreen->trial->reset();
	floorscreen->trial->reset();
}

