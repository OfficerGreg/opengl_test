#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include "graphics/block/Block.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "io/Screen.h"
#include "io/Camera.h"

void processInput(double deltaTime);

float mixVal = 0.5f;

Joystick mainJ(0);
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera Camera::secondary(glm::vec3(5.0f, 5.0f, 5.0f));
bool Camera::usingDefault = true;

double deltaTime = 0.0f; // tme btwn frames
double lastFrame = 0.0f; // time of last frame

Screen screen;

int main() {
	int success;
	char infoLog[512];


	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif
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

	// SHADERS===============================
	Shader shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");


	Block block(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	block.init();
	
	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present." << std::endl;
	}

	while (!screen.shouldClose()) {
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

		block.render(shader);

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