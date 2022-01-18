/* Visualises sound from audio input.
* 
* TODO:
* Detect input.
* Visualise input.
* 
*/

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

/**
* Takes the source code of two shaders, and turns them into
* a shader program.
*/
GLuint shadersToProgram(const char** source1, GLenum type1, const char** source2, GLenum type2);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"\tFragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
	const int WIDTH = 800;
	const int HEIGHT = 800;

	// Initialise GLFW
	glfwInit();

	// Tells GLFW we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tells GLFW we are using the CORE profile, giving us access to the modern functions.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
	};

	// Create window, and exit if it fails.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SoundVisualiser", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce window to current context.
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL.
	gladLoadGL();

	// Specify the viewport of OpenGL in the window.
	glViewport(0, 0, WIDTH, HEIGHT);

	GLuint shaderProgram = shadersToProgram(&vertexShaderSource, GL_VERTEX_SHADER, &fragmentShaderSource, GL_FRAGMENT_SHADER);

	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Swap the front and back buffers.
	// (NOTE: The front buffer is the current screen, 
	// the back buffer is the next frame (in preparation for it to be drawn)).
	glfwSwapBuffers(window);

	// Run continuously, until the window is closed.
	while (!glfwWindowShouldClose(window)) {
		// Specify background colour.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clear the back buffer and assign the new colour to it.
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		// Poll for events that may or may not happen (who knows.......)
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// End by closing a bunch of shit.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


GLuint shadersToProgram(const char** source1, GLenum type1, const char** source2, GLenum type2) {
	// Creates and compiles a vertex shader based on vertexShaderSource
	GLuint shader1 = glCreateShader(type1);
	glShaderSource(shader1, 1, source1, NULL);
	glCompileShader(shader1);

	// Creates and compiles a fragment shader based on fragmentShaderSource
	GLuint shader2 = glCreateShader(type2);
	glShaderSource(shader2, 1, source2, NULL);
	glCompileShader(shader2);

	// Combines the two shaders into one thing.
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shader1);
	glAttachShader(shaderProgram, shader2);
	glLinkProgram(shaderProgram);
	glDeleteShader(shader1);
	glDeleteShader(shader2);

	return shaderProgram;
}