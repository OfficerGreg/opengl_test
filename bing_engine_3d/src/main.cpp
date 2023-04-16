#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/Shader.h"
#include "graphics/Texture.h"

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

	// VBO, VAO, EBO
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// set attributes pointers
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Texturess

	Texture texture1("assets/image1.png", "texture1");
	Texture texture2("assets/image3.jpg", "texture2");

	texture1.load();
	texture2.load();

	shader.activate();
	shader.setInt("texture1", texture1.id);
	shader.setInt("texture2", texture2.id);

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

		// bind texture
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glActiveTexture(GL_TEXTURE1);
		texture2.bind();
		// draw shapes
		glBindVertexArray(VAO);
		shader.activate();


		shader.setFloat("mixVal", mixVal);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// create transformation
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		view = Camera::usingDefault ? Camera::defaultCamera.getViewMatrix() : Camera::secondary.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::usingDefault ? Camera::defaultCamera.getZoom() : Camera::secondary.getZoom()),
			(float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f);

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(0);

		screen.newFrame();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	//glDeleteBuffers(1, &EBO);

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