#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"


// Callbacks

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void keyinput_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {

	if (!glfwInit()) {
		std::cout << "Konnte GLFW nicht initialisieren!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1280, 720, "Mandelbrotmenge", NULL, NULL);

	if (window == NULL) {
		std::cout << "Konnte kein GLFW-Fenster initialisieren!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Konnte GLAD nicht initialisieren!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 1280, 720);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyinput_callback);


	GLfloat vertices[] = { -1, -1, 0, // unten links
					  -1,  1, 0, // oben links
					   1,  1, 0, // oben rechts
					   1, -1, 0 }; // unten rechts

	GLubyte indices[] = { 0,1,2, // erstes dreieck
					 0,2,3 }; // zweites dreieck

	unsigned int vaoID, vboID, eboID;

	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &eboID);

	glBindVertexArray(vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	Shader shader("vertexShader.txt", "fragmentShader.txt");
	shader.bind();

	while (!glfwWindowShouldClose(window)) {

		// Zeug rendern

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



void keyinput_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}