#include "Window.h"

GLint shaderProgram;
GLint skyShader; 
GLint fbshader; 

Screen * leftscreen; 
Screen * rightscreen; 

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.vert"
#define FRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.frag"

#define SKYVERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.vert"
#define SKYFRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.frag"

#define SVERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/fbshader.vert"
#define SFRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/fbshader.frag"

glm::mat4 Window::P;
glm::mat4 Window::V;

using namespace std;

void Window::initialize_objects()
{

	leftscreen = new Screen(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(20.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 10.0f, 0.0f));
	
	rightscreen = new Screen(glm::vec3(20.0f,0.0f,0.0f),
							 glm::vec3(40.0f,0.0f,0.0f),
							 glm::vec3(20.0f,10.0f,0.0f)); 

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyShader = LoadShaders(SKYVERTEX_SHADER_PATH, SKYFRAGMENT_SHADER_PATH); 
	fbshader = LoadShaders(SVERTEX_SHADER_PATH, SFRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyShader);
	glDeleteProgram(fbshader); 
}


void Window::idle_callback(float direction)
{
	// Call the update function the cube
	leftscreen->trial->update(direction);

}

void Window::transCube(float direction, int mode) {

	if (mode == 0) {
		leftscreen->trial->translateX(direction);
	}
	 if (mode == 1) {
		 leftscreen->trial->translateY(direction);
	}
	 if (mode == 2) {
		 leftscreen->trial->translateZ(direction); 
	 }
}

void Window::display_callback(int Xcount, int mode)
{
	if (mode == 0) {
		leftscreen->render(shaderProgram, fbshader, skyShader);
	}
	if (mode == 1) {
		leftscreen->render(shaderProgram, fbshader, skyShader);
		//cerr << "in" << endl;
	}
}

void Window::reset() {
	leftscreen->trial->reset(); 
}

