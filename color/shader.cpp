#include "shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const GLchar* vsPath, const GLchar* fsPath)
{
	std::string vsCodeString;
	std::string fsCodeString;
	std::ifstream vsFile;
	std::ifstream fsFile;

	// ensure ifstream objects can throw exceptions:
    vsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fsFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    // load shaders from files
    try {
    	vsFile.open(vsPath);
    	std::stringstream vsStream;
    	vsStream << vsFile.rdbuf();
    	vsFile.close();
    	vsCodeString = vsStream.str();

    	fsFile.open(fsPath);
    	std::stringstream fsStream;
    	fsStream << fsFile.rdbuf();
    	fsFile.close();
    	fsCodeString = fsStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vsCode = vsCodeString.c_str();
    const char* fsCode = fsCodeString.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // compile shaders
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vsCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
    	glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fsCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
    	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success) {
    	glGetProgramInfoLog(ID, 512, NULL, infoLog);
    	    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setMat4(const std::string &name, const glm::mat4 &matrix) const
{
    unsigned int matloc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(matloc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vector) const
{
    unsigned int vecloc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(vecloc, 1, &vector[0]);
}

