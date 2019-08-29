
#include <GLFW/glfw3.h>

class HelloTriangleApplication {

public:
	// Run our HelloTriangle program
	void run();

private:
	// Initialize GLFW and create a window
	void initWindow();

	void initVulkan();

	void mainLoop();

	// Free up dynamic memory and allocated objects in Vulkan
	void cleanup();

public:

private:
	GLFWwindow* window;

};