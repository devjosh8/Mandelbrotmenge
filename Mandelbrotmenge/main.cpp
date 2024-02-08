#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"



// Variablen
float x_translation = 0.0f;
float y_translation = 0.0f;

double zoom = 4.0f;


// Zeitberechnungen
float delta_time = 0;
float current_frame = 0;
float last_frame = 0;

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


	GLFWwindow* window = glfwCreateWindow(1280, 720, "Mandelbrotmenge von Josh", NULL, NULL);

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



	GLfloat vertices[] = { -1, -1, 0, // bottom left corner
					  -1,  1, 0, // top left corner
					   1,  1, 0, // top right corner
					   1, -1, 0 }; // bottom right corner

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader shader("vertexShader.txt", "fragmentShader.txt");
	shader.init();
	shader.bind();

	while (!glfwWindowShouldClose(window)) {

		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame; 
		last_frame = current_frame;


		//std::cout << "FPS: " << 1 / delta_time << std::endl;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			y_translation += 1.0f * delta_time * zoom;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			x_translation -= 1.0f * delta_time * zoom;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			y_translation -= 1.0f * delta_time * zoom;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			x_translation += 1.0f * delta_time * zoom;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			zoom -= zoom/3 * delta_time * 2;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			zoom += zoom / 3 * delta_time * 2;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		
		shader.bind();
		glUniform3f(shader.getUniformLocation("a_translation"), x_translation, y_translation, 0.0f);
		glUniform1f(shader.getUniformLocation("a_zoom"), zoom);
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	shader.~Shader();

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