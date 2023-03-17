#include "Mouse.h"


void Mouse::cursosPosCallback(GLFWwindow* window, double _x, double _y){}
void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){}
void Mouse::mouseWheelCallback(GLFWwindow* window, double dx, double dy){}

double Mouse::getMouseX(){}
double Mouse::getMouseY(){}

double Mouse::getDX(){}
double Mouse::getDY(){}
 
double Mouse::getScrollDX(){}
double Mouse::getScrollDY(){}

bool Mouse::buttonChanged(int button){}
bool Mouse::buttonPressed(int button){}
bool Mouse::buttonReleased(int button){}
bool Mouse::button(int button){}