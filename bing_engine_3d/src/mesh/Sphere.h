#ifndef SPHERE_H
#define SPHERE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Sphere{
public:
	Sphere(unsigned int Segments);
	~Sphere();

	void draw(GLuint shaderProgram);
private:
    unsigned int vao, vbo, ebo;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    void createSphere(unsigned int segments);
    void setupMesh();
};

#endif // !SPHERE_H

