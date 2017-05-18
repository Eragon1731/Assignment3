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

	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();


	//static GLFWwindow* create_window(int width, int height);
	//static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback(float direction);
	static void display_callback(glm::mat4 headPose,int  mode);
	static void transCube(float direction, int mode); 

	static glm::mat4 Vleft_eye; 
	static glm::mat4 Vright_eye; 
	static bool debugStatus;

	static glm::vec3 eyePose; 

	static float Loffsetx; 
	static float Loffsety;
	static float Loffsetz;

	static float Roffsetx;
	static float Roffsety;
	static float Roffsetz;


	static int xPosL;
	static int yPosL; 
	static int xPosR;
	static int yPosR;

	//mine
	static void reset(); 

	static GLuint tempfbo; 

	
};

#endif
