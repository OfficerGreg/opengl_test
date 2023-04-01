//opoengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//img loading
#include "stb_image.h"

#include "Gui.h"

//logging
#include <spdlog/spdlog.h>
//math

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"


//mesh
#include "mesh/Sphere.h";

#include "Window.h"
#include "Camera.h"


#define GLFW_WINDOW window.getGLFWwindow()

//resize window
void framebuff_size_callback(GLFWwindow* window, int width, int height);

//I/O handling
void processInput(GLFWwindow* window);

//camera mouse 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


static float mixTex = 0.0f;

uint16_t SCREEN_WIDTH = 1400;
uint16_t SCREEN_HEIGHT = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;


//camera pos
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//tab-out
bool mouse_locked = true;  // flag to keep track of whether the mouse is locked or not
bool tab_pressed_last_frame = false;  // flag to keep track of whether the tab key was pressed last frame or not
static bool center_on_cursor = false;
glm::vec3 center = glm::vec3(1.0f);


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

std::string glsl_version = "#version 330 core";

int main() {
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, u8"中国天然橡胶1个");
	GLFWwindow* glfwWindow = window.getGLFWwindow();
	framebuff_size_callback(window.getGLFWwindow(), SCREEN_WIDTH, SCREEN_HEIGHT);
	//mouse
	glfwSetCursorPosCallback(GLFW_WINDOW, mouse_callback);
	glfwSetScrollCallback(GLFW_WINDOW, scroll_callback);
	glfwSetInputMode(GLFW_WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(GLFW_WINDOW, framebuff_size_callback);
	//init imGui
	Gui gui(GLFW_WINDOW, glsl_version);



	//Shader
	Shader cubeShader("src/shaders/cube_vert.glsl", "src/shaders/cube_frag.glsl");
	Shader lightShader("src/shaders/lighting_vert.glsl", "src/shaders/lighting_frag.glsl");
	
	//Cube
	float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// VAO, VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind VAO 
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_DYNAMIC_DRAW = change every frame

	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	float fov = 45.0f;

	//lighting
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	Sphere sphere(15);

	//disable vsync
	glfwSwapInterval(0);

	ImVec2 win1 = { 300.0f, 150.0f };


	while (!glfwWindowShouldClose(GLFW_WINDOW)) {
		if (glfwGetTime() - lastTime >= 1.0) {
			std::string windowName = u8"中国天然橡胶1个 |  FPS: " + std::to_string(nbFrames) + "     |  Bing Chilling Engine " + u8"\U0001F368";
			glfwSetWindowTitle(window.getGLFWwindow(), windowName.c_str());
		}



		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			spdlog::info("{}", nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}


		//frame logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glEnable(GL_DEPTH_TEST);


		//background color
		glClearColor(0.55f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		gui.beginFrame();


		cubeShader.activate();
		cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		float light_x = static_cast<float>(sin(glfwGetTime()) * 8);
		float light_y = static_cast<float>(sin(glfwGetTime()) * 12);
		float light_z = static_cast<float>(cos(glfwGetTime()) * 5);
		cubeShader.setVec3("lightColor", light_x, light_y, light_z);
		cubeShader.setVec3("lightPos", lightPos);
		cubeShader.setVec3("viewPos", camera.Position);

		//camera
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		cubeShader.setMat4("projection", projection);
		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		cubeShader.setMat4("view", view);

		//float light_x = static_cast<float>(sin(glfwGetTime()) * 8);
		//float light_y = static_cast<float>(sin(glfwGetTime()) * 12);
		//float light_z = static_cast<float>(cos(glfwGetTime()) * 5);

		lightPos = glm::normalize(glm::vec3(light_x, light_y, light_z));

		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		cubeShader.setMat4("model", model);

		glBindVertexArray(VAO); 
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//lamp cube

		lightShader.activate();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cubeShader.activate();
		glm::mat4 sphere_model = glm::mat4(1.0f);
		const float radiusS = 5.0f;
		float camXS = static_cast<float>(sin(glfwGetTime()) * radiusS);
		float camZS = static_cast<float>(cos(glfwGetTime()) * radiusS);
		sphere_model = glm::translate(sphere_model, glm::vec3(camXS, camXS, camZS));
		lightShader.setMat4("model", sphere_model);
		sphere.draw(cubeShader.id);

		// render your GUI
		ImGui::Begin("Properties");
		ImGui::SetWindowSize(win1, 0);
		ImGui::SliderFloat("texture mix", &mixTex, 0, 1 );
		ImGui::SetWindowSize(win1, 0);
		ImGui::SliderFloat("Fov", &fov, 0, 90);
		ImGui::End();

		camera.SetZoom(fov);

		gui.endFrame();


		processInput(window.getGLFWwindow());
		glfwSwapBuffers(GLFW_WINDOW);
		glfwPollEvents();


	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime * 2.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		if (!tab_pressed_last_frame) {
			tab_pressed_last_frame = true;
			mouse_locked = !mouse_locked;
			if (mouse_locked) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				lastX = static_cast<float>(xpos);
				lastY = static_cast<float>(ypos);
			}
			glfwSetInputMode(window, GLFW_CURSOR, mouse_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
	}
	else {
		tab_pressed_last_frame = false;
	}

}

void framebuff_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
	if (mouse_locked) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}else if (center_on_cursor) {
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		center = glm::vec3(xpos, ypos, 0.0f);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

