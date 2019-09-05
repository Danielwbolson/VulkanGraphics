
#include <optional>

struct QueueFamilyIndices {
	std::optional<unsigned int> graphicsFamily;
	std::optional<unsigned int> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();;
	}
};