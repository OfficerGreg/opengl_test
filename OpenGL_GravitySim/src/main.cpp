#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "include/Shader.h"

//resize window
void framebuff_size_callback(GLFWwindow* window, int width, int height);

//I/O handling
void processInput(GLFWwindow* window);


int main() {
	int success;
	char infoLog[512];

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//init glad (rendering window)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	//resizing
	glfwSetFramebufferSizeCallback(window, framebuff_size_callback);

	/*
		shaders
	*/
	Shader shader("src/shaders/vertex_core.glsl", "src/shaders/fragment_core.glsl");
	Shader shader2("src/shaders/vertex_core.glsl", "src/shaders/fragment_core2.glsl");
	

	float vertices[] = {
		//positions				colors
		-0.25f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.15f,  0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,   0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f,  -0.4f, 0.0f,		1.0f, 0.2f, 1.0f

	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 1, 2
	};

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO 
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW = change every frame

	//set attributes pointers


	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLuint)));
	glEnableVertexAttribArray(1);


	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//matrix init
	glm::mat4 trans		= glm::mat4(1.0f);
	glm::mat4 trans2	= glm::mat4(1.0f);
	//transform 45 degrees z axis
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	
	trans2 = glm::scale(trans, glm::vec3(1.2f));
	//trans2 = glm::rotate(trans, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//give to shader
	shader.activate();
	shader.setMat4("transform", trans);
	shader2.activate();
	shader2.setMat4("transform", trans2);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
		//background color
		glClearColor(0.55f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		
		
		//draw shape
		glBindVertexArray(VAO);
		//first triangl
		shader.activate();
		shader.setMat4("transform", trans);
		//glDrawArrays(GL_LINE_STRIP, 0, 6);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		//second triangle
		//glUseProgram(shaderPrograms[1]);
		shader2.activate();
		shader2.setMat4("transform", trans2);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));


		processInput(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void framebuff_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

}
