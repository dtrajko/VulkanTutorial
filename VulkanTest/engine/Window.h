#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Window
{

public:

	// GLFW
	GLFWwindow* m_Window;

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Framebuffers
	bool framebufferResized = false; // used to recreate the swap chain


public:

	Window();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	~Window();

};
