#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

class Shader
{
public:
	//program ID

	GLuint Program;
	
	Shader(const GLchar* vertexFileName, const GLchar* fragmentFileName);

	void Use();

	~Shader();
};

