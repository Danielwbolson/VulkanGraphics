
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

	// Initialize window (width,height,name,which monitor to place on, opengl-specific)
	window = glfwCreateWindow(windowWidth, windowHeight, "Vulkan", nullptr, nullptr);
}

void HelloTriangleApplication::initVulkan() {
	// Create our Vulkan instance. Connection between app and vulkan
	createInstance();
}

void HelloTriangleApplication::mainLoop() {
	// Keep running until window closes or error
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup() {
	// Destroy created window
	glfwDestroyWindow(window);

	// Terminate glfw itself
	glfwTerminate();
}

void HelloTriangleApplication::createInstance() {
	// Set up basic info about application for vulkan
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Following applies to entire program, not a specific device --> global
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Create extension interface to allow us to work glfw windows
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	// Enable global validation layers
	createInfo.enabledLayerCount = 0;

	// Finally, create instance 
	// arguments:
	//	pointer to creation struct, 
	//	pointer to custom allocator callback, 
	//	pointer to variable that stores handle to instance)
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vulkan instance!");
	}
}