#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include "graphics/block/Block.h"

#include "world/Chunk.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "io/Screen.h"
#include "io/Camera.h"

void processInput(double deltaTime);
void show_wireframe();

float mixVal = 0.5f;

Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera Camera::secondary(glm::vec3(5.0f, 5.0f, 5.0f));
bool Camera::usingDefault = true;

double deltaTime = 0.0f; // tme btwn frames
double lastFrame = 0.0f; // time of last frame

Screen screen;

int main() {

	if (!screen.init()) {
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	// SHADERS
	Shader shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");

	//Block
	Block block(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f));
	block.init();

	Chunk chunk(glm::vec3(0.0f, 0.0f, 0.0f));
	chunk.init();
	

	while (!screen.shouldClose()) {

		show_wireframe();

		// calculate dt
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(deltaTime);

		screen.update();

		shader.activate();

		shader.setFloat("mixVal", mixVal);


		// create transformation
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Camera::usingDefault ? Camera::defaultCamera.getViewMatrix() : Camera::secondary.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::usingDefault ? Camera::defaultCamera.getZoom() : Camera::secondary.getZoom()),
			(float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		chunk.render(shader);
		//block.render(shader);

		screen.newFrame();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}
	block.cleanup();

	glfwTerminate();
	return 0;
}

void processInput(double deltaTime) {
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		screen.setShouldClose(true);
	}

	// change mix value
	if (Keyboard::key(GLFW_KEY_UP)) {
		mixVal += .05f;
		if (mixVal > 1) {
			mixVal = 1.0f;
		}
	}
	if (Keyboard::key(GLFW_KEY_DOWN)) {
		mixVal -= .05f;
		if (mixVal < 0) {
			mixVal = 0.0f;
		}
	}

	// update camera
	if (Keyboard::keyPressed(GLFW_KEY_TAB)) {
		Camera::usingDefault = !Camera::usingDefault;
	}

	// move camera
	CameraDirection direction = CameraDirection::NONE;

	if (Keyboard::key(GLFW_KEY_W)) {
		direction = CameraDirection::FORWARD;
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		direction = CameraDirection::BACKWARD;
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		direction = CameraDirection::RIGHT;
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		direction = CameraDirection::LEFT;
	}
	if (Keyboard::key(GLFW_KEY_E)) {
		direction = CameraDirection::UP;
	}
	if (Keyboard::key(GLFW_KEY_Q)) {
		direction = CameraDirection::DOWN;
	}

	if ((int)direction) {
		if (Camera::usingDefault) {
			Camera::defaultCamera.updateCameraPos(direction, deltaTime);
		}
		else {
			Camera::secondary.updateCameraPos(direction, deltaTime);
		}
	}
}

void show_wireframe() {
	static bool wireframeMode = false;
	if (Keyboard::keyPressed(GLFW_KEY_G)){
		wireframeMode = !wireframeMode;
	}
	// Set the polygon mode based on the wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
}