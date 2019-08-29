
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>

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

	// Creates a Vulkan instance
	void createInstance();

public:
	const int windowWidth = 800;
	const int windowHeight = 600;

private:
	GLFWwindow* window;
	VkInstance instance;

};