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

	// Load GLAD so it configures OpenGL.
	gladLoadGL();

	// Specify the viewport of OpenGL in the window.
	glViewport(0, 0, WIDTH, HEIGHT);

	// Specify background colour.
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Clear the back buffer and assign the new colour to it.
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the front and back buffers.
	// (NOTE: The front buffer is the current screen, 
	// the back buffer is the next frame (in preparation for it to be drawn).
	glfwSwapBuffers(window);

	// Run continuously, until the window is closed.
	while (!glfwWindowShouldClose(window)) {
		// Poll for events that may or may not happen (who knows.......)
		glfwPollEvents();
	}

	// End by closing a bunch of shit.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}