#include "Mesh.h"

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices) {
	std::vector<Vertex> ret(noVertices);

	int stride = sizeof(Vertex) / sizeof(float);

	for (int i = 0; i < noVertices; i++) {
		ret[i].pos = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

		ret[i].texCoord = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);
	}
	return ret;
}