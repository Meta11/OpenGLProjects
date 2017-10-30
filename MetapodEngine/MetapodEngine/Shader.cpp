#include "Shader.h"

Shader::Shader(const GLchar* vertexFileName, const GLchar* fragmentFileName)
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	
	string vertexSourcePath = "C:/Users/TOSHIBA/Documents/Visual Studio 2017/Projects/MetapodEngine/MetapodEngine/shaders/"
		+ (string)vertexFileName + ".txt";
	string fragmentSourcePath = "C:/Users/TOSHIBA/Documents/Visual Studio 2017/Projects/MetapodEngine/MetapodEngine/shaders/"
		+ (string)fragmentFileName + ".txt";

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		//Abrimos los archivos
		vShaderFile.open(vertexSourcePath);
		fShaderFile.open(fragmentSourcePath);
		//Leemos los ficheros y añadimos lo que hemos leído en los streams
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//Cerramos los ficheros
		vShaderFile.close();
		fShaderFile.close();
		//Convertimos los streams en formato string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	//Creamos y compliamos los shaders que necesitamos
	//vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Metemos el código del shader en el objeto que acabamos de crear y lo compilamos
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	//Comprobación  de que se ha compilado correctamente el Shader
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//lo mismo con el fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//Una vez tenemos los shaders, tenemos que vincularlos al programa utilizando el shader program
	this->Program = glCreateProgram();
	//Hacemos attach de los shaders creados a traves de glAttachShader y lo linkamos a través de linkprogram
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	//Comprobamos que se han linkado bien los shaders al programa 
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" <<
			infoLog << std::endl;
	}
	//Una vez hamos linkado los shaders al programa, ya no los necesitamos más
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


Shader::~Shader()
{
}

void Shader::Use() {
	glUseProgram(this->Program);
	GLfloat timeValue = glfwGetTime();
	GLfloat colorValue = (sin(timeValue) / 2 + 0.5);
	GLint vertexColorLocation = glGetUniformLocation(this->Program, "ourColor");
	glUniform4f(vertexColorLocation, colorValue, colorValue, 0.0f, 1.0f);
}