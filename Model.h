// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <glew.h>

// Include GLFW
#include <glfw3.h>


// Include GLM
#include <glm.hpp>
#include <matrix_transform.hpp>
using namespace glm;


#include "objloader.hpp"

class Model{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint VertexArrayID;
	GLuint uvbuffer;
	GLuint vertexbuffer;

	Model(char* path){
		bool res = loadOBJ(path, vertices, uvs, normals);
	}
	void Draw(GLuint programID, GLenum mode){
		
		glGenVertexArrays(1, &this->VertexArrayID);
		glBindVertexArray(this->VertexArrayID);
		
		glGenBuffers(1, &this->vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

		
		glGenBuffers(1, &this->uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

		// Draw the triangle !
		glDrawArrays(mode, 0, this->vertices.size());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	void cleanUp(){
		glDeleteBuffers(1, &this->vertexbuffer);
		glDeleteBuffers(1, &this->uvbuffer);
		glDeleteVertexArrays(1, &this->VertexArrayID);
	}
};