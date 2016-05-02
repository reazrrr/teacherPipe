// scale2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


// drawACube.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

GLuint program;
GLint attribute_coord2d;

const char *vs_source =
"#version 410\n"
"layout (location = 0) in vec3 vp;"
"layout(location = 1) in vec3 color;"
"layout(location = 2) in vec2 texCoord; "

"out vec3 vColor;"
"out vec2 TexCoord;"

"void main(void){"
"gl_Position = vec4(vp, 1.0);"
"vColor = color;"
"TexCoord = texCoord;"
"}";

const char *fs_cource =
"#version 410\n"
"in vec3 vColor;"
"in vec2 TexCoord;"
"out vec4 outputColor;"
"uniform sampler2D ourTexture;"
"void main(void){"
"outputColor = texture(ourTexture, TexCoord);"
"}";


int init_resources()
{
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{

		GLint infoLogLength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(vs, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;
		fprintf(stderr, "error in vertex error\n");
		return 0;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fs, 1, &fs_cource, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(fs, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;

		fprintf(stderr, "error in fragment shader\n");
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		fprintf(stderr, "glinkprogram\n");
		return 0;
	}

	const char* attribute_name = "vp";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1)
	{
		fprintf(stderr, "cannot bind attribute");
		return 0;
	}

	return 1;
}



GLuint loadTexture(){
	// Load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	return texture;
}
void onDisplay()
{
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		-1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,     0.0f, 1.0f,   // 0
		-0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,     0.25f, 1.0f,   // 1
		 0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,     0.75f, 1.0f,   // 2
		 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,     1.0f, 1.0f,   // 3
	    -1.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.0f,0.75f,	   //4
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.25f,0.75f,	   //5
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.75f,0.75f,	   //6
		 1.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 1.0f,0.75f,	   //7
		-1.0f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.0f,0.25f,	   //8
		-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.25f,0.25f,	   //9
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.75f,0.25f,	   //10
		 1.0f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	 1.0f,0.25f,	   //11
		-1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.0f, 0.0f,	   //12
		-0.5f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.25f,0.0f,	   //13
	 	 0.5f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,	 0.75f,0.0f,	   //14
		 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,	 1.0f, 0.0f	   //15

	};

	GLuint indices[] = {  // Note that we start from 0!
		 1,5,0,   5,4,0,  
		 2,6,1,   6,5,1,
		 3,7,2,   7,6,2,
		 5,9,4,   9, 8, 4,
		 6,10,5,  10,9,5,
		 7,11,6,  11,10,6,
		 9,13,8,  13,12,8,
		 10,14,9, 14,13,9,
		11,15,10, 15,14,10

	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);



	glBindTexture(GL_TEXTURE_2D, loadTexture());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}

void onDisplayAScale()
{
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		// Positions          // Colors           // Texture Coords
		-1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // 0
		-0.75f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 0.25f, 1.0f,   // 1
		0.75f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 0.75f, 1.0f,   // 2
		1.0f, 1.0f, 0.0f,     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // 3
		-1.0f, 0.75f, 0.0f,    1.0f, 0.0f, 0.0f, 0.0f, 0.75f,	   //4
		-0.75f, 0.75f, 0.0f,    1.0f, 0.0f, 0.0f, 0.25f, 0.75f,	   //5
		0.75f, 0.75f, 0.0f,     1.0f, 0.0f, 0.0f, 0.75f, 0.75f,	   //6
		1.0f, 0.75f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f, 0.75f,	   //7
		-1.0f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f, 0.0f, 0.25f,	   //8
		-0.75f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f, 0.25f, 0.25f,	   //9
		0.75f, -0.75f, 0.0f,    1.0f, 0.0f, 0.0f, 0.75f, 0.25f,	   //10
		1.0f, -0.75f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f, 0.25f,	   //11
		-1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	   //12
		-0.75f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.25f, 0.0f,	   //13
		0.75f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 0.75f, 0.0f,	   //14
		1.0f, -1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f, 0.0f	   //15
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 5, // First Triangle
		5, 4, 0,  // Second Triangle
		1, 2, 6,
		6, 5, 1,
		2, 3, 7,
		7, 6, 2,
		4, 5, 9,
		9, 8, 4,
		5, 6, 10,
		10, 9, 5,
		6, 7, 11,
		11, 10, 6,
		8, 9, 13,
		13, 12, 8,
		9, 10, 14,
		14, 13, 9,
		10, 11, 15,
		15, 14, 10




	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);



	glBindTexture(GL_TEXTURE_2D, loadTexture());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}

void free_resources()
{
	glDeleteProgram(program);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{



	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		//      throw std::runtime_error("glew fails to start.");
		fprintf(stderr, "glew error\n");
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glfwSetKeyCallback(window, key_callback);

	GLint maxV;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
	fprintf(stderr, "maxv: %d\n", maxV);

	init_resources();

	while (!glfwWindowShouldClose(window))
	{
		//onDisplay();
		onDisplayAScale();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	free_resources();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}




