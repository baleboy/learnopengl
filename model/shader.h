#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

	unsigned int ID;

	Shader(const GLchar* vertxShaderPath, const GLchar* fragmentShaderPath);

	void use();

	void setInt(const std::string &name, int value) const;

	void setFloat(const std::string &name, float value) const;

	void setMat4(const std::string &name, const glm::mat4 &matrix) const;

	void setVec3(const std::string &name, const glm::vec3 &vector) const;

};

#endif