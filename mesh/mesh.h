#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>
#include "shader.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader s);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif

