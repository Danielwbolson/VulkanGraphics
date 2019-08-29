
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

	// Check for extension support
	// Get number of extensions so we can allocate space
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Allocate an array to hold extension details, and fill it
	// Each VkExtensionProperty holds a name and version of an extension
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Verify extensions needed by GLFW are supported
	for (int i = 0; i < glfwExtensionCount; i++) {
		bool supported = false;

		for (const auto& extension : extensions) {
			if (strcmp(glfwExtensions[i], extension.extensionName) == 0) {
				supported = true;
			}
		}
		
		if (!supported) {
			throw std::runtime_error("Necessary GLFW Extension not supported!");
		}
	}

	// Print out available extentions that we can use with Vulkan
	std::cout << "Available extensions:" << std::endl;
	for (const auto& extension : extensions) {
		bool usedbyGLFW = false;

		for (int i = 0; i < glfwExtensionCount; i++) {
			// Ithis extention is used  by glfw, break and print as such
			if (strcmp(glfwExtensions[i], extension.extensionName) == 0) {
				usedbyGLFW = true;
				break;
			}
		}

		// Give user some info in an extension is being used by GLFW
		if (usedbyGLFW) {
			std::cout << "\t" << extension.extensionName << " : used by GLFW" << std::endl;
		} else {
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	}


	// Finally, create instance 
	// arguments:
	//	pointer to creation struct, 
	//	pointer to custom allocator callback, 
	//	pointer to variable that stores handle to instance)
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vulkan instance!");
	}
}