#pragma once


#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

class VulkanApplication {

	/// * * * * * INITIALIZATION AND MAIN LOGIC * * * * * ///
public:
	// Run our HelloTriangle program
	void run();

private:
	// Initialize GLFW and create a window
	void initWindow();

	// Initialize our handle to vulkan and setup graphics communication
	void initVulkan();

	// Our main game loop
	void mainLoop();

	// Free up dynamic memory and allocated objects in Vulkan
	void cleanup();

	// Our main draw loop. Calls draw commands
	void drawFrame();

	// Our callback for resizing of window
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);


	
	/// * * * * * VULKAN HANDLE CREATION AND MANAGEMENT* * * * * ///

	// Create our Vulkan instance. Connection between app and vulkan
	void createInstance();

	// Create a logical device to interact with gpu with
	void createLogicalDevice();

	// Create our window to interact with our application
	void createSurface();

	// Create our swapchain to draw images to our surface
	void createSwapChain();

	// Handle window changes like fullscreen
	void recreateSwapChain();
	void cleanupSwapChain();

	// Create our handle to basic views of our swap chain images
	void createImageViews();

	// Create our grpahics pipeline, creating shaders and graphics pipeline settings
	void createGraphicsPipeline();

	// Create our shader module
	VkShaderModule createShaderModule(const std::vector<char>& code);

	// Set up our render pass
	void createRenderPass();

	// Create our framebuffers for each image in our swapchain
	void createFramebuffers();

	// Create our command pool
	void createCommandPool();

	// Create our command buffers to be used in our command pool
	void createCommandBuffers();

	// Set up our semaphores and fences
	void createSyncObjects();
	
	// Check if all requested validation layers are supported
	bool checkValidationSupport();



	/// * * * * * GPU FOCUSED * * * * * ///

	// Grab our graphics card
	void pickPhysicalDevice();

	// Check if our gpu is suitable for the operations we want
	// Currently only works if a user has a discrete gpu included
	bool isDeviceSuitable(const VkPhysicalDevice&);

	// Find which queue families are supported by a device
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

	// Checks if our device has the extensions that we need
	bool checkExtensionSupport(const VkPhysicalDevice& device);
	
	// Query details of swap chain extension on device for future creation of swap chain 
	SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device);

	// Choose which format we want from those available
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	// Choose which presentation mode we want
	// Only VK_PRESENT_MODE_FIFO_KHR is guaranteed
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	// Set resolution of swap chain images
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


public:
	const int windowWidth = 800;
	const int windowHeight = 600;

private:
	// Our window to draw to
	GLFWwindow* window;
	VkInstance instance;

	// Logical device to interact with gpu
	// May have multiple logical devices, each for different requirements
	VkDevice logicalDevice;

	// Our physical device that vulkan will use (gpu)
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // implicitly destroyed with instance

	// Handle to interact with graphics queues in the logical device
	VkQueue graphicsQueue;
	// Handle to interact with presentation queue in the logical device
	VkQueue presentationQueue;

	// Cached queue families supported on our physical device
	QueueFamilyIndices indices;

	// Window surface, where pixels go, allows platform agnostic vulkan to interface with window
	// Our interface between Vulkan and our GLFW window
	VkSurfaceKHR windowSurface;

	// Our swapchain handles
	VkSwapchainKHR swapChain;
	VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	// Command pool and buffers for our graphics queue
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;


	// Frame variables
	const int MAX_FRAMES_IN_FLIGHT = 2;
	int currentFrame = 0;
	bool frameBufferResized = false;

	// Semaphores/fences to synchronize drawing operations on gpu
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	// Handle to our one graphics pipeline
	VkPipeline graphicsPipeline;
	// Our handle to our renderpass. Determines what happens during this render
	VkRenderPass renderPass;
	// Our shader layout. Need one for each shader combination / pipeline we want
	VkPipelineLayout pipelineLayout;

	// Which validation layers we want, which check for improper usage
	// Validates what we are using
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	// Wanted extensions in the device
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

};
