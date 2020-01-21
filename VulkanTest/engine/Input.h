#pragma once

#define MAX_KEYS  1024
#define MAX_BUTTONS 32

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Window.h"


class Window;


class Input
{

private:

	GLFWwindow * glfwWindow;
	Window * m_Window;
	glm::vec2 displayVector;
	glm::vec2 previousPosition;
	glm::vec2 currentPosition;
	bool * keys;
	bool * buttons;

	// temporary values
	bool mode3D = true; // m_Window->getWindowOptions()->mode3D

public:
	bool m_Keys[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];
	double mouseX;
	double mouseY;
	double m_MouseWheelOffsetX;
	double m_MouseWheelOffsetY;

public:
	Input(Window * window);
	void init();
	void update();
	bool isKeyPressed(unsigned int keyCode) const;
	bool isKeyDown(unsigned int keyCode) const;
	bool isMouseButtonPressed(unsigned int button) const;
	glm::vec2 getMousePosition() const;
	void setMousePosition(double xpos, double ypos);
	double getMouseWheelDeltaY();
	glm::vec2 getDisplayVector();
	virtual ~Input();

	static void key_callback(GLFWwindow * glfwWindow, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow * glfwWindow, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow * glfwWindow, int button, int action, int mods);
	static void mouse_scroll_callback(GLFWwindow * glfwWindow, double xoffset, double yoffset);
	static void window_resize(GLFWwindow * glfwWindow, int width, int height);

	// TODO: 
	// getDisplayVector()
	// getMousePosition()
	// getMouseWheelDelta()
	// isLeftButtonPressed()
	// isRightButtonPressed()
	// isKeyDown()
	// isKeyPressed()
	// isKeyReleased()
	// isMouseButtonDown()
	// isMouseButtonPressed()
	// isMouseButtonReleased()
};
