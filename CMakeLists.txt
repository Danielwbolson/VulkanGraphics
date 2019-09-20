cmake_minimum_required (VERSION 3.8)

project (Vulkan_Graphics LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

if (UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-implicit-fallthrough -Wshadow -Wno-unused-variable -Wno-unused-function")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2") # look up Ofast
elseif (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS 	OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS 	OFF CACHE BOOL " " FORCE)

add_subdirectory(external/glfw)

cmake_minimum_required(VERSION 3.8)

set(SRC
	src/main.cpp
	src/VulkanApplication.cpp
)

set(INCS
	src/QueueFamilyIndices.h
	src/SwapChainSupportDetails.h
	src/VulkanApplication.h
)

set(INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/external
	${CMAKE_CURRENT_SOURCE_DIR}/external/glfw
	${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/Vulkan/1.1.114.0/include
)

set(SYSTEM_LIBS "")
if (UNIX AND NOT APPLE)
	set(SYSTEM_LIBS
		dl
		stdc++fs
	)
endif()

set(LIBS
	glfw
	${SYSTEM_LIBS}
)

if (MSVC)
	set(LIB_DIRS
		${CMAKE_CURRENT_SOURCE_DIR}/external/Vulkan/1.1.114.0/Lib
	)
	set(LIBS
		${LIBS}
		vulkan-1
	)
else()
	find_package(VULKAN REQUIRED)
	set(LIBS
		LIBS	
		Vulkan::Vulkan
	)
endif()

link_directories(${LIB_DIRS})
include_directories(${INCLUDE_DIRS})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vulkanGraphics)

add_executable(vulkanGraphics ${SRC} ${INCS})

if (MSVC)
	target_compile_options(vulkanGraphics PRIVATE "/MP")
endif()

target_link_libraries(vulkanGraphics ${LIBS})