#include "Window.h"


const char* window_title = "GLFW Starter Project";
Cube * cube;
Skybox * skybox;
Skybox * skyboxRight; 

Skybox * skyboxMine; 
 
GLint shaderProgram;
GLint skyShader; 

Screen * leftscreen; 

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.vert"
#define FRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader_1.frag"

#define SKYVERTEX_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.vert"
#define SKYFRAGMENT_SHADER_PATH "H:/Assignment3/Assignment2/Assignment2/shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

glm::mat4 Window::Vleft_eye; 
glm::mat4 Window::Vright_eye;

using namespace std;

void Window::initialize_objects()
{
	cube = new Cube();
	skybox = new Skybox(); 
	skyboxRight = new Skybox(); 
	skyboxMine = new Skybox(); 

	//leftscreen = new Screen(); 

	vector<const GLchar *> faces;

	vector<const GLchar* > leftFaces; 
	vector<const GLchar* > rightFaces;
	vector <const GLchar*> mine; 

	for (int i = 0; i < 6; i++) {
		faces.push_back("H:/Assignment3/Assignment2/textures/vr_test_pattern.ppm");
	}

	//load left eye
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/px.ppm"); 
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/nx.ppm"); 
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/py.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/ny.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/pz.ppm");
	leftFaces.push_back("H:/Assignment3/Assignment2/textures/left-ppm/nz.ppm");

	//load right eye 
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/px.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/nx.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/py.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/ny.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/pz.ppm");
	rightFaces.push_back("H:/Assignment3/Assignment2/textures/right-ppm/nz.ppm");

	//load personal box 
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/px.ppm");
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/nx.ppm");
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/py.ppm");
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/ny.ppm");
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/pz.ppm");
	mine.push_back("H:/Assignment3/Assignment2/textures/360ppm/nz.ppm");

	cube->loadCubemap(faces);
	skybox->loadCubemap(leftFaces);
	skyboxRight->loadCubemap(rightFaces); 
	skyboxMine->loadCubemap(mine); 

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyShader = LoadShaders(SKYVERTEX_SHADER_PATH, SKYFRAGMENT_SHADER_PATH); 
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	//factory->Reset(); 
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback(float direction)
{
	// Call the update function the cube
	cube->update(direction);

}

void Window::transCube(float direction, int mod) {

	if (mod == 0) {
		cube->translateX(direction); 
	}
	 if (mod == 1) {
		cube->translateY(direction);
	}

	 if (mod == 2) {
		 cube->translateZ(direction); 
	 }
}

void Window::display_callback(int Xcount, int mod)
{

	//leftscreen->render(shaderProgram);

	//BOTH
	if (Xcount == 0) {

		cube->draw(shaderProgram);
		if (mod == 0) {
			skybox->draw(skyShader);
		}
		if (mod == 1) {
			skyboxRight->draw(skyShader);
		}
	}

	//only cube
	if (Xcount == 1) {
		if (mod == 0) {
			cube->draw(shaderProgram);
		}
		if (mod == 1) {

			cube->draw(shaderProgram); 
		}
	}

	//only box
	if (Xcount == 2) {
		if (mod == 0) {
			skybox->draw(skyShader);
		}
		if (mod == 1) {
			skyboxRight->draw(skyShader);
		}
	}
	if (Xcount == 3) {
		skyboxMine->draw(skyShader);
	}


}

void Window::reset() {
	cube->reset(); 
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}