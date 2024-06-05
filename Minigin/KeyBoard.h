#pragma once
#include "SDL.h"
#include <memory>

class Keyboard
{
public:

	Keyboard(unsigned int controllerIndex);
	~Keyboard();

	void Update();

	bool IsDown(int button) const;
	bool IsUp(int button) const;
	bool IsPressed(int button) const;

private:
	Uint8* m_pPreviousKeyboardState;
	Uint8* m_pCurrentKeyboardState;
};

