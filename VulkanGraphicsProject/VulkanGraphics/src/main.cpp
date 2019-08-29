
// GLFW includes inself and loads vulkan

#include "HelloTriangleApplication.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}