//opoengl

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//img loading
#include "stb_image.h"


//imgui
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//logging
#include <spdlog/spdlog.h>
//math
#include <glm/gtc/matrix_transform.hpp>

#include "include/Shader.h"

//mesh
#include "mesh/Sphere.h";

#include "Window.h"
//resize window
void framebuff_size_callback(GLFWwindow* window, int width, int height);

//I/O handling
void processInput(GLFWwindow* window);

static float mixTex = 0.0f;
float positionZ = -3.0f;

int main() {
	uint16_t screen_width	= 1200;
	uint16_t screen_height	= 720;


	Window window(screen_width, screen_height, u8"中国天然橡胶1个");
	GLFWwindow* glfwWindow = window.getGLFWwindow();
	framebuff_size_callback(window.getGLFWwindow(), screen_width, screen_height);


	/*
		shaders
	*/
	Shader shader("src/shaders/vertex_core.glsl", "src/shaders/fragment_core.glsl");
	Shader shader2("src/shaders/vertex_core.glsl", "src/shaders/fragment_core2.glsl");
	
	
	//Cube
	
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//multiple cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
	};

	//IMGUI INIT
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//colors
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);*/
	
	//textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	unsigned int texture1, texture2;
	
	//bind textures
	glGenTextures(1, &texture1);

	glBindTexture(GL_TEXTURE_2D, texture1);
	//image wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//S = X, T = Y, R = Z
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//load image		channels = rbg, argb, r
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/image1.png", &width, &height, &nChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		spdlog::error("Failed to load texture");
	}
	stbi_image_free(data);
	
	
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load("assets/image2.jpg", &width, &height, &nChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		spdlog::error("Failed to load texture");
	}
	stbi_image_free(data);

	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);



	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	ImVec2 win1 = {300.0f, 150.0f};

	/*
		Camera	
	*/
	//camera										fov				aspect ratio						near	far frustum ratio
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	
	//camera position
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	//camera direction y
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	//right axis x
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	//up axis y
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	
	
	//sphere
	Sphere sphere(15);

	
	while (!glfwWindowShouldClose(window.getGLFWwindow())) {
		glfwSwapBuffers(window.getGLFWwindow());
		glfwPollEvents();
		//background color
		glClearColor(0.55f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//input to imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//enables depth testing = no xray cube
		glEnable(GL_DEPTH_TEST);


	
		//draw shape
		shader.activate();
		shader.setFloat("mixTex", mixTex);

		//camera
		glm::mat4 view = glm::mat4(1.0f);
		const float radius = 30.0f;
		float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shader.setMat4("view", view);

		glBindVertexArray(VAO); 
		for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 2 == 0) {
				angle = 1000.0f * glfwGetTime();
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//shader.activate();
		//draw this sphere at pos 0
		glm::mat4 sphere_model = glm::mat4(1.0f);
		const float radiusS = 15.0f;
		float camXS = static_cast<float>(sin(glfwGetTime()) * radiusS);
		float camZS = static_cast<float>(cos(glfwGetTime()) * radiusS);
		sphere_model = glm::translate(sphere_model, glm::vec3(camXS, camXS, -camZS));
		shader.setMat4("model", sphere_model);
		sphere.draw(shader.id);






		// render your GUI
		ImGui::Begin("Properties");
		ImGui::SetWindowSize(win1, 0);
		ImGui::SliderFloat("texture mix", &mixTex, 0, 1 );
		ImGui::SliderFloat("position", &positionZ, -50, 50);
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		processInput(window.getGLFWwindow());
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixTex += 0.1f;
		if (mixTex > 1.0f) {
			mixTex = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixTex -= 0.1f;
		if (mixTex < 0.0f) {
			mixTex = 0.0f;
		}
	
	}

}

void framebuff_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
