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
#include<chrono>
#include<thread>
//#include<Windows.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

// FUNCTION DECLARATIONS **********************
int getTime();

// MAIN *******************************************
int main() {
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const int FRAME_PERIOD = 17; // ms

	srand(time(NULL));

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
		// CO-ORDS ----------------------------------- \\  // COLOURS -------- \\.
		-0.5f,     -0.5f * float(sqrt(3)) / 3,       0.0f, 0.02f, 0.3f,  0.8f,  // Lower left corner
		0.5f,      -0.5f * float(sqrt(3)) / 3,       0.0f, 0.8f,  0.3f,  0.02f, // Lower right corner
		0.0f,      0.5f * float(sqrt(3)) * 2.0f / 3, 0.0f, 0.32f, 0.6f,  1.0f,  // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6,        0.0f, 0.9f,  0.45f, 0.17f, // Inner left
		0.5f / 2,  0.5f * float(sqrt(3)) / 6,        0.0f, 0.9f,  0.45f, 0.17f, // Inner right
		0.0f,      -0.5f * float(sqrt(3)) / 3,       0.0f, 0.8f,  0.3f,  0.02f // Middle bottom
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

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	int prev_frame = 0;
	int now = 0;
	int frameCounter = 0;

	// Run continuously, until the window is closed.
	while (!glfwWindowShouldClose(window)) {
		// Move vertices and colours a little bit.
		int i = 0; 
		for (GLfloat num : vertices) {
			num += (GLfloat)(rand() / (float)RAND_MAX / 100.0f - 0.005f);

			if (num < -1.0f) {
				num = -1.0f;
			}
			else if (num > 1.0f) {
				num = 1.0f;
			}

			vertices[i] = num;
			i++;
		}
		
		/*
		for (GLfloat num : vertices) {
			std::cout << num << " ";
		}
		std::cout << std::endl;
		*/
		
		// Specify background colour.
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Clear the back buffer and assign the new colour to it.
		glClear(GL_COLOR_BUFFER_BIT);

		// Generates vertex array object and binds it.
		VAO VAO1;
		VAO1.Bind();

		// Generates vertex buffer object and links it to vertices.
		VBO VBO1(vertices, sizeof(vertices));
		// Generates element buffer object and links it to indices.
		EBO EBO1(indices, sizeof(indices));
		// Links VBO to VAO.
		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		// Unbind all to prevent accidentally modifying them.
		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

		shaderProgram.Activate();
		glUniform1f(uniID, 1.0f);
		VAO1.Bind(); 
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// Wait until next frame.
		std::this_thread::sleep_for(std::chrono::milliseconds(
			prev_frame + FRAME_PERIOD - getTime() - 10));
		prev_frame = getTime();

		glfwSwapBuffers(window);

		// Poll for events that may or may not happen (who knows.......)
		glfwPollEvents();

		// Delete stuff.
		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();

		frameCounter++;
	}

	
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