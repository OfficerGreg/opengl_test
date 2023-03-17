#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse{
public:
	static void cursosPosCallback(GLFWwindow* window, double _x, double _y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static double getMouseX();
	static double getMouseY();

	static double getDX();
	static double getDY();

	static double getScrollDX();
	static double getScrollDY();

	static bool button(int button);
	static bool buttonChanged(int button);
	static bool buttonPressed(int button);
	static bool buttonReleased(int button);

private:
};

#endif 
