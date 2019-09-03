
#include <optional>

struct QueueFamilyIndices {
	std::optional<unsigned int> graphicsFamily;

	bool isComplete() {
		return graphicsFamily.has_value();
	}
};