#include <GLEW\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>
#include <iostream>
#include "Shader.h"

//Constantes para la ventana
const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char *argv) {

	//Inicializamos el glfw y hacemos set de las propiedades que va a utilizar.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Creamos un puntero a donde almacenaremos la ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MetapodEngine", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//función que se llama cada vez que hay un evento (se presiona una tecla)
	glfwSetKeyCallback(window, key_callback);

	//Inicializamos glew
	//glewExperimental = GL_TRUE;
	glewInit();

	//El viewport es el tamaño en el que se renderizarán los gráficos dentro de la ventana
	//puede ser más pequeño
	glViewport(0, 0, 800, 600);

	//Creamos el objeto Shader
	Shader ourShader("vertexShader", "fragmentShader");

	//Render de rectángulo. Empezamos por los vértices
	GLfloat vertices[] = {
		//3 x position  3 x color  2 x texture coordinate
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Bottom left
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //Bottom right
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //Up left
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f  //Up Right
	};

	//Ahora utilizamos el Element buffer Object para decidir el orden que deberán seguirse al renderizar
	//de esta forma podemos ahorrar dos vértices del rectángulo repitiendo dos vértices
	GLuint indices[] = {
		0, 1, 2,  //Primer triángulo
		1, 2, 3,  //Segundo triángulo
	};

	//Este es el buffer de vértices que se envía a la GPU (VBO).
	//VAO es un objeto que almacena las llamadas de vertex attributes para que no tengamos
	//que volver a reescribir el código cada vez.
	GLuint VBO, VAO, EBO;
	//glGenBuffers(numero de buffers, ID del buffer) crea el buffer
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Hacemos bind del vertex array object primero, para que las llamadas a los búferes se almacenen
	glBindVertexArray(VAO);

	//Hacemos bind del vertex buffer generado al buffer de arrays mediante glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Una vez hemos hecho el bind, copiamos los datos del array en el buffer mediante glBufferData
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Hacemos bind del element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//STATIC_DRAW sirve para los datos que no cambian
	//DINAMIC_DRAW para datos que cambian mucho
	//STREAM_DRAW datos que cambian cada vez que se dibujan

	//Configuramos el buffer con el tipo de data (vector shader) que vamos a pasar
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Cerramos el bind del Vertex Array
	glBindVertexArray(0);


	// Load and create a texture 
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("textures/wood3.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


	while (!glfwWindowShouldClose(window))
	{
		//Activa los eventos
		glfwPollEvents();

		//le da color a la pantalla cada frame
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

		//limpia el buffer de color
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		//Dibujamos los objetos vía programShader
		ourShader.Use();

		//Hacemos update del color de la variable uniform del fragment shader
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//trabajamos con doble buffer, así que se va alternando
		glfwSwapBuffers(window);
	}
	//Salimos de la pantalla y acabamos el glfw
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//if we press esc, exit the window

	std::cout << key << std::endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}