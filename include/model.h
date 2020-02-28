#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"


using namespace std;

class Model
{
public:
	Model(const char *path);
	void Draw(Shader shader);
	vector<Mesh> meshes;

private:
	string directory;

	void loadModel(string path);
	void processNode(aiNode *mode, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
	unsigned int TextureFromFile(const char* filename);
};

#endif 