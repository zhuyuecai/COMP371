//HW1 Comp 371 summer 2016
//Lab 1
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include "glm.hpp"
//#include "matrix.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "Model.h"
#include "Shader.h" //include the shader class
#include "Camera.h"
#include "objloader.hpp"//model loader
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "SOIL.h" //include soil
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
// Is called whenever a key is pressed/released via GLFW
Camera myCamera = Camera();
bool firstMouse = true;
bool keys[1024];
GLenum mode = GL_TRIANGLES;
float scale_1 = 1.0f;
float scale_2 = 1.0f;
float scaleChange = 0.01f;
float rotateChange = 10.0f;
float rot = 0.0f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, int key, int scancode, int action) {
	std::cout << key << std::endl;
	if (key == GLFW_MOUSE_BUTTON_RIGHT )
	{
		myCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	else if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		myCamera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
}

void Do_Movement()
{
	
	if (keys[GLFW_KEY_UP])
		myCamera.ProcessKeyboard(Camera_Movement::FORWARD,deltaTime);
	if (keys[GLFW_KEY_DOWN])
		myCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT])
		myCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT])
		myCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	if (keys[GLFW_KEY_L])
		mode = GL_LINE_LOOP;
	if (keys[GLFW_KEY_T])
		mode = GL_TRIANGLES;
	if (keys[GLFW_KEY_P])
		mode = GL_TRIANGLE_STRIP;
	if (keys[GLFW_KEY_S] && keys[GLFW_KEY_LEFT_SHIFT])
		scale_1 += scaleChange;
	if (keys[GLFW_KEY_R] && keys[GLFW_KEY_LEFT_SHIFT])
		scale_2 += scaleChange;
	if (keys[GLFW_KEY_S] && !keys[GLFW_KEY_LEFT_SHIFT])
		scale_1 -= scaleChange;
	if (keys[GLFW_KEY_R] && !keys[GLFW_KEY_LEFT_SHIFT])
		scale_2 -= scaleChange;
	if (keys[GLFW_KEY_N])
		rot += rotateChange;
	if (keys[GLFW_KEY_M])
		rot -= rotateChange;



}
// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	// Build and compile our shader program
	// Vertex shader

	// Read the Vertex Shader code from the file

	//-----------------------------------------------------------------
	//pattern to read shader source code into the shader string
	GLchar* vertex_shader_path = "vertex.shader";
	GLchar* fragment_shader_path = "fragment.shader";
	Shader ourShader(vertex_shader_path, fragment_shader_path);
	//start using the program
	ourShader.use();
	//shader part is finished
	//-----------------------------------------------------------------------------------------
	//

	// Read our .obj file

	Model model1 = Model("cube.obj");
	Model model2 = Model("cube.obj");

	//texture

	/*

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	int txwidth, txheight;
	unsigned char* image = SOIL_load_image("test.jpg", &txwidth, &txheight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txwidth, txheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	glBindTexture(GL_TEXTURE_2D, 0);
    */
	//Making the link between vertex data and shader program attributes
	//----------------------------------------------------------------------------
	GLint posAttrib = glGetAttribLocation(ourShader.program, "position");
	
	glEnableVertexAttribArray(posAttrib);
	GLint colAttrib = glGetAttribLocation(ourShader.program, "color");
	glEnableVertexAttribArray(colAttrib);

	GLint txcolAttrib = glGetAttribLocation(ourShader.program, "texCoord");

	glEnableVertexAttribArray(txcolAttrib);
	//--------------------------------------------------------------------------------------
// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	// Load models
	glEnable(GL_DEPTH_TEST);
	// Game loop

	//start the clock
	//auto t_start = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;

		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glfwPollEvents();
		Do_Movement();


	
		glm::mat4 view;
		view = myCamera.GetViewMatrix();
		//define camera view

		GLint uniView = glGetUniformLocation(ourShader.program, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		//set up perspective
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		GLint uniProj = glGetUniformLocation(ourShader.program, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));


		// Render
		// Clear the colorbuffer

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		glm::mat4 trans;
		
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, rot, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(scale_1, scale_1, scale_1));
		trans = glm::scale(trans, glm::vec3(0.8, 0.8, 0.8));
		GLuint modelLoc = glGetUniformLocation(ourShader.program, "model");
	    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		

		GLint uniColor = glGetUniformLocation(ourShader.program, "ourColor");
		glUniform3f(uniColor, 1.0f,0.0f,0.0f);
		//glBindTexture(GL_TEXTURE_2D, texture);

		model1.Draw(ourShader.program, mode);


		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		glm::mat4 trans1;
		
		trans1 = glm::rotate(trans1, (GLfloat)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		trans1 = glm::translate(trans1, glm::vec3(3.0f, 0.0f, 0.0f));
		trans1 = glm::rotate(trans1, rot, glm::vec3(0.0f, 0.0f, 1.0f));
		trans1 = glm::scale(trans1, glm::vec3(scale_2, scale_2, scale_2));
		trans1 = glm::scale(trans1, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans1));


		
		glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);
		//glBindTexture(GL_TEXTURE_2D, texture);

		model2.Draw(ourShader.program, mode);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
