#define GLM_ENABLE_EXPERIMENTAL

#include <GL\glew.h>
#include "MetapodGLWindow.h"
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Vertex.h"
#include "ShapeGenerator.h"
#include "Camera.h"

using std::string;
using std::cout;
using std::endl;
using glm::mat4;
using glm::vec3;

GLuint numTriangles = 0;

const GLuint MAX_TRIES = 20;
const GLuint NUM_VERTEX_PER_TRI = 3;
const GLuint NUM_FLOATS_PER_VERTEX = 6;
const GLuint NUM_BYTES_TRI = NUM_VERTEX_PER_TRI * NUM_FLOATS_PER_VERTEX * sizeof(float);
const float OFF_SET = 0.1f;

GLuint programID;
GLsizei numIndices;
Camera camera;

void MetapodGLWindow::sendDataToOpenGL() {

	ShapeData shape = ShapeGenerator::makeQuad();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIndices = shape.indexBufferSize();
	shape.cleanup();
}

void MetapodGLWindow::sendAnotherTriangleToOpenGL() {

	if (numTriangles == MAX_TRIES)
		return;

	const GLfloat vertexPosition = -1.0f + numTriangles * OFF_SET;

	GLfloat myTri[] = {
		+vertexPosition, +1.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,

		+vertexPosition + OFF_SET, +1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f,

		+vertexPosition, +0.0f, +0.0f,
		+0.0f, +0.0f, +1.0f
	};

	glBufferSubData(GL_ARRAY_BUFFER, numTriangles * NUM_BYTES_TRI, NUM_BYTES_TRI, myTri);
	numTriangles++;

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

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	if (!checkProgramStatus(programID))
		return;
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
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
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
}

void MetapodGLWindow::mouseMoveEvent(QMouseEvent* e) {
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MetapodGLWindow::keyPressEvent(QKeyEvent* e) {
	switch(e->key()){
		case Qt::Key::Key_W:
			camera.moveForward();
			break;
		case Qt::Key::Key_A:
			camera.moveLeft();
			break;
		case Qt::Key::Key_S:
			camera.moveBackwards();
			break;
		case Qt::Key::Key_D:
			camera.moveRight();
			break;
		case Qt::Key::Key_R:
			camera.moveUp();
			break;
		case Qt::Key::Key_F:
			camera.moveDown();
			break;
	}
	repaint();
}

void MetapodGLWindow::paintGL() {
	glViewport(0, 0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");

	mat4 fullTransformMatrix;
	mat4 perspectiveMatrix = glm::perspective(1.54f, ((float)width()) / height(), 0.10f, 10.0f);

	//Cubo 1
	mat4 translationMatrix = glm::translate(vec3(-1.0f, +0.0f, -3.0f));
	mat4 rotationMatrix = glm::rotate(0.6283185f, vec3(1.0f, 0.0f, 0.0f));

	fullTransformMatrix = perspectiveMatrix*camera.getWorldToViewMatrix()*translationMatrix*rotationMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	//Cubo 2
	translationMatrix = glm::translate(vec3(+1.0f, +0.0f, -3.75f));
	rotationMatrix = glm::rotate(2.20f, vec3(0.0f, 1.0f, 0.0f));

	fullTransformMatrix = perspectiveMatrix*camera.getWorldToViewMatrix()*translationMatrix*rotationMatrix;

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}
