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
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<chrono>
#include<thread>
//#include<Windows.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"

// FUNCTION DECLARATIONS **********************
int getTime();
GLFWwindow* createWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

// MAIN *******************************************
int main() {
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const int FRAME_PERIOD = 17; // ms

	// Begin random number generator.
	srand(time(NULL));

	// Create window, and exit if it fails.
	GLFWwindow* window = createWindow(WIDTH, HEIGHT, "SoundVisualiser", NULL, NULL);
	
	// Rectangle
	GLfloat vertices[] = {
		// CO-ORDS -----\\  // COLOURS ----- \\ 
		-1.0f, -1.0f, 0.0f,	0.83f, 0.70f, 0.44f,
		1.0f,  -1.0f, 0.0f,	0.83f, 0.70f, 0.44f,
		1.0f,  1.0f, 0.0f,	0.83f, 0.70f, 0.44f,
		-1.0f, 1.0f, 0.0f,	0.83f, 0.70f, 0.44f,
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
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

	// Links VBO to VAO.s
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them.
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// #JustFramerateThings.
	int prev_frame = 0;
	int now = 0;

	// Run continuously, until the window is closed.
	while (!glfwWindowShouldClose(window)) {

		// Specify background colour.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clear the back buffer and assign the new colour to it.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		VAO1.Bind(); 
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		// Wait until next frame.
		std::this_thread::sleep_for(std::chrono::milliseconds(
			prev_frame + FRAME_PERIOD - getTime() - 10));
		prev_frame = getTime();

		glfwSwapBuffers(window);

		// Poll for events that may or may not happen (who knows.......)
		glfwPollEvents();
	}

	char info[1024];
	glGetProgramInfoLog(shaderProgram.ID, 1024, NULL, info);
	std::cout << info << std::endl;

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

int getTime() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now()
		.time_since_epoch()).count();
}

GLFWwindow* createWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
	// Initialise GLFW
	glfwInit();

	// Tells GLFW we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tells GLFW we are using the CORE profile, giving us access to the modern functions.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, share);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Introduce window to current context.
	glfwMakeContextCurrent(window);

	return window;
}