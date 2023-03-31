#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window{
public:
	Window(uint16_t window_width, uint16_t window_height, std::string window_title);

	~Window();
	GLFWwindow* getGLFWwindow()
		const {
		return m_window;
	}
private:
	GLFWwindow *m_window;
	uint16_t m_window_width;
	uint16_t m_window_height;
	std::string m_window_title;

};


#endif

