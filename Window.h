#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>


#include <GL/glew.h>
//#endif
#include "shader.h"
#include "Screen.h"


class Window
{
public:

	//static float down = 0.0f; 

	//static int width;
	//static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	//static GLFWwindow* create_window(int width, int height);
	//static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback(float direction);
	static void display_callback(int Xcount, int mode);
	static void transCube(float direction, int mode); 

	static glm::mat4 Vleft_eye; 
	static glm::mat4 Vright_eye; 

	//mine
	static void reset(); 

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
