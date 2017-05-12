#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "Skybox.h"
#include "shader.h"
#include "Screen.h"


class Window
{
public:

	//static float down = 0.0f; 

	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback(float direction);
	static void display_callback(int Xcount, int mod);
	static void transCube(float direction, int mod); 


	static glm::mat4 Vleft_eye; 
	static glm::mat4 Vright_eye; 

	//mine
	static void reset(); 

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
