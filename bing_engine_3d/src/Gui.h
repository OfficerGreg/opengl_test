#ifndef GUI_H
#define GUI_H

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>

class Gui{
public:
	Gui(GLFWwindow* window, std::string glsl_verison);
	~Gui();
	void beginFrame();
	void endFrame();

private:
	std::string m_glsl_version;
	GLFWwindow* m_window;
	ImVec2 m_gui_window;
};

#endif

