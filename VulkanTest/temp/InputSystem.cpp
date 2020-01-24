#include "InputSystem.h"

#include <stdexcept>


// TODO Begin
enum  class MouseButtons
{
	VK_LBUTTON = 0,
	VK_MBUTTON = 1,
	VK_RBUTTON = 2,
};

struct CurrentMousePos
{
	int x;
	int y;
};

void GetCursorPos(CurrentMousePos* currentMousePos)
{
	std::logic_error("Function not yet implemented");
}

bool GetKeyboardState(unsigned char* m_keys_state)
{
	std::logic_error("Function not yet implemented");
	return false;
}

void SetCursorPos(int m_x, int m_y)
{
	std::logic_error("Function not yet implemented");
}

void ShowCursor(bool show)
{
	std::logic_error("Function not yet implemented");
}
// TODO End


InputSystem::InputSystem()
{
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}

void InputSystem::update()
{
	CurrentMousePos current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		// There is a mouse move event
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// Key is down
			if (m_keys_state[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
				while (it != m_set_listeners.end())
				{
					if (m_keys_state[i] != m_old_keys_state[i])
					{
						if (i == (unsigned int)MouseButtons::VK_LBUTTON)
						{
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == (unsigned int)MouseButtons::VK_RBUTTON)
						{
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == (unsigned int)MouseButtons::VK_MBUTTON)
						{
							(*it)->onMiddleMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
					}
					else
					{
						(*it)->onKeyDown(i);
					}

					++it;
				}
			}
			else // Key is up
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();
					while (it != m_set_listeners.end())
					{
						if (i == (unsigned int)MouseButtons::VK_LBUTTON)
						{
							(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == (unsigned int)MouseButtons::VK_RBUTTON)
						{
							(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == (unsigned int)MouseButtons::VK_MBUTTON)
						{
							(*it)->onMiddleMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
						{
							(*it)->onKeyUp(i);
						}

						++it;
					}
				}
			}
		}

		// store current keys state to old keys state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem::~InputSystem()
{
}
