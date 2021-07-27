#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include "glm/matrix.hpp"
class Shader
{
public: 
	//the program ID
	GLuint ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//Use/Activate the shader
	void Activate();
	//Utility uniform functions
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setVec3(const std::string&name, glm::vec3 value)const;
	void setMat4(const std::string&name, glm::mat4 value)const;
};
#endif