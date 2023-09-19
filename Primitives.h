#pragma once

#include <GL/glew.h>

class Primitives
{
public:
	static GLuint quad()
	{
		// position + texture
		GLfloat vertices[] =
		{
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left 
		};

		GLuint indices[] =
		{
			2, 1, 0,  // first Triangle
			2, 0, 3   // second Triangle
		};

		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//Bind VAO
		glBindVertexArray(VAO);
		//Bind VBO, send data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//Bind EBO, send indices 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//Configure Vertex Attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));

		//Data passing and configuration is done 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return VAO;
	}
};
