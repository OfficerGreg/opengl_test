#include "Sphere.h"

#include <cmath>
#include <glm/gtc/type_ptr.hpp>

Sphere::Sphere(unsigned int segments) {
    createSphere(segments);
    setupMesh();
}


Sphere::~Sphere() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Sphere::createSphere(unsigned int segments) {
    float radius = 1.0f;
    float segmentStep = 1.0f / static_cast<float>(segments);
    float angleStep = 2.0f * glm::pi<float>() * segmentStep;

    for (unsigned int i = 0; i <= segments; ++i) {
        for (unsigned int j = 0; j <= segments; ++j) {
            float x = radius * sin(i * glm::pi<float>() * segmentStep) * cos(j * angleStep);
            float y = radius * sin(i * glm::pi<float>() * segmentStep) * sin(j * angleStep);
            float z = radius * cos(i * glm::pi<float>() * segmentStep);
            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (unsigned int i = 0; i < segments; ++i) {
        for (unsigned int j = 0; j < segments; ++j) {
            indices.push_back(i * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j + 1);

            indices.push_back(i * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j + 1);
            indices.push_back(i * (segments + 1) + j + 1);
        }
    }
}


void Sphere::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}


void Sphere::draw(GLuint shaderProgram) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}