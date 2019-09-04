
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "QueueFamilyIndices.h"

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

	// Check if all requested validation layers are supported
	bool checkValidationSupport();

	// Grab our graphics card
	void pickPhysicalDevice();

	// Check if our gpu is suitable for the operations we want
	// Currently only works if a user has a discrete gpu included
	bool isDeviceSuitable(const VkPhysicalDevice&);

	// Find which queue families are supported by a device
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

	// Create a logical device to interact with gpu with
	void createLogicalDevice();

public:
	const int windowWidth = 800;
	const int windowHeight = 600;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation",
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

private:
	GLFWwindow* window;
	VkInstance instance;

	// Logical device to interact with gpu
	// May have multiple logical devices, each for different requirements
	VkDevice logicalDevice;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // implicitly destroyed with instance

	// Handle to interact with graphics queues in the logical device
	VkQueue graphicsQueue;

	// Cached queue families supported on our physical device
	QueueFamilyIndices indices;
};