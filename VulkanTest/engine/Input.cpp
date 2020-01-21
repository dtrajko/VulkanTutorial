#include "Input.h"


void Input::key_callback(GLFWwindow * glfwWindow, int key, int scancode, int action, int mods)
{
	Window * window = (Window*) glfwGetWindowUserPointer(glfwWindow);
	// window->getInput()->m_Keys[key] = action != GLFW_RELEASE;
}

void Input::mouse_button_callback(GLFWwindow * glfwWindow, int button, int action, int mods)
{
	Window * window = (Window*) glfwGetWindowUserPointer(glfwWindow);
	// window->getInput()->m_MouseButtons[button] = action != GLFW_RELEASE;
}

void Input::mouse_scroll_callback(GLFWwindow * glfwWindow, double xoffset, double yoffset)
{
	Window * window = (Window*)glfwGetWindowUserPointer(glfwWindow);
	// window->getInput()->m_MouseWheelOffsetX = xoffset;
	// window->getInput()->m_MouseWheelOffsetY = yoffset;
}

void Input::cursor_position_callback(GLFWwindow * glfwWindow, double xpos, double ypos)
{
	Window * window = (Window*) glfwGetWindowUserPointer(glfwWindow);
	// window->getInput()->setMousePosition(xpos, ypos);
}

void Input::window_resize(GLFWwindow * glfwWindow, int width, int height)
{
	Window * window = (Window*)glfwGetWindowUserPointer(glfwWindow);
	// window->setWidth(width);
	// window->setHeight(height);
}

Input::Input(Window * window)
{
	displayVector = glm::vec2();
	previousPosition = glm::vec2();
	currentPosition = glm::vec2();
	m_Window = window;
	mouseX = 0.0;
	mouseY = 0.0;
	init();

	std::cout << "Input object instanced and initialized!" << std::endl;
}

void Input::init()
{
	for (int indKeys = 0; indKeys < MAX_KEYS; indKeys++)
	{
		m_Keys[indKeys] = false;
	}

	for (int indButtons = 0; indButtons < MAX_BUTTONS; indButtons++)
	{
		m_MouseButtons[indButtons] = false;
	}

	// GLFW callbacks
	glfwWindow = glfwWindow; // m_Window->getHandle();
	glfwSetWindowUserPointer(glfwWindow, m_Window);
	glfwSetWindowSizeCallback(glfwWindow, window_resize);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
	glfwSetScrollCallback(glfwWindow, mouse_scroll_callback);
	glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);

	if (mode3D) {
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(glfwWindow, m_Window->WIDTH / 2, m_Window->HEIGHT / 2);
	}

}

void Input::update()
{

	for (int indKey = 0; indKey < GLFW_KEY_LAST; indKey++)
	{
		m_Keys[indKey] = isKeyPressed(indKey);
	}
	for (int indButton = 0; indButton < GLFW_MOUSE_BUTTON_LAST; indButton++)
	{
		m_MouseButtons[indButton] = isMouseButtonPressed(indButton);
	}

	displayVector.x = 0;
	displayVector.y = 0;
	if (previousPosition.x != 0 && previousPosition.y != 0) {
		double deltaX = currentPosition.x - previousPosition.x;
		double deltaY = currentPosition.y - previousPosition.y;
		bool rotateX = deltaX != 0;
		bool rotateY = deltaY != 0;
		if (rotateX) {
			displayVector.y = (float)deltaX;
		}
		if (rotateY) {
			displayVector.x = (float)deltaY;
		}
	}
	previousPosition.x = currentPosition.x;
	previousPosition.y = currentPosition.y;
}

bool Input::isKeyPressed(unsigned int keyCode) const
{
	if (keyCode >= MAX_KEYS)
	{
		return false;
	}
	return m_Keys[keyCode];
}

bool Input::isKeyDown(unsigned int keyCode) const
{
	return glfwGetKey(glfwWindow, keyCode) == GL_TRUE;
}

bool Input::isMouseButtonPressed(unsigned int buttonCode) const
{
	if (buttonCode >= MAX_BUTTONS)
	{
		return false;
	}
	return m_MouseButtons[buttonCode];
}

glm::vec2 Input::getMousePosition() const
{
	return glm::vec2(mouseX, mouseY);
}

void Input::setMousePosition(double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	currentPosition.x = (float) xpos;
	currentPosition.y = (float) ypos;
}

double Input::getMouseWheelDeltaY() {
	double mouseWheelDeltaY = m_MouseWheelOffsetY;
	m_MouseWheelOffsetY = 0;
	return mouseWheelDeltaY;
}

glm::vec2 Input::getDisplayVector()
{
	return displayVector;
}

Input::~Input()
{
}
