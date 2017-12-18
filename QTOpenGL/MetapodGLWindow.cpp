#include <GL\glew.h>
#include "MetapodGLWindow.h"
#include <string>
#include <iostream>
#include <fstream>

using std::string;

void MetapodGLWindow::sendDataToOpenGL() {
	GLfloat verts[] = {
		+0.0f, +1.0f, +1.0f, //Vertices
		+1.0f, +1.0f, +0.0f,
		-1.0f, -1.0f, +1.0f, //Vertices
		+1.0f, +0.0f, +0.0f,
		+1.0f, -1.0f, +1.0f, //Vertices
		+0.0f, +1.0f, +0.0f
	};

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	GLushort indices[] = { 0,1,2 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

string MetapodGLWindow::readShaderCode(char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "No se ha podido abrir el archivo: " << fileName;
		exit(1);
	}
	return string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void MetapodGLWindow::installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string shaderString = readShaderCode("vertexShaderCode.glsl");
	adapter[0] = shaderString.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	shaderString = readShaderCode("fragmentShaderCode.glsl");
	adapter[0] = shaderString.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	if (!checkProgramStatus(programID))
		return;
	glUseProgram(programID);
}

bool MetapodGLWindow::checkShaderStatus(GLuint shaderID) {
	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		GLint infoLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLength);
		GLchar* buffer = new GLchar[infoLength];

		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool MetapodGLWindow::checkProgramStatus(GLuint programID) {
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint infoLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLength);
		GLchar* buffer = new GLchar[infoLength];

		GLsizei bufferSize;
		glGetProgramInfoLog(programID, infoLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

void MetapodGLWindow::initializeGL() {
	glewInit();
	sendDataToOpenGL();
	installShaders();
}

void MetapodGLWindow::paintGL() {
	glViewport(0, 0, width(), height());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}
