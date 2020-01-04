#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>


const int WIDTH = 1280;
const int HEIGHT = 720;


class HelloTriangleApplication
{

public:
	void run();

private:
	void initWindow();
	void initVulkan();
	void createInstance();
	void mainLoop();
	void cleanup();

private:

	// GLFW
	GLFWwindow* window;

	// Vulkan
	VkInstance instance;
	

};
