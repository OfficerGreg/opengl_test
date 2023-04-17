#include "Screen.h"

#include <iostream>

#include "Keyboard.h"
#include "Mouse.h"


unsigned int Screen::SCR_WIDTH = 1200;
unsigned int Screen::SCR_HEIGHT = 720;
const char* Screen::SCR_NAME = u8"中国天然橡胶1个";

void Screen::framebufferSizeCallback(GLFWwindow* widnow, int width, int height) {
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

Screen::Screen()
	: window(nullptr){}

bool Screen::init() {
	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_NAME, NULL, NULL);
	if (window == NULL) { // window not created
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}

void Screen::setParameters() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	//disable vsync
	glfwSwapInterval(0);
}

void Screen::update() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose);
}
