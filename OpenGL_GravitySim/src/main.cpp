﻿//opoengl
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

//resize window
void framebuff_size_callback(GLFWwindow* window, int width, int height);

//I/O handling
void processInput(GLFWwindow* window);

static float mixTex = 0.0f;
float positionZ = -3.0f;

int main() {
	uint16_t screen_width	= 1200;
	uint16_t screen_height	= 720;


	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	std::cout << glfwGetVersionString() << std::endl;

	//create window
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, u8"中国天然橡胶1个", NULL, NULL);
	if (window == NULL) {
		spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	else {
		spdlog::info("Successfully created glfw window!");
	}
	glfwMakeContextCurrent(window);

	//init glad (rendering window)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		spdlog::error("Failed to initialize GLAD");
		return -1;
	}
	else {
		spdlog::info("Successfully initialized GLAD!");
	}

	glViewport(0, 0, screen_width, screen_height);
	//resizing
	glfwSetFramebufferSizeCallback(window, framebuff_size_callback);


	//set window icon
	int icon_width, icon_height, icon_channels;

	unsigned char* icon_data = stbi_load("assets/image1.png", &icon_width, &icon_height, &icon_channels, 4);
	if (icon_data) {
		spdlog::info("icon loaded successfully!");
	}
	else {
		spdlog::error("icon could not be loaded");
	}


	GLFWimage icons[1];
	icons[0].width	= icon_width;
	icons[0].height = icon_height;
	icons[0].pixels = icon_data;

	glfwSetWindowIcon(window, 1, icons);
	stbi_image_free(icon_data);

	/*
		shaders
	*/
	Shader shader("src/shaders/vertex_core.glsl", "src/shaders/fragment_core.glsl");
	Shader shader2("src/shaders/vertex_core.glsl", "src/shaders/fragment_core2.glsl");
	

	//float vertices[] = {
	//	//positions					colors				texture coordinates
	//	 -0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	//	 -0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
	//	 0.5f,  -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
	//	 0.5f,   0.5f, 0.0f,		1.0f, 0.2f, 1.0f,	1.0f, 1.0f	

	//};
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

	//IMGUI INIT
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
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

	//matrix init
	//glm::mat4 trans		= glm::mat4(1.0f);
	//transform 45 degrees z axis
	//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//
	//trans2 = glm::scale(trans, glm::vec3(1.2f));
	////trans2 = glm::rotate(trans, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	////give to shader
	//shader.activate();
	//shader.setMat4("transform", trans);
	//shader2.activate();
	//shader2.setMat4("transform", trans2);

	ImVec2 win1 = {300.0f, 150.0f};

	//camera										fov				aspect ratio						near	far frustum ratio
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	int modelLoc = glGetUniformLocation(shader.id, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	int projectionLoc = glGetUniformLocation(shader.id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	int viewLoc = glGetUniformLocation(shader.id, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
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


		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 10.0f), glm::vec3(12.0f, 12.0f, 12.0f));
		//trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) / 1000, glm::vec3(0.5f, 1.0f, 0.0f));
		
		//draw shape
		glBindVertexArray(VAO); 
		//first triangle
		shader.activate();
		shader.setMat4("model", model);
		//glDrawArrays(GL_LINE_STRIP, 0, 6);


		shader.setFloat("mixTex", mixTex);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// render your GUI
		ImGui::Begin("Properties");
		ImGui::SetWindowSize(win1, 0);
		ImGui::SliderFloat("texture mix", &mixTex, -2, 2 );
		ImGui::SliderFloat("position", &positionZ, -50, 50);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, positionZ));
		ImGui::End();



		//trans = glm::translate(trans, glm::vec3(translation[0], translation[1], 0.0f));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//second triangle
		//glUseProgram(shaderPrograms[1]);
		//shader2.activate();
		//shader2.setMat4("transform", trans2);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));


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
