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
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader s);

	unsigned int VAO;

private:
	unsigned int VBO, EBO;
	void setupMesh();
};

#endif

