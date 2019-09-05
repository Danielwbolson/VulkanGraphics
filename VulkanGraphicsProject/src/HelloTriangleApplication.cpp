
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

	// Create our window for our application
	createSurface();

	// Grab our graphics card
	pickPhysicalDevice();

	// Create a logical device to interact with the gpus
	createLogicalDevice();
}

void HelloTriangleApplication::mainLoop() {
	// Keep running until window closes or error
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup() {
	// Destroy our glfw window
	vkDestroySurfaceKHR(instance, windowSurface, nullptr);

	// Destroy vulkan instance
	vkDestroyInstance(instance, nullptr);

	// Destroy logical device
	vkDestroyDevice(logicalDevice, nullptr);

	// Destroy created window
	glfwDestroyWindow(window);

	// Terminate glfw itself
	glfwTerminate();
}

void HelloTriangleApplication::createInstance() {

	// Check if we are requesting any unsupported validation layers
	if (enableValidationLayers && !checkValidationSupport()) {
		throw std::runtime_error("Some validation layers that were requested are not available");
	}

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
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	// Enable global validation layers if wanted, otherwise set them to nothing/null
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	// Check for extension support
	// Get number of extensions so we can allocate space
	unsigned int extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Allocate an array to hold extension details, and fill it
	// Each VkExtensionProperty holds a name and version of an extension
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Verify extensions needed by GLFW are supported
	for (int i = 0; i < (int)glfwExtensionCount; i++) {
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

		for (int i = 0; i < (int)glfwExtensionCount; i++) {
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

bool HelloTriangleApplication::checkValidationSupport() {
	// Get number of layers supported by vulkan
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Get a vector of all of our supported validation layers
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check if all of our layers in validationLayers exist in availableLayers
	for (const auto& layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		// We did not find our validationLayer inside the availableLayers
		if (!layerFound) {
			return false;
		}
	}
	// If we reach this point, every validationLayer we wanted is supported
	return true;
}

void HelloTriangleApplication::pickPhysicalDevice() {
	// Get number of physicaldevices (gpus) with vulkan support
	unsigned int deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// If we found no Vulkan-supported devices
	if (deviceCount == 0) {
		throw std::runtime_error("No vulkan compatible gpus found.");
	}

	// Fill a vector with our available gpus
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// Check if our found devices are suitable gpus, grab first suitable one
	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	// If we found no suitable gpus
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("No suitable gpus found.");
	}
}

bool HelloTriangleApplication::isDeviceSuitable(const VkPhysicalDevice& device) {
	// Make sure our device has necessary queues and thus can process the commands we want
	indices = findQueueFamilies(device);
	// True if we successfully found the queue families we need
	return indices.isComplete();

	//// Get basic device info
	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//// Get information about features like texture compression, 64bit floats, multi-viewport rendering
	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	//// Return true if we have a discrete gpu and it supports a geometry shader
	//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(const VkPhysicalDevice& device) {
	// Get our Queue Family properties first by finding how many, than allocating space for them grabbing them
	unsigned int queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Find our wanted queue families
	// Very likely that presentation and graphics will be the same index, but possibility of not
	int i = 0; 
	for (const auto& queueFamily : queueFamilies) {
		// Early exit if we have already found a suitable graphics card
		if (indices.isComplete()) {
			break;
		}

		// Grab presentation (window) queue index
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		// Grab graphics queue index
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		
		// Increment i
		i++;
	}

	return indices;
}

void HelloTriangleApplication::createLogicalDevice() {

	// Create our queue families for our logical device
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<unsigned int> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;

	// Must create a different queue info per family
	for (unsigned int queueFamily : uniqueQueueFamilies) {

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// What does this device support, which features?
	VkPhysicalDeviceFeatures deviceFeatures = {};

	// Set up logic device info using our queues and features struct
	VkDeviceCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<unsigned int>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	// Device specific setup. Device specific setup matters because diffferent devices support
	// different features. EX. Compute gpu vs graphcis gpu. Compute doesn't have the feature for rendering
	createInfo.enabledExtensionCount = 0;

	// New vulkan does not differentiate between global and device validation layers
	// This is for older versions
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else {
		createInfo.enabledLayerCount = 0;
	}
	
	// Actually instantiate our logical device, for communication with gpu
	// Will throw an error if we have tried to enable non-existant device extensions or
	// tried to specify features that are not actually supported
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device to interact with gpu");
	}

	// Create graphics and presentation queue handlers so we can interact with them
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentationQueue);

	// At this point, we can actually use the graphics card to do things
	// We have set up the basic necessary information to interact with the gpu and draw
}

void HelloTriangleApplication::createSurface() {
	// Vulkan is platform agnostic so GLFW handles platform specific window creation for us
	if (glfwCreateWindowSurface(instance, window, nullptr, &windowSurface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface with glfw/vulkan");
	}
}