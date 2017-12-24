#pragma once
#include <QtOpenGL\qgl.h>

class MetapodGLWindow : public QGLWidget {
private:
	void sendDataToOpenGL(); 
	void sendAnotherTriangleToOpenGL();
	void installShaders();
	std::string readShaderCode(char* fileName);
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
protected:
	void initializeGL();
	void paintGL();
public:

};
