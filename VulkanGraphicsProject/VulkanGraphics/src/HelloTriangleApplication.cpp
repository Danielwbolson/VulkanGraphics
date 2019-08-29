
#include "HelloTriangleApplication.h"

void HelloTriangleApplication::run() {
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow() {
	// Init glfw library
	glfwInit();

	// Force GLFW to not make an OpenGL client which is its default
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Disable resizing of windows for simplicity
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Initialize window
	window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {

}

void HelloTriangleApplication::mainLoop() {

}

void HelloTriangleApplication::cleanup() {

}