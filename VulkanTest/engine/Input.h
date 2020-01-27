#pragma once

#include <utility>


class Input
{
public:

	static Input* Get();
	inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
	inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); };
	inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); };
	inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
	inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); };

protected:

	virtual bool IsKeyPressedImpl(int keycode);
	virtual bool IsMouseButtonPressedImpl(int button);
	virtual std::pair<float, float> GetMousePositionImpl();
	virtual float GetMouseXImpl();
	virtual float GetMouseYImpl();

private:

	static Input* s_Instance;


};
