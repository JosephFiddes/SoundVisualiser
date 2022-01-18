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

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

/**
* Takes the source code of two shaders, and turns them into
* a shader program.
*/
GLuint shadersToProgram(const char** source1, GLenum type1, const char** source2, GLenum type2);

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

	// Create window, and exit if it fails.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SoundVisualiser", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce window to current context.
	glfwMakeContextCurrent(window);
	
	// Triforce
	GLfloat vertices[] = {
		-0.5f,     -0.5f * float(sqrt(3)) / 3,       0.0f, // Lower left corner
		0.5f,      -0.5f * float(sqrt(3)) / 3,       0.0f, // Lower right corner
		0.0f,      0.5f * float(sqrt(3)) * 2.0f / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6,        0.0f, // Inner left
		0.5f / 2,  0.5f * float(sqrt(3)) / 6,        0.0f, // Inner right
		0.0f,      -0.5f * float(sqrt(3)) / 3,       0.0f, // Middle bottom
	};

	GLuint indices[] = {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1  // Upper triangle
	};

	// Load GLAD so it configures OpenGL.
	gladLoadGL();

	// Specify the viewport of OpenGL in the window.
	glViewport(0, 0, WIDTH, HEIGHT);

	// Generates Shader object using shaders default.vert and default.frag.
	Shader shaderProgram("default.vert", "default.frag");

	// Generates vertex array object and binds it.
	VAO VAO1;
	VAO1.Bind();

	// Generates vertex buffer object and links it to vertices.
	VBO VBO1(vertices, sizeof(vertices));
	// Generates element buffer object and links it to indices.
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO.
	VAO1.LinkVBO(VBO1, 0);

	// Unbind all to prevent accidentally modifying them.
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

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
		 
		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		// Poll for events that may or may not happen (who knows.......)
		glfwPollEvents();
	}

	// Delete stuff.
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// End by closing a bunch of stuff.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}