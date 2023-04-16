#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
 
struct Vertex{
	glm::vec3 pos;
	glm::vec2 texCoord;
	
	static std::vector<Vertex> genList(float* vertices, int noVertices);

};
typedef struct Vertex Vertex;

#endif // !MESH_H
