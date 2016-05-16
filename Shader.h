//LINK FOR SOURCE CODE :http://learnopengl.com/#!Getting-started/Shaders
//header for shader class
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>

class Shader
{
public:
	// The program ID
	GLuint program;
	
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Use the program
	void use();
	//detacth the instance shaders  
	//void detacth();
};

#endif

