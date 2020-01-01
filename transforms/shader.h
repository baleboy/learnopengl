#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

class Shader {
public:

	unsigned int ID;

	Shader(const GLchar* vertxShaderPath, const GLchar* fragmentShaderPath);

	void use();

	void setInt(const std::string &name, int value) const;

};

#endif