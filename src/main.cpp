
// GLFW includes inself and loads vulkan

#include "VulkanApplication.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

int main() {
	VulkanApplication app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}