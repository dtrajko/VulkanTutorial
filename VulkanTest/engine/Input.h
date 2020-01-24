#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


#define MAX_KEYS  1024
#define MAX_BUTTONS 32


class Input
{
public:

	bool m_Keys[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];
	float mouseX;
	float mouseY;
	float m_MouseWheelOffsetX;
	float m_MouseWheelOffsetY;
	glm::vec2 currentPosition;

public:

	Input();
	static Input* get();
	void update();

	bool isKeyPressed(unsigned int keyCode) const;
	bool isMouseButtonPressed(unsigned int button) const;
	void setMousePosition(float xpos, float ypos);
	virtual ~Input();

};
