#include "Input.h"
#include "Window.h"


#include <GLFW/glfw3.h>


Input* Input::s_Instance = new Input();

Input* Input::Get()
{
	return s_Instance;
}

bool Input::IsKeyPressedImpl(int keycode)
{
	auto window = Window::Get()->m_Window;
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressedImpl(int button)
{
	auto window = Window::Get()->m_Window;;
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePositionImpl()
{
	auto window = Window::Get()->m_Window;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return { (float)xpos, (float)ypos };
}

float Input::GetMouseXImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return x;
}

float Input::GetMouseYImpl()
{
	auto [x, y] = GetMousePositionImpl();
	return y;
}
