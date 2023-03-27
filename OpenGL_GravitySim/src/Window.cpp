#include "Window.h"



#include <iostream>
#include <spdlog/spdlog.h>
#include "stb_image.h"

Window::Window(uint16_t window_width, uint16_t window_height, std::string window_title)
	: m_window_width(window_width), m_window_height(window_height), m_window_title(window_title)
{
	//glfw init
	if (!glfwInit()) {
		spdlog::error("Failed to initialize GLFW");
		exit(EXIT_FAILURE);
	}
	//glfw settings
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::cout << glfwGetVersionString() << std::endl;

	//create window
	m_window = glfwCreateWindow(m_window_width, m_window_height, m_window_title.c_str(), NULL, NULL);
	if (m_window == NULL) {
		spdlog::error("Failed to create GLFW window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	else {
		spdlog::info("Successfully created GLFW window!");
	}
	glfwMakeContextCurrent(m_window);




	//init glad (rendering window)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		exit(EXIT_FAILURE);
	}
	else {
		spdlog::info("Successfully initialized GLAD!");
	}


	//set viewport 
	glViewport(0, 0, m_window_width, m_window_height);



	//set the window icon
	int icon_width, icon_height, icon_channels;

	unsigned char* icon_data = stbi_load("assets/image1.png", &icon_width, &icon_height, &icon_channels, 4);
	if (icon_data) {
		spdlog::info("Window icon loaded successfully!");
	}
	else {
		spdlog::error("Window icon could not be loaded");
	}


	GLFWimage icons[1];
	icons[0].width = icon_width;
	icons[0].height = icon_height;
	icons[0].pixels = icon_data;

	glfwSetWindowIcon(m_window, 1, icons);
	stbi_image_free(icon_data);
}

Window::~Window() {
	glfwTerminate();
}